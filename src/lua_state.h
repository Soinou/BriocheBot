// ircbot2
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

#ifndef LUA_STATE_H_
#define LUA_STATE_H_

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include <string>

// Lua namespace
namespace Lua
{
    // Lua state
    class State
    {
    private:
        // The lua state
        lua_State* state_;

        // Gets the error on the stack
        std::string get_error();

    public:
        // Constructor
        State();

        // Destructor
        ~State();

        // Get the lua state
        lua_State* state() const
        {
            return state_;
        }

        // Open all the standard and user defined libraries
        void open_libs();

        // Loads a file (Throws an exception if something occurs)
        void do_file(const std::string& file_path);

        // Get an integer from a variable name
        int get_integer(const std::string& variable_name);

        // Get a string from a variable name
        std::string get_string(const std::string& variable_name);

        // Get a function to the top of the stack (Throws an exception if something occurs)
        void get_function(const std::string& function_name);

        // Calls a method (You must get the function then push the function arguments first) (Throws an exception if something occurs)
        void call(int arguments_count, int results_count);

        // Push a string on the stack
        void push(const std::string& string);

        // Push an integer on the stack
        void push(int integer);

        // Operator * (Allows to write lua functions with *Lua::State instead of Lua::state.state())
        inline lua_State* operator*()
        {
            return state_;
        }
    };
};

#endif // LUA_STATE_H_
