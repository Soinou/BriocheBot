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

#include "utils/http.h"

#include "utils/logger.h"
#include "utils/utils.h"

#include <happyhttp.h>

Http::Http() : connection_(nullptr), buffer_(), status_(0)
{

}

Http::~Http()
{

}

// Called when the server sends headers back to us
static void on_begin(const happyhttp::Response* response, void* userdata)
{
    // Nothing to do here
}

// Called when the server sends data back to us
static void on_data(const happyhttp::Response* response, void* userdata, const unsigned char* data, int count)
{
    // Get the calling instance of the http object
    Http* http = (Http*)userdata;
    
    // Append the sent data to our buffer
    http->buffer().append(std::string(data, data + count));
}

// Called when the server completes the request
static void on_complete(const happyhttp::Response* response, void* userdata)
{
    // Get the calling instance of the http object
    Http* http = (Http*)userdata;

    // Change our actual status
    http->set_status(response->getstatus());
}

std::string Http::get(const std::string& url, int port, const std::string& route)
{
    try
    {
        // Create a new connection
        connection_ = new happyhttp::Connection(url.c_str(), port);

        // Set the connection callbacks
        connection_->setcallbacks(on_begin, on_data, on_complete, this);

        // Clear the buffer
        buffer_.clear();

        // Execute a request
        connection_->request("GET", route.c_str());


        // While our connection is alive
        while (connection_->outstanding())
            // Get responses
            connection_->pump();


        // Delete the connection
        delete connection_;

        // Reset it
        connection_ = nullptr;
    }
    // Catch http exceptions
    catch (happyhttp::Wobbly exception)
    {
        Meow("server")->error(Utils::string_format("Exception when performing get request: %s", exception.what()));
    }

    // Return our buffer
    return buffer_;
}
