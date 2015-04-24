// ircbot
// Copyright (C) 2014 Abricot Soinou (abricot.soinou@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.

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
