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

#include "uv/dns.h"

#include "uv/loop.h"
#include "utils/utils.h"

#include <uv.h>

namespace Uv
{
    // Get address context
    struct get_address_context_t
    {
        // The callback
        address_callback callback;
    };

    // Called on getaddrinfo completion
    static void on_getaddrinfo(uv_getaddrinfo_t* req, int status, struct addrinfo* res)
    {
        // Get context
        get_address_context_t* context = static_cast<get_address_context_t*>(req->data);

        // Error
        if (status < 0)
        {
            // Callback with errors
            context->callback(false, Utils::string_format("Impossible to retrieve address informations: %s", uv_strerror(status)), sockaddr());

            // Delete context and request
            delete context;
            delete req;
        }
        // No error
        else
        {
            // Call the callback with success
            context->callback(true, "", *res->ai_addr);

            // Delete context and request
            delete context;
            delete req;

            // Free result
            uv_freeaddrinfo(res);
        }
    }

    void get_address(std::string host, std::string port, address_callback callback)
    {
        // Create a new context
        get_address_context_t* context = new get_address_context_t();

        // Set context callback
        context->callback = callback;

        // Create a new request
        uv_getaddrinfo_t* req = new uv_getaddrinfo_t();

        // Pass the context to the request
        req->data = context;

        // Create some hints
        struct addrinfo hints;
        hints.ai_family = PF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = 0;

        // Call getaddrinfo
        int res = uv_getaddrinfo(*the_loop, req, on_getaddrinfo, host.c_str(), port.c_str(), &hints);

        // Impossible to call function
        if (res)
        {
            // Callback with error
            callback(false, Utils::string_format("Impossible to call getaddrinfo function: %s", uv_strerror(res)), sockaddr());

            // Delete context and request
            delete context;
            delete req;
        }
    }
}
