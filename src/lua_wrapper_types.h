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

#ifndef LUA_WRAPPER_TYPES_H_
#define LUA_WRAPPER_TYPES_H_

#include <string>

// Redefinition of luaU_Impl for the string type
template<>
struct luaU_Impl < std::string >
{
    // Check for a string type
    static std::string luaU_check(lua_State* L, int index)
    {
        return std::string(luaL_checkstring(L, index));
    }

    // Convert something to a string type
    static std::string luaU_to(lua_State* L, int index)
    {
        return std::string(lua_tostring(L, index));
    }

    // Push a string type
    static void luaU_push(lua_State* L, const std::string& val)
    {
        lua_pushstring(L, val.c_str());
    }
};

#endif // LUA_WRAPPER_TYPES_H_
