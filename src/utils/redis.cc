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

#include "utils/redis.h"

#include "utils/utils.h"
#include <errno.h>

namespace Redis
{
    Connection::Connection(const std::string& host, int port) : redis_context_(redisConnect(host.c_str(), port))
    {
        // If the connection is not established
        if (redis_context_ == nullptr || redis_context_->err)
        {
            fprintf(stderr, "redis error: %d - %d - %s\n", errno, redis_context_->err, redis_context_->errstr);
            // Error
            Utils::throw_error("Connection", "Constructor",
                               Utils::string_format("Impossible to connect to the redis server: %s", redis_context_->errstr));
        }
    }

    Connection::~Connection()
    {
        // If we have a context
        if (redis_context_ != nullptr)
            // Delete it
            redisFree(redis_context_);
    }

    Reply Connection::parse_reply(redisReply* reply)
    {
        // Create a reply object
        Reply new_reply;

        // If the reply is a nullptr
        if (reply == nullptr)
        {
            // No reply
            new_reply.type = Reply::None;
        }
        // Else
        else
        {
            // Based on the reply type, parse the reply object correctly
            switch (reply->type)
            {
                case REDIS_REPLY_STATUS:
                    new_reply.type = Reply::Status;
                    new_reply.string = std::string(reply->str);
                    break;
                case REDIS_REPLY_ERROR:
                    new_reply.type = Reply::Error;
                    new_reply.string = std::string(reply->str);
                    break;
                case REDIS_REPLY_INTEGER:
                    new_reply.type = Reply::Integer;
                    new_reply.integer = reply->integer;
                    break;
                case REDIS_REPLY_NIL:
                    new_reply.type = Reply::Nil;
                    new_reply.integer = -1;
                    break;
                case REDIS_REPLY_STRING:
                    new_reply.type = Reply::String;
                    new_reply.string = std::string(reply->str);
                    break;
                case REDIS_REPLY_ARRAY:
                    new_reply.type = Reply::Array;
                    // Do something about this, but later, too lazy atm
                    break;
                default:
                    break;
            }
        }

        // Return the reply
        return new_reply;
    }

    Reply Connection::execute(const std::string& command)
    {
        // Create a reply holder
        redisReply* reply = nullptr;

        // Call the redis command function
        reply = (redisReply*)redisCommand(redis_context_, command.c_str());

        // Parse the reply we got
        Reply new_reply = parse_reply(reply);

        // If the reply exists
        if (reply != nullptr)
            // Delete the reply object
            freeReplyObject(reply);

        // Now that we have a reply, parse it and return it
        return new_reply;
    }
}
