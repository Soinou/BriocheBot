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

#include "lua/server.h"

#include "lua/wrapper.h"
#include "lua/wrapper_utils.h"
#include "lua/wrapper_types.h"

#include "models/viewer.h"
#include "models/streamer.h"
#include "server/server.h"
#include "utils/logger.h"

// Call to the Server.current_streamer method
static int lua_server_current_streamer(lua_State* L)
{
    // Get the server
    Server* server = luaW_check<Server>(L, 1);

    // Get the current streamer
    Viewer* viewer = server->current_streamer();

    // No current streamer
    if (viewer == nullptr)
        // Push nil
        lua_pushnil(L);
    // A current streamer
    else
        // Push the current streamer
        luaW_push<Viewer>(L, viewer);

    // One result
    return 1;
}

// Call to the Server.set_current_streamer method
static int lua_server_set_current_streamer(lua_State* L)
{
    // Get the server
    Server* server = luaW_check<Server>(L, 1);

    // If the next argument is nil
    if (lua_isnil(L, 2))
        // Remove the current streamer
        server->set_current_streamer(nullptr);
    // Else
    else
    {
        // Get the player
        Viewer* viewer = luaW_check<Viewer>(L, 2);

        try
        {
            // Set this player as the server current streamer
            server->set_current_streamer(dynamic_cast<Streamer*>(viewer));
        }
        // On bad cast
        catch (std::bad_cast e)
        {
            // Print some error
            Meow("lua")->error("Error: trying to set a non-streamer as current streamer");
        }
    }

    // No result
    return 0;
}

// Call to the Server.up_time method
static int lua_server_up_time(lua_State* L)
{
    // Get the server
    Server* server = luaW_check<Server>(L, 1);

    // Push the stream time
    lua_pushinteger(L, server->up_time());

    // One result
    return 1;
}

// Call to the Server.stream_time method
static int lua_server_stream_time(lua_State* L)
{
    // Get the server
    Server* server = luaW_check<Server>(L, 1);

    // Push the stream time
    lua_pushinteger(L, server->stream_time());

    // One result
    return 1;
}

// Call to the Server.send_twitch method
static int lua_server_send_twitch(lua_State* L)
{
    // Get the server
    Server* server = luaW_check<Server>(L, 1);

    // Get the message
    std::string message(luaL_checkstring(L, 2));

    // Send the message
    server->send_twitch(message);

    // No result
    return 0;
}

// Call to the Server.send_osu method
static int lua_server_send_osu(lua_State* L)
{
    // Get the server
    Server* server = luaW_check<Server>(L, 1);

    // Get the message
    std::string message(luaL_checkstring(L, 2));

    // Send the message
    server->send_osu(message);

    // No result
    return 0;
}

// Call to the server.stop method
static int lua_server_stop(lua_State* L)
{
    // Get the server
    Server* server = luaW_check<Server>(L, 1);

    // Stop the server
    server->stop();

    // Return
    return 0;
}

// Metatable
static const luaL_Reg lua_server_meta[] =
{
    // Up time getter
    { "upTime", lua_server_up_time },

    // Stream time getter
    { "streamTime", lua_server_stream_time },

    // Twitch username getter
    { "sendTwitch", lua_server_send_twitch },

    // osu! username getter
    { "sendOsu", lua_server_send_osu },

    // Skin getter
    { "getCurrentStreamer", lua_server_current_streamer },

    // Skin setter
    { "setCurrentStreamer", lua_server_set_current_streamer },

    // Stop the server
    { "stop", lua_server_stop },

    // Sentinel
    { 0, 0 }
};

// Static metatable
static const luaL_Reg lua_server_static_meta[] =
{
    // Sentinel
    { 0, 0 }
};

// Register the server library
int luaopen_Server(lua_State* L)
{
    luaW_register<Server>(L, "Server", lua_server_static_meta, lua_server_meta);
}
