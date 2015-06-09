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

#ifndef UV_BUFFER_H_
#define UV_BUFFER_H_

#include <uv.h>

#include <string>

namespace Uv
{
    // Represents a buffer wrapper
    class Buffer
    {
    private:
        // The underlying buffer
        uv_buf_t buffer_;

    public:
        // Default constructor
        Buffer();

        // Constructor with a string
        Buffer(const std::string& data);

        // Destructor
        ~Buffer();

        // Allocate space for the buffer
        void allocate(int size);

        // Free the previous space used by the buffer
        void free();

        // Get the underlying buffer
        inline const uv_buf_t& get() const
        {
            return buffer_;
        }

        // Get the buffer size
        inline int size() const
        {
            return buffer_.len;
        }

        // Overload to the dereference operator
        char* operator*()
        {
            return buffer_.base;
        }

        // Overload of the reference operator
        uv_buf_t* operator&()
        {
            return &buffer_;
        }
    };
}

#endif // UV_BUFFER_H_
