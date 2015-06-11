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

#include "uv/work.h"

#include "uv/loop.h"

namespace Uv
{
    // Represents a queued work context
    struct WorkContext
    {
        // The runnable function
        runnable_function runnable;
    };

    // Called once the queued work needs to be done
    static void on_do_work(uv_work_t* request)
    {
        // Get the context
        WorkContext* context = static_cast<WorkContext*>(request->data);

        // Execute the runnable function
        context->runnable();
    }

    // Called once the queued work is done
    static void on_after_work(uv_work_t* request, int status)
    {
        // Get the context
        WorkContext* context = static_cast<WorkContext*>(request->data);

        // Delete the context and the request
        delete context;
        delete request;
    }

    void run(runnable_function runnable)
    {
        // Create a new context
        WorkContext* context = new WorkContext();

        // Store the runnable
        context->runnable = runnable;

        // Create a new request
        uv_work_t* request = new uv_work_t();

        // Store the context as request data
        request->data = static_cast<void*>(context);

        // Queue the runnable
        uv_queue_work(*the_loop, request, on_do_work, on_after_work);
    }
}
