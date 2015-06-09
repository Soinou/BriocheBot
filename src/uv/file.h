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

#ifndef UV_FILE_H_
#define UV_FILE_H_

#include "uv/buffer.h"

#include <uv.h>

#include <functional>

namespace Uv
{
    // Represents a file, on which we can perform read/write operations
    class File
    {
    public:
        // Open callback
        typedef std::function<void(File*, bool)> open_callback;

        // Size callback
        typedef std::function<void(File*, uint64_t)> size_callback;

        // Read callback
        typedef std::function<void(File*, std::string, bool)> read_callback;

        // Write callback
        typedef std::function<void(File*, bool)> write_callback;

        // Close callback
        typedef std::function<void(File*)> close_callback;

    private:
        // If the file is opening
        bool opening_;

        // The current file descriptor
        int descriptor_;

        // The internal buffer (For read operations)
        Buffer internal_buffer_;

        // The string buffer (Total data read)
        std::string string_buffer_;

        // On open
        open_callback on_open_;

        // On size
        size_callback on_size_;

        // On read
        read_callback on_read_;

        // On write
        write_callback on_write_;

        // On close
        close_callback on_close_;

    public:
        // Constructor
        File();

        // Destructor
        ~File();

        // Opens a file
        void open(const std::string& path, bool append);

        // Get the size of the file
        void size();

        // Reads data from the file
        void read();

        // Writes data to the file
        void write(const std::string& data);

        // Closes the file
        void close();

        // Opening getter
        inline bool opening() const
        {
            return opening_;
        }

        // If the file is currently opened or not
        inline bool opened() const
        {
            return descriptor_ != -1;
        }

        // On open setter
        inline void set_on_open(open_callback on_open)
        {
            on_open_ = on_open;
        }

        // On size setter
        inline void set_on_size(size_callback on_size)
        {
            on_size_ = on_size;
        }

        // On read setter
        inline void set_on_read(read_callback on_read)
        {
            on_read_ = on_read;
        }

        // On write setter
        inline void set_on_write(write_callback on_write)
        {
            on_write_ = on_write;
        }

        // On close setter
        inline void set_on_close(close_callback on_close)
        {
            on_close_ = on_close;
        }
    };
}

#endif // UV_FILE_H_
