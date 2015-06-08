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

#ifndef UV_TIMER_H_
#define UV_TIMER_H_

#include <uv.h>

#include <functional>

namespace Uv
{
    // Represents a simple timer, calling a function each x milliseconds
    class Timer
    {
    public:
        // Timer callback
        typedef std::function <void(Timer*)> timer_callback;

    private:
        // If the timer is running
        bool running_;

        // Internal uv timer
        uv_timer_t timer_;

        // The interval
        int interval_;

        // The callback
        timer_callback callback_;

    public:
        // Constructor
        Timer(int interval, timer_callback callback);

        // Destructor
        ~Timer();

        // Running getter
        inline bool running() const
        {
            return running_;
        }

        // Timer getter
        inline const uv_timer_t& timer() const
        {
            return timer_;
        }

        // Interval getter
        inline int interval() const
        {
            return interval_;
        }

        // Callback getter
        inline timer_callback callback() const
        {
            return callback_;
        }

        // Starts the timer
        void start();

        // Stops the timer
        void stop();
    };
}

#endif // UV_TIMER_H_