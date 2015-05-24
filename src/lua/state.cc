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

#include "lua/state.h"

#include "lua/logger.h"
#include "lua/player.h"
#include "lua/request.h"
#include "lua/server.h"
#include "lua/utils.h"
#include "utils/utils.h"

#include <cstdio>

namespace Lua
{
    State::State() : state_(luaL_newstate())
    {

    }

    State::~State()
    {
        // If we have a state
        if (state_)
            // Close it
            lua_close(state_);
    }

    std::string State::get_error()
    {
        // Pop the error on the stack and return it
        std::string error(luaL_checkstring(state_, -1));

        // Pop the error from the stack
        lua_pop(state_, -1);

        // Return the error message
        return error;
    }

    void State::open_libs()
    {
        // Open standard libraries
        luaL_openlibs(state_);

        // Open user defined libraries
        luaopen_Player(state_);
        luaopen_Request(state_);
        luaopen_Server(state_);
        luaopen_Logger(state_);
        luaopen_Utils(state_);
    }

    void State::do_file(const std::string& file_path)
    {
        // If the file is not loaded correctly
        if (luaL_loadfile(state_, file_path.c_str()))
            // Throw an exception
            Utils::throw_error("Lua::State", "do_file", get_error());
        // Else, if executing the file does not work correctly
        else if (lua_pcall(state_, 0, 0, 0))
            // Throw an exception
            Utils::throw_error("Lua::State", "do_file", get_error());
    }

    int State::get_integer(const std::string& variable_name)
    {
        lua_getglobal(state_, variable_name.c_str());

        return luaL_checkinteger(state_, -1);
    }

    std::string State::get_string(const std::string& variable_name)
    {
        lua_getglobal(state_, variable_name.c_str());

        return std::string(luaL_checkstring(state_, -1));
    }

    void State::get_function(const std::string& function_name)
    {
        // Get the function on the stack
        lua_getglobal(state_, function_name.c_str());

        // If the field we got is a function
        if (!lua_isfunction(state_, -1))
            // Throw an error
            Utils::throw_error("Lua::State", "call", Utils::string_format("Field '%s' is not a function", function_name.c_str()));
    }

    void State::call(int arguments_count, int results_count)
    {
        // Else, if the function call returns an error
        if (lua_pcall(state_, arguments_count, results_count, 0))
            // Throw an error
            Utils::throw_error("Lua::State", "call", get_error());
    }

    void State::push(const std::string& string)
    {
        lua_pushstring(state_, string.c_str());
    }

    void State::push(int integer)
    {
        lua_pushinteger(state_, integer);
    }
}
