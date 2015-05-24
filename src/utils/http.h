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

#ifndef UTILS_HTTP_H_
#define UTILS_HTTP_H_

#include <string>

// Forward declaration of the happyhttp connection
namespace happyhttp
{
    class Connection;
}

// HTTP utility
class Http
{
private:
    // The http connection
    happyhttp::Connection* connection_;

    // The buffer
    std::string buffer_;

    // The last status
    int status_;

    // Constructor
    Http();

    // Delete copy constructor
    Http(const Http&) = delete;

    // Delete copy operator
    void operator=(const Http&) = delete;

public:
    // Destructor
    ~Http();

    // Get a string from an url
    std::string get(const std::string& url, int port, const std::string& route);

    // Buffer getter
    inline std::string& buffer()
    {
        return buffer_;
    }

    // Status getter
    inline int status() const
    {
        return status_;
    }

    // Status setter
    inline void set_status(int status)
    {
        status_ = status;
    }

    // Singleton get instance
    static Http& get_instance()
    {
        static Http instance;

        return instance;
    }

};

// Shortcut macro
#define Bringz Http::get_instance()

#endif // UTILS_HTTP_H_
