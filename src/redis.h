// ircbot2
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

#ifndef REDIS_H_
#define REDIS_H_

#include <hiredis/hiredis.h>

#include <string>

// Redis namespace
namespace Redis
{
    // A redis reply
    struct Reply
    {
        // Different types of replies
        enum Type
        {
            // No reply
            None,
            
            // Status reply
            Status,

            // Error reply
            Error,

            // Integer reply
            Integer,

            // Nil reply
            Nil,

            // String reply
            String,

            // Array reply
            Array,

            // Count of replies
            Count
        };

        // The type of reply
        Type type;

        // The string in case of a status, error or string reply
        std::string string;

        // The integer in case of an integer reply
        int integer;

        // The reply array in case of an array reply
        Reply* elements;
    };

    // A redis connection
    class Connection
    {
    private:
        // The redis context
        redisContext* redis_context_;

        // Parses a reply object from a redisReply object
        static Reply parse_reply(redisReply* reply);

    public:
        // Constructor
        Connection(const std::string& host, int port);

        // Destructor
        ~Connection();

        // Get the error code
        inline int error() const
        {
            return redis_context_->err;
        }

        // Get the error string
        inline std::string error_string() const
        {
            return std::string(redis_context_->errstr);
        }

        // Executes a command and returns a reply
        Reply execute(const std::string& command);
    };
}

#endif // REDIS_H_
