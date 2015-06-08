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

#ifndef UV_LOOP_H_
#define UV_LOOP_H_

#include "utils/non_copyable.h"

#include <uv.h>

// Namespace uv
namespace Uv
{
    // Represents an uv loop abstraction class
    class Loop : NonCopyable
    {
    private:
        // The base uv loop (Always the default loop)
        uv_loop_t* loop_;

        // Constructor
        Loop();

    public:
        // Destructor
        ~Loop();

        // Runs the loop
        void run();

        // Operator*
        inline uv_loop_t* operator*()
        {
            return loop_;
        }

        // Singleton operator
        inline static Loop& instance()
        {
            static Loop instance;

            return instance;
        }
    };
}

// Just redefine the default 
#define the_loop Uv::Loop::instance()

#endif // UV_LOOP_H_
