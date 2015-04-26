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

#ifndef CURL_H_
#define CURL_H_

#include <string>

// Curl wrapper namespace
namespace CurlWrapper
{
    class Connection
    {
    private:
        // Curl session
        void* session_;

        // String buffer
        std::string buffer;

        // Constructor
        Connection();

        // Delete copy constructor
        Connection(const Connection&) = delete;

        // Delete copy operator
        void operator=(const Connection&) = delete;

    public:
        // Destructor
        ~Connection();

        // Append some data to the buffer
        void append(const char* data);

        // Executes an HTTP GET request on the given url
        std::string perform_get(const std::string& url);

        // Singleton get instance
        static Connection& get_instance()
        {
            static Connection instance;

            return instance;
        }
    };
}

// Singleton get instance macro
#define Curl CurlWrapper::Connection::get_instance()

#endif // CURL_H_
