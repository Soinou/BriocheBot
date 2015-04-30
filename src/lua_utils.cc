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

#include "lua_utils.h"

#include "lua_wrapper.h"
#include "lua_wrapper_utils.h"
#include "lua_wrapper_types.h"

#include <math.h>

// Dummy
struct Utils
{

};

// Call to the Server.current_streamer method
static int lua_utils_get_time(lua_State* L)
{
    // Get the seconds
    int seconds = luaL_checkinteger(L, 1);

    // Get the minutes
    int minutes = seconds / 60;

    // Get the hours
    int hours = minutes / 60;

    // Push the hours
    lua_pushinteger(L, hours);

    // Push the minutes
    lua_pushinteger(L, minutes % 60);

    // Push the seconds
    lua_pushinteger(L, seconds % 60);

    // One result
    return 3;
}

// Metatable
static const luaL_reg lua_utils_meta[] =
{
    // Sentinel
    { 0, 0 }
};

// Static metatable
static const luaL_reg lua_utils_static_meta[] =
{
    // Get time
    { "getTime", lua_utils_get_time },

    // Sentinel
    { 0, 0 }
};

// Register the utils library
int luaopen_Utils(lua_State* L)
{
    luaW_register<Utils>(L, "Utils", lua_utils_static_meta, lua_utils_meta);
}
