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

#include "lua_request.h"

#include "lua_wrapper.h"
#include "lua_wrapper_utils.h"
#include "lua_wrapper_types.h"

#include "request.h"

// Call to the Request::parse method
static int lua_request_static_parse(lua_State* L)
{
    std::string sender(std::string(luaL_checkstring(L, 1)));

    // Get the message to parse
    std::string message(std::string(luaL_checkstring(L, 2)));

    // Parse the request
    Request* request = Request::parse(sender, message);

    // No request
    if (request == nullptr)
        // Nil
        lua_pushnil(L);
    // A request
    else
        // Push this request
        luaW_push(L, request);

    // One result
    return 1;
}

// Metatable
static const luaL_reg lua_request_meta[] =
{
    // To Twitch string
    { "toTwitchString", luaU_get < Request, std::string, &Request::to_twitch_string > },

    // To osu! string
    { "toOsuString", luaU_get < Request, std::string, &Request::to_osu_string > },

    // Sentinel
    { 0, 0 }
};

// Static metatable
static const luaL_reg lua_request_static_meta[] =
{
    // Exists
    { "parse", lua_request_static_parse },

    // Sentinel
    { 0, 0 }
};

// Register the request library
int luaopen_Request(lua_State* L)
{
    luaW_register<Request>(L, "Request", lua_request_static_meta, lua_request_meta, NULL);
}
