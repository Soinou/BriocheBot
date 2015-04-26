// BriocheBot
// The MIT License(MIT)
//
// Copyright(c) 2015 Abricot Soinou <abricot.soinou@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "curl.h"

#include "logger.h"
#include "utils.h"

#include <curl/curl.h>

#include <cstdlib>
#include <cassert>

namespace CurlWrapper
{
    static size_t write_function(void* data, const size_t size, const size_t count, Connection* connection)
    {
        // Append the data we got to the connection buffer
        connection->append((char*)data);

        // Return the new size
        return size * count;
    }

    Connection::Connection() : session_(curl_easy_init())
    {
        // If the session is not initialized
        if (!session_)
            // Error
            Utils::throw_error("CurlWrapper::Connection", "Constructor", "Impossible to create a curl session");

        // Set the write callback function
        curl_easy_setopt(session_, CURLOPT_WRITEFUNCTION, write_function);

        // Set the user argument of the callback
        curl_easy_setopt(session_, CURLOPT_WRITEDATA, this);
    }

    Connection::~Connection()
    {
        // If we have a session
        if (session_)
            // Destroy it
            curl_easy_cleanup(session_);
    }

    void Connection::append(const char* data)
    {
        buffer.append(data);
    }

    std::string Connection::perform_get(const std::string& url)
    {
        // Ensure we have a session
        assert(session_);

        // The page content
        std::string content;

        // Set the curl url option
        curl_easy_setopt(session_, CURLOPT_URL, url.c_str());

        // Perform the request and get the response
        CURLcode response = curl_easy_perform(session_);

        // If the response is an ok
        if (response == CURLE_OK)
        {
            // Copy the buffer into the content variable
            content = buffer;

            // Clear the buffer
            buffer.clear();
        }
        else
            Log.error(Utils::string_format("Impossible to perform a get request! Server returned response %d", response));

        return content;
    }
}
