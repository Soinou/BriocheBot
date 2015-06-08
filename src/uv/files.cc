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

#include "uv/files.h"

#include "uv/loop.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <uv.h>

namespace Uv
{
    // Represents a read context
    struct read_context_t
    {
        // The read file descriptor (The file we want to read from)
        ssize_t file;

        // The buffer (Where all the data will be written)
        std::string buffer;

        // The base buffer (Where the uv buffer will store data)
        char base_buffer[1024];

        // The uv buffer (Where the uv operations will send data)
        uv_buf_t uv_buffer;

        // Callback (What will be called upon completion)
        file_callback callback;
    };

    // Called when the reader has received some data
    static void on_reader_data(uv_fs_t* req)
    {
        // Get the request context
        read_context_t* context = static_cast<read_context_t*>(req->data);

        // Error when reading from the file
        if (req->result < 0)
        {
            // Callback with error
            context->callback(false, "Impossible to read from the file");

            // Delete context and request
            delete context;
            delete req;
        }
        // End of file
        else if (req->result == 0)
        {
            // Create a new request
            uv_fs_t close_req;

            // Close the file (Synchronously)
            uv_fs_close(*the_loop, &close_req, req->result, NULL);

            // Callback with a success and the data we got
            context->callback(true, context->buffer);

            // Delete context and request
            delete context;
            delete req;
        }
        // We got data
        else
        {
            // The length we read is now the buffer length
            context->uv_buffer.len = req->result;

            // Copy the data to our string buffer
            context->buffer.append(context->uv_buffer.base, context->uv_buffer.base + context->uv_buffer.len);

            // Continue to read
            uv_fs_read(*the_loop, req, context->file, &context->uv_buffer, 1, -1, on_reader_data);
        }
    }

    // Called when a read operation has opened the file
    static void on_reader_open(uv_fs_t* req)
    {
        // Get the request context
        read_context_t* context = static_cast<read_context_t*>(req->data);

        // If the file could not be opened
        if (req->result < 0)
        {
            // Callback with error
            context->callback(false, "Impossible to open the file!");

            // Delete context and request
            delete context;
            delete req;
        }
        // File could be opened
        else
        {
            // Initialize our buffer
            context->uv_buffer = uv_buf_init(context->base_buffer, sizeof(context->base_buffer));

            // Store the file descriptor in our context
            context->file = req->result;

            // Create a new request
            uv_fs_t* read_req = new uv_fs_t();

            // Put our context in the request
            read_req->data = context;

            // Start read action
            uv_fs_read(*the_loop, read_req, req->result, &context->uv_buffer, 1, -1, on_reader_data);

            // Delete the open request
            delete req;
        }
    }

    void read_contents(const std::string& file_path, file_callback callback)
    {
        // Create a new context
        read_context_t* context = new read_context_t();

        // Set context callback
        context->callback = callback;

        // Create a new open request
        uv_fs_t* open_req = new uv_fs_t();

        // Store our context in the request
        open_req->data = context;

        // Start a new open operation
        uv_fs_open(*the_loop, open_req, file_path.c_str(), O_RDONLY, 0, on_reader_open);
    }

    // Represents a write context
    struct write_context_t
    {
        // The file descriptor (The file we want to write to)
        ssize_t file;

        // The content we want to write (What will be written to the file)
        std::string content;

        // The buffer (What will be used to write to the file)
        uv_buf_t buffer;

        // The callback (What will be called when everything is written)
        file_callback callback;
    };

    // Called when the writer has written some data
    void on_writer_data(uv_fs_t* req)
    {
        // Get the request context
        write_context_t* context = static_cast<write_context_t*>(req->data);

        // Error when writing to the file
        if (req->result < 0)
        {
            // Callback with error
            context->callback(false, "Impossible to write to the file");

            // Delete context and request
            delete context;
            delete req;
        }
        // Write successful
        else
        {
            // Create a new request
            uv_fs_t close_req;

            // Close the file (Synchronously)
            uv_fs_close(*the_loop, &close_req, req->result, NULL);

            // Callback with a success and an empty string
            context->callback(true, "");

            // Delete context and request
            delete context;
            delete req;
        }
    }

    // Called when the writer has successfully opened the file
    void on_writer_open(uv_fs_t* req)
    {
        // Get the request context
        write_context_t* context = static_cast<write_context_t*>(req->data);

        // If the file could not be opened
        if (req->result < 0)
        {
            // Callback with error
            context->callback(false, "Impossible to open the file!");

            // Delete context and request
            delete context;
            delete req;
        }
        // File could be opened
        else
        {
            // Initialize our buffer
            context->buffer = uv_buf_init((char*)context->content.c_str(), context->content.length());

            // Store the file descriptor in our context
            context->file = req->result;

            // Create a new request
            uv_fs_t* write_req = new uv_fs_t();

            // Put our context in the request
            write_req->data = context;

            // Start read action
            uv_fs_write(*the_loop, write_req, req->result, &context->buffer, 1, -1, on_writer_data);

            // Delete the open request
            delete req;
        }
    }

    void write_contents(const std::string& file_path, const std::string& content, bool append, file_callback callback)
    {
        // Create a new context
        write_context_t* context = new write_context_t();

        // Set context callback
        context->callback = callback;

        // Set context content
        context->content = content;

        // Create a new open request
        uv_fs_t* open_req = new uv_fs_t();

        // Store our context in the request
        open_req->data = context;

        // Set file open flags (Create + Append or Create + Write)
        int flags = append ? (O_CREAT | O_APPEND) : (O_CREAT | O_WRONLY);

        // Start a new open operation
        uv_fs_open(*the_loop, open_req, file_path.c_str(), flags, 00666, on_writer_open);
    }

    // Informations context
    struct infos_context_t
    {
        // The callback
        size_callback callback;
    };

    void on_infos_open(uv_fs_t* req)
    {
        // Get the context
        infos_context_t* context = static_cast<infos_context_t*>(req->data);

        // Error when accessing file
        if (req->result < 0)
        {
            // Call the callback with error
            context->callback(false, 0);

            // Delete context and request
            delete context;
            delete req;
        }
        // Access successful
        else
        {
            // Call the callback with success
            context->callback(true, req->statbuf.st_size);

            // Delete context and request
            delete context;
            delete req;
        }
    }

    void file_size(const std::string& file_path, size_callback callback)
    {
        // Create a new context
        infos_context_t* context = new infos_context_t();

        // Set context callback
        context->callback = callback;

        // Create a new infos request
        uv_fs_t* infos_req = new uv_fs_t();

        // Store our context in the request
        infos_req->data = context;

        // Start a new stat operation
        uv_fs_stat(*the_loop, infos_req, file_path.c_str(), on_infos_open);
    }
}
