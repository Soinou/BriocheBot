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

#ifndef EVENTS_EMITTER_H_
#define EVENTS_EMITTER_H_

#include <functional>
#include <vector>

// Represents an event emitter
template<typename ...Parameters>
class Emitter
{
private:
    // The list of callbacks
    std::vector<std::function<void(Parameters...)>> callbacks_;

public:
    // Constructor
    Emitter() : callbacks_() {}

    // Destructor
    ~Emitter() {}

    // Emit an event
    void emit(Parameters... parameters)
    {
        for (auto i = callbacks_.begin(); i != callbacks_.end(); i++)
            (*i)(parameters...);
    }

    // Add a callback to our emitter
    void operator+=(std::function<void(Parameters...)> callback)
    {
        callbacks_.push_back(callback);
    }
};

#endif // EVENTS_EMITTER_H_
