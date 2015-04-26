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

#ifndef REQUEST_BUILDER_H_
#define REQUEST_BUILDER_H_

#include <sstream>
#include <string>

// Curl wrapper namespace
namespace CurlWrapper
{
    // A request builder, to prepare a request before sending it
    class RequestBuilder
    {
    private:
        // The internal stream
        std::stringstream stream_;

    public:
        // Constructor
        RequestBuilder(const std::string& base_url);

        // Destructor
        ~RequestBuilder();

        // Appends a key/value pair to the request
        void append(const std::string& key, const std::string& value);

        // Appends a key/value pair to the request
        void append(const std::string& key, long value);

        // Appends a key/value pair to the request
        void append(const std::string& key, int value);

        // Builds the request
        inline std::string build() const
        {
            return stream_.str();
        }
    };
}

#endif // REQUEST_BUILDER_H_
