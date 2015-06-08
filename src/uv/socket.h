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

#ifndef UV_SOCKET_H_
#define UV_SOCKET_H_

#include "uv/buffer.h"

#include <uv.h>

#include <functional>
#include <string>

namespace Uv
{
    // Represents a tcp socket
    class Socket
    {
    public:
        ///////////////////////////////
        //
        // Typedefs
        //
        ///////////////////////////////

        // Connect callback
        typedef std::function<void(Socket*, bool)> connect_callback;

        // Write callback
        typedef std::function<void(Socket*, bool)> write_callback;

        // Read callback
        typedef std::function<void(Socket*, std::string)> data_callback;

        // Close callback
        typedef std::function<void(Socket*)> close_callback;

    private:
        ///////////////////////////////
        //
        // Members
        //
        ///////////////////////////////

        // If the socket is connected
        bool connected_;

        // The underlying socket
        uv_tcp_t socket_;

        // The on connect callback
        connect_callback on_connect_;

        // The on write callback
        write_callback on_write_;

        // The on read callback
        data_callback on_data_;

        // The on close callback
        close_callback on_close_;

    public:
        ///////////////////////////////
        //
        // Constructors/Destructors
        //
        ///////////////////////////////

        // Constructor
        Socket();

        // Destructor
        ~Socket();

        ///////////////////////////////
        //
        // Methods
        //
        ///////////////////////////////

        // Connects the socket to the given host
        void connect(const sockaddr& address);

        // Starts reading from the socket
        void read_start();

        // Stops reading from the socket
        void read_stop();

        // Writes to the socket
        void write(const std::string& data);

        // Closes the socket
        void close();

        ///////////////////////////////
        //
        // Getters/Setters
        //
        ///////////////////////////////

        // Connected getter
        inline bool connected() const
        {
            return connected_;
        }

        // On connect setter
        inline void set_on_connect(connect_callback on_connect)
        {
            on_connect_ = on_connect;
        }

        // On write setter
        inline void set_on_write(write_callback on_write)
        {
            on_write_ = on_write;
        }

        // On data setter
        inline void set_on_data(data_callback on_data)
        {
            on_data_ = on_data;
        }

        // On close setter
        inline void set_on_close(close_callback on_close)
        {
            on_close_ = on_close;
        }
    };
}

#endif // UV_SOCKET_H_
