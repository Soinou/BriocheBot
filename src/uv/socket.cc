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

#include "uv/socket.h"

#include "uv/loop.h"

namespace Uv
{
    Socket::Socket() : connected_(false), socket_(), on_connect_(), on_data_(), on_write_()
    {
        // Pass the socket as socket_ data
        socket_.data = static_cast<void*>(this);

        // Pass our socket to the main loop
        uv_tcp_init(*the_loop, &socket_);
    }

    Socket::~Socket()
    {
    }

    void Socket::connect(const sockaddr& address)
    {
        // Create a new connect request
        uv_connect_t* request = new uv_connect_t();

        // Pass the socket as request data
        request->data = static_cast<void*>(this);

        // Called when the socket is connected
        auto on_connect = [](uv_connect_t* request, int status)
        {
            // On connection, get the socket
            Socket* socket = static_cast<Socket*>(request->data);

            // Set connected to true
            socket->connected_ = true;

            // Call the given callback with status
            socket->on_connect_(socket, status >= 0);

            // Delete the request
            delete request;
        };

        // Connect the socket
        uv_tcp_connect(request, &socket_, &address, on_connect);
    }

    void Socket::read_start()
    {
        // Called when we need to allocate a new buffer
        auto on_alloc = [](uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
        {
            buf->base = new char[suggested_size];
            buf->len = suggested_size;
        };

        // Called when we have data
        auto on_data = [](uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
        {
            // Get the socket
            Socket* socket = static_cast<Socket*>(stream->data);

            // Call on data callback
            socket->on_data_(socket, std::string(buf->base, nread));

            // Delete the allocated buffer
            delete buf->base;
        };

        // Start to read
        uv_read_start(reinterpret_cast<uv_stream_t*>(&socket_), on_alloc, on_data);
    }

    void Socket::read_stop()
    {
        // Stop to read
        uv_read_stop(reinterpret_cast<uv_stream_t*>(&socket_));
    }

    void Socket::write(const std::string& data)
    {
        // Create a new request
        uv_write_t* request = new uv_write_t();

        // Pass the socket as request data
        request->data = static_cast<void*>(this);

        // Called when the write is finished
        auto on_write = [](uv_write_t* request, int status)
        {
            // Get the socket
            Socket* socket = static_cast<Socket*>(request->data);

            // Call socket callback
            socket->on_write_(socket, status >= 0);

            // Delete request
            delete request;
        };

        // Start a write action
        uv_write(request, reinterpret_cast<uv_stream_t*>(&socket_), &Buffer(data), 1, on_write);
    }

    void Socket::close()
    {
        auto on_close = [](uv_handle_t* handle)
        {
            // On close, get the socket
            Socket* socket = static_cast<Socket*>(handle->data);

            // Socket is no longer connected
            socket->connected_ = false;

            // Call the callback
            socket->on_close_(socket);
        };

        // Close the socket
        uv_close(reinterpret_cast<uv_handle_t*>(&socket_), on_close);
    }
}
