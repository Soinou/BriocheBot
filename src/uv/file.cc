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

#include "uv/file.h"

#include "uv/buffer.h"
#include "uv/loop.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

namespace Uv
{
    File::File() : opening_(false), descriptor_(-1), internal_buffer_(), string_buffer_()
    {

    }

    File::~File()
    {

    }

    void File::open(const std::string& path, bool append)
    {
        // If no file is opened yet
        if (descriptor_ == -1)
        {
            // Called when the 
            auto on_open = [](uv_fs_t* request)
            {
                // Get the file
                File* file = static_cast<File*>(request->data);

                // Error opening the file
                if (request->result < 0)
                    // Impossible to open
                    file->on_open_(file, false);
                // File opened
                else
                {
                    // Store the descriptor
                    file->descriptor_ = request->result;

                    // File is no longer opening
                    file->opening_ = false;

                    // Success
                    file->on_open_(file, true);
                }

                // Cleanup request
                uv_fs_req_cleanup(request);

                // Delete request
                delete request;
            };

            // Create a fs request
            uv_fs_t* request = new uv_fs_t();

            // Pass the file as request data
            request->data = static_cast<void*>(this);

            // File is opening
            opening_ = true;

            // Open flags
            int flags = append ? O_CREAT | O_APPEND : O_CREAT | O_RDWR;

            // Open the file
            uv_fs_open(*the_loop, request, path.c_str(), flags, 00666, on_open);
        }
    }

    void File::size()
    {
        // If we have a file currently opened
        if (descriptor_ != -1)
        {
            // Called when the stat request has finished
            auto on_stat = [](uv_fs_t* request)
            {
                // Get the file
                File* file = static_cast<File*>(request->data);

                // Error
                if (request->result < 0)
                    file->on_size_(file, -1);
                // No error
                else
                    file->on_size_(file, request->statbuf.st_size);

                // Cleanup request
                uv_fs_req_cleanup(request);

                // Delete request
                delete request;
            };

            // Create a new request
            uv_fs_t* request = new uv_fs_t();

            // Pass the file as request data
            request->data = static_cast<void*>(this);

            // Start a new stat operation
            uv_fs_fstat(*the_loop, request, descriptor_, on_stat);
        }
    }

    void File::read()
    {
        // If we have a file
        if (descriptor_ != -1)
        {
            // Called when we have data
            auto on_read = [](uv_fs_t* request)
            {
                // Get the file
                File* file = static_cast<File*>(request->data);

                // Error
                if (request->result < 0)
                {
                    // Call on read with error
                    file->on_read_(file, std::string(), false);

                    // Clean everything
                    file->internal_buffer_.free();
                    uv_fs_req_cleanup(request);
                    delete request;
                }
                // EOF
                else if (request->result == 0)
                {
                    // Call on read with success
                    file->on_read_(file, file->string_buffer_, true);

                    // Clean everything
                    file->internal_buffer_.free();
                    uv_fs_req_cleanup(request);
                    delete request;
                }
                // Data to read
                else
                {
                    // Append data to the internal buffer
                    file->string_buffer_.append(*file->internal_buffer_, request->result);

                    // Call the next file read
                    file->read();
                }
            };

            // Create a new request
            uv_fs_t* request = new uv_fs_t();

            // Set the file as request data
            request->data = static_cast<void*>(this);

            // Create a new buffer
            internal_buffer_.allocate(1024);

            // Start to read
            uv_fs_read(*the_loop, request, descriptor_, &internal_buffer_, 1, -1, on_read);
        }
    }

    void File::write(const std::string& data)
    {
        // If we have a file
        if (descriptor_ != -1)
        {
            // Called when the write is finished
            auto on_write = [](uv_fs_t* request)
            {
                // Get the file
                File* file = static_cast<File*>(request->data);

                if (request->result < 0)
                    printf("Error when writing to file: %s\n", uv_strerror(request->result));

                // Call file callback
                file->on_write_(file, request->result >= 0);

                // Cleanup the request
                uv_fs_req_cleanup(request);

                // Delete request
                delete request;
            };

            // Create a new request
            uv_fs_t* request = new uv_fs_t();

            // Pass the socket as request data
            request->data = static_cast<void*>(this);

            // Create a buffer for our data
            Buffer buffer(data);

            // Start a write action
            uv_fs_write(*the_loop, request, descriptor_, &buffer, 1, -1, on_write);
        }
    }

    void File::close()
    {
        // If we currently have a file
        if (descriptor_ != -1)
        {
            // Called when the file is successfully closed
            auto on_close = [](uv_fs_t* request)
            {
                // Get the file
                File* file = static_cast<File*>(request->data);

                // Call on close
                file->on_close_(file);

                // Cleanup the request
                uv_fs_req_cleanup(request);

                // Delete the request
                delete request;
            };

            // Create a new request
            uv_fs_t* request = new uv_fs_t();

            // Pass the file as request data
            request->data = static_cast<void*>(this);

            // Close the file
            uv_fs_close(*the_loop, request, descriptor_, on_close);

            // Reset descriptor (To avoid closing the file twice)
            descriptor_ = -1;
        }
    }
}
