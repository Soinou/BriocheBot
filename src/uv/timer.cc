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

#include "uv/timer.h"

#include "uv/loop.h"

namespace Uv
{
    Timer::Timer(int interval, timer_callback callback) : running_(false), timer_(), interval_(interval), callback_(callback)
    {
        // Pass the timer as the request context
        timer_.data = this;
    }

    Timer::~Timer()
    {

    }

    static void on_timer(uv_timer_t* timer)
    {
        // Get the timer context
        Timer* context = static_cast<Timer*>(timer->data);

        // Call the context callback
        context->callback()(context);
    }

    void Timer::start()
    {
        // If we're not running
        if (!running_)
        {
            // Set running to true
            running_ = true;

            // Add the timer to the event loop
            uv_timer_init(*the_loop, &timer_);

            // Start the timer
            uv_timer_start(&timer_, on_timer, 0, interval_);
        }
    }

    void Timer::stop()
    {
        // If we're running
        if (running_)
        {
            // Set running to false
            running_ = false;

            // Stop the timer
            uv_timer_stop(&timer_);
        }
    }
}
