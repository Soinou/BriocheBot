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

#include "lua/logger.h"

#include "lua/wrapper.h"
#include "lua/wrapper_utils.h"
#include "lua/wrapper_types.h"

#include "utils/logger.h"

#include <string>

// Call to the Logger.trace method
static int lua_logger_trace(lua_State* L)
{
    // Get the file
    std::string file(luaL_checkstring(L, 1));

    // Get the message
    std::string message(luaL_checkstring(L, 2));

    // Log the message
    Meow(file)->trace(message);

    // No result
    return 0;
}

// Call to the Logger.debug method
static int lua_logger_debug(lua_State* L)
{
    // Get the file
    std::string file(luaL_checkstring(L, 1));

    // Get the message
    std::string message(luaL_checkstring(L, 2));

    // Log the message
    Meow(file)->debug(message);

    // No result
    return 0;
}

// Call to the Logger.info method
static int lua_logger_info(lua_State* L)
{
    // Get the file
    std::string file(luaL_checkstring(L, 1));

    // Get the message
    std::string message(luaL_checkstring(L, 2));

    // Log the message
    Meow(file)->info(message);

    // No result
    return 0;
}

// Call to the Logger.warning method
static int lua_logger_warning(lua_State* L)
{
    // Get the file
    std::string file(luaL_checkstring(L, 1));

    // Get the message
    std::string message(luaL_checkstring(L, 2));

    // Log the message
    Meow(file)->warning(message);

    // No result
    return 0;
}

// Call to the Logger.error method
static int lua_logger_error(lua_State* L)
{
    // Get the file
    std::string file(luaL_checkstring(L, 1));

    // Get the message
    std::string message(luaL_checkstring(L, 2));

    // Log the message
    Meow(file)->error(message);

    // No result
    return 0;
}

// Call to the Logger.fatal method
static int lua_logger_fatal(lua_State* L)
{
    // Get the file
    std::string file(luaL_checkstring(L, 1));

    // Get the message
    std::string message(luaL_checkstring(L, 2));

    // Log the message
    Meow(file)->fatal(message);

    // No result
    return 0;
}

// Static metatable
static const luaL_Reg lua_logger_meta[] =
{
    // Sentinel
    { 0, 0 }
};

// Metatable
static const luaL_Reg lua_logger_static_meta[] =
{
    { "trace", lua_logger_trace },

    { "debug", lua_logger_debug },

    { "info", lua_logger_info },

    { "warning", lua_logger_warning },

    { "error", lua_logger_error },

    { "fatal", lua_logger_fatal },

    // Sentinel
    { 0, 0 }
};

// Register the server library
int luaopen_Logger(lua_State* L)
{
    luaW_register<Logger>(L, "Meow", lua_logger_static_meta, lua_logger_meta, NULL);
}
