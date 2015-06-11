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

#include "lua/viewer.h"

#include "lua/wrapper.h"
#include "lua/wrapper_utils.h"
#include "lua/wrapper_types.h"

#include "models/viewers.h"
#include "models/viewer.h"
#include "models/streamer.h"
#include "models/moderator.h"
#include "utils/logger.h"
#include "utils/utils.h"

////////////////////////////////////////////////////
//
// Constructors/Destructors
//
////////////////////////////////////////////////////

static int lua_viewer_static_new_viewer(lua_State* L)
{
    // Create a new viewer
    Viewer* viewer = new Viewer();

    // Push him
    luaW_push<Viewer>(L, viewer);

    // We have one result
    return 1;
}

static int lua_viewer_static_new_streamer(lua_State* L)
{
    // Create a new streamer
    Viewer* viewer = new Streamer();

    // Push him
    luaW_push<Viewer>(L, viewer);

    // We have one result
    return 1;
}

// Creates a new viewer
static int lua_viewer_static_new_moderator(lua_State* L)
{
    // Create a new moderator
    Viewer* viewer = new Moderator();

    // Push him
    luaW_push<Viewer>(L, viewer);

    // We have one result
    return 1;
}

// Deletes a viewer
static int lua_viewer_free(lua_State* L)
{
    // Get the viewer
    Viewer* viewer = luaW_check<Viewer>(L, 1);

    // Delete him
    delete viewer;

    // No result
    return 0;
}

////////////////////////////////////////////////////
//
// Static methods
//
////////////////////////////////////////////////////

// Call to the_viewers.exists method
static int lua_viewer_static_exists(lua_State* L)
{
    // Get the twitch username as an argument
    std::string twitch_username(std::string(luaL_checkstring(L, 1)));

    // Check if the player exists
    bool exists = the_viewers.exists(twitch_username);

    // Push the result
    lua_pushboolean(L, exists ? 1 : 0);

    // One result
    return 1;
}

// Call to the_viewers.get method
static int lua_viewer_static_get(lua_State* L)
{
    // Get the twitch username as an argument
    std::string twitch_username(std::string(luaL_checkstring(L, 1)));

    // Get the viewer
    Viewer* viewer = the_viewers.get(twitch_username);

    // If the viewer is not a dummy
    if (viewer != nullptr)
        // Push the viewer
        luaW_push<Viewer>(L, viewer);
    // Else
    else
        // Push nil
        lua_pushnil(L);

    // One result
    return 1;
}

////////////////////////////////////////////////////
//
// Viewer getters/setters
//
////////////////////////////////////////////////////

// Id getter
static int lua_viewer_id(lua_State* L)
{
    // Get the viewer
    Viewer* viewer = luaW_check<Viewer>(L, 1);

    // Push the viewer id
    lua_pushinteger(L, viewer->id());

    // One result
    return 1;
}

// Type getter
static int lua_viewer_type(lua_State* L)
{
    // Get the viewer
    Viewer* viewer = luaW_check<Viewer>(L, 1);

    // Push the viewer type
    lua_pushinteger(L, viewer->type());

    // One result
    return 1;
}

// Type setter
static int lua_viewer_set_type(lua_State* L)
{
    // Get the viewer
    Viewer* viewer = luaW_check<Viewer>(L, 1);

    // Get the new type
    int type = luaL_checkinteger(L, 2);

    // Create an holder for the new viewer
    Viewer* new_viewer = nullptr;

    // Create an instance of the right type
    switch (type)
    {
        case Viewer::kViewer:
            new_viewer = new Viewer();
            break;
        case Viewer::kStreamer:
            new_viewer = new Streamer();
            break;
        case Viewer::kModerator:
            new_viewer = new Moderator();
            break;
        default:
            break;
    }

    // Result holder
    bool ok = false;

    // If we have a new viewer
    if (new_viewer != nullptr)
    {
        // Load data from the old viewer
        new_viewer->from_viewer(viewer);

        // Change the new viewer type
        new_viewer->set_type(type);

        // Replace the old viewer
        ok = the_viewers.replace(viewer->twitch_username(), new_viewer);

        // If the viewer was replaced
        if (ok)
            // Push the new viewer
            luaW_push<Viewer>(L, new_viewer);
        // Else
        else
            // Push nil
            lua_pushnil(L);
    }
    // Else
    else
        // Push nil
        lua_pushnil(L);

    // One result
    return 1;
}

// Twitch username getter
static int lua_viewer_twitch_username(lua_State* L)
{
    // Get the viewer
    Viewer* viewer = luaW_check<Viewer>(L, 1);

    // Push the twitch username
    lua_pushstring(L, viewer->twitch_username().c_str());

    // One result
    return 1;
}

// Twitch username setter
static int lua_viewer_set_twitch_username(lua_State* L)
{
    // Get the viewer
    Viewer* viewer = luaW_check<Viewer>(L, 1);

    // Get the twitch username
    std::string twitch_username = luaL_checkstring(L, 2);

    // Set the twitch username
    viewer->set_twitch_username(twitch_username);

    // No result
    return 0;
}

// Online time getter
static int lua_viewer_online_time(lua_State* L)
{
    // Get the viewer
    Viewer* viewer = luaW_check<Viewer>(L, 1);

    // Push the online time
    lua_pushinteger(L, viewer->online_time());

    // One result
    return 1;
}

// Online time setter
static int lua_viewer_set_online_time(lua_State* L)
{
    // Get the viewer
    Viewer* viewer = luaW_check<Viewer>(L, 1);

    // Get the online time
    int online_time = luaL_checkinteger(L, 2);

    // Set the online time
    viewer->set_online_time(online_time);

    // No result
    return 0;
}

// Messages sent getter
static int lua_viewer_messages_sent(lua_State* L)
{
    // Get the viewer
    Viewer* viewer = luaW_check<Viewer>(L, 1);

    // Push the messages sent
    lua_pushinteger(L, viewer->messages_sent());

    // One result
    return 1;
}

// Messages sent setter
static int lua_viewer_set_messages_sent(lua_State* L)
{
    // Get the viewer
    Viewer* viewer = luaW_check<Viewer>(L, 1);

    // Get the messages sent
    int messages_sent = luaL_checkinteger(L, 2);

    // Set the messages sent
    viewer->set_messages_sent(messages_sent);

    // No result
    return 0;
}

////////////////////////////////////////////////////
//
// Streamer getters/setters
//
////////////////////////////////////////////////////

// osu! username getter
static int lua_streamer_osu_username(lua_State* L)
{
    // Get the viewer
    Viewer* viewer = luaW_check<Viewer>(L, 1);

    try
    {
        // Cast it as a streamer
        Streamer* streamer = dynamic_cast<Streamer*>(viewer);

        // If we have one
        if (streamer != nullptr)
            // Push the streamer osu! username
            lua_pushstring(L, streamer->osu_username().c_str());
        // Else
        else
            // Push nil
            lua_pushnil(L);
    }
    // On bad cast
    catch (std::bad_cast e)
    {
        // Print an error
        Meow("lua")->error("Error: call to set_osu_skin on non-streamer object");
    }

    return 1;
}

// osu! username setter
static int lua_streamer_set_osu_username(lua_State* L)
{
    // Get the viewer
    Viewer* viewer = luaW_check<Viewer>(L, 1);

    try
    {
        // Cast it as a streamer
        Streamer* streamer = dynamic_cast<Streamer*>(viewer);

        // If we have one
        if (streamer != nullptr)
        {
            // Get the osu! username
            std::string osu_username = luaL_checkstring(L, 2);

            // Set the osu! username
            streamer->set_osu_username(osu_username);
        }
    }
    // On bad cast
    catch (std::bad_cast e)
    {
        // Print an error
        Meow("lua")->error("Error: call to set_osu_skin on non-streamer object");
    }

    return 0;
}

// osu! skin link getter
static int lua_streamer_osu_skin_link(lua_State* L)
{
    // Get the viewer
    Viewer* viewer = luaW_check<Viewer>(L, 1);

    try
    {
        // Cast it as a streamer
        Streamer* streamer = dynamic_cast<Streamer*>(viewer);

        // If we have one
        if (streamer != nullptr)
            // Push the osu! skin link
            lua_pushstring(L, streamer->osu_skin_link().c_str());
        // Else
        else
            // Push nil
            lua_pushnil(L);
    }
    // On bad cast
    catch (std::bad_cast e)
    {
        // Print an error
        Meow("lua")->error("Error: call to osu_skin on non-streamer object");

        // Push null
        lua_pushstring(L, "null");
    }

    return 1;
}

// osu! skin link setter
static int lua_streamer_set_osu_skin_link(lua_State* L)
{
    // Get the viewer
    Viewer* viewer = luaW_check<Viewer>(L, 1);

    try
    {
        // Cast it as a streamer
        Streamer* streamer = dynamic_cast<Streamer*>(viewer);

        // If we have one
        if (streamer != nullptr)
        {
            // Get the osu! skin link
            std::string osu_skin_link = luaL_checkstring(L, 2);

            // Change his osu! skin link
            streamer->set_osu_skin_link(osu_skin_link);
        }
    }
    // On bad cast
    catch (std::bad_cast e)
    {
        // Print an error
        Meow("lua")->error("Error: call to set_osu_skin on non-streamer object");
    }

    // No result
    return 0;
}

// Stream time getter
static int lua_streamer_stream_time(lua_State* L)
{
    // Get the viewer
    Viewer* viewer = luaW_check<Viewer>(L, 1);

    try
    {
        // Cast it as a streamer
        Streamer* streamer = dynamic_cast<Streamer*>(viewer);

        // If we have one
        if (streamer != nullptr)
            // Push the stream time
            lua_pushinteger(L, streamer->stream_time());
        // Else
        else
            // Push nil
            lua_pushnil(L);
    }
    // On bad cast
    catch (std::bad_cast e)
    {
        // Print an error
        Meow("lua")->error("Error: call to osu_skin on non-streamer object");

        // Push null
        lua_pushstring(L, "null");
    }

    return 1;
}

// Stream time setter
static int lua_streamer_set_stream_time(lua_State* L)
{
    // Get the viewer
    Viewer* viewer = luaW_check<Viewer>(L, 1);

    try
    {
        // Cast it as a streamer
        Streamer* streamer = dynamic_cast<Streamer*>(viewer);

        // If we have one
        if (streamer != nullptr)
        {
            // Get the stream time
            int stream_time = luaL_checkinteger(L, 2);

            // Change his stream time
            streamer->set_stream_time(stream_time);
        }
    }
    // On bad cast
    catch (std::bad_cast e)
    {
        // Print an error
        Meow("lua")->error("Error: call to set_osu_skin on non-streamer object");
    }

    // No result
    return 0;
}

////////////////////////////////////////////////////
//
// Moderators getters/setters
//
////////////////////////////////////////////////////

// Call to the Moderator::privileges method
static int lua_moderator_privileges(lua_State* L)
{
    // Get the viewer
    Viewer* viewer = luaW_check<Viewer>(L, 1);

    try
    {
        // Cast it as a moderator
        Moderator* moderator = dynamic_cast<Moderator*>(viewer);

        // If we have one
        if (moderator != nullptr)
            // Push the privileges on the stack
            lua_pushinteger(L, moderator->privileges());
        // Else
        else
            // Push nil
            lua_pushnil(L);
    }
    // On bad cast
    catch (std::bad_cast e)
    {
        // Print an error
        Meow("lua")->error("Error: call to set_privileges on non-moderator object");

        // Push 0 on the stack
        lua_pushinteger(L, 0);
    }

    return 1;
}

// Call to the Moderator::set_privileges method
static int lua_moderator_set_privileges(lua_State* L)
{
    // Get the viewer
    Viewer* viewer = luaW_check<Viewer>(L, 1);

    try
    {
        // Cast it as a moderator
        Moderator* moderator = dynamic_cast<Moderator*>(viewer);

        // If we have one
        if (moderator != nullptr)
        {
            // Get the privileges
            int privileges = luaL_checkinteger(L, 2);

            // Change his privileges
            moderator->set_privileges(privileges);
        }
    }
    // On bad cast
    catch (std::bad_cast e)
    {
        // Print an error
        Meow("lua")->error("Error: call to set_privileges on non-moderator object");
    }

    return 0;
}

////////////////////////////////////////////////////
//
// Insertion/Deletion
//
////////////////////////////////////////////////////

// Call to the Viewer::insert method
static int lua_viewer_insert(lua_State* L)
{
    // Get the viewer
    Viewer* viewer = luaW_check<Viewer>(L, 1);

    // Insert him
    bool result = viewer->insert();

    // Push the result
    lua_pushboolean(L, result ? 1 : 0);

    // One result
    return 1;
}

// Call to the Viewer::erase method
static int lua_viewer_erase(lua_State* L)
{
    // Get the viewer
    Viewer* viewer = luaW_check<Viewer>(L, 1);

    // Insert him
    bool result = viewer->erase();

    // Push the result
    lua_pushboolean(L, result ? 1 : 0);

    // One result
    return 1;
}

////////////////////////////////////////////////////
//
// Miscellaneous
//
////////////////////////////////////////////////////

// Compares a viewer to a twitch username
static int lua_viewer_is(lua_State* L)
{
    // Get the viewer
    Viewer* viewer = luaW_check<Viewer>(L, 1);

    // Get the twitch username
    std::string twitch_username = luaL_checkstring(L, 2);

    // Push the result
    lua_pushboolean(L, (Utils::to_lower(viewer->twitch_username()) == Utils::to_lower(twitch_username)) ? 1 : 0);

    // One result
    return 1;
}

// Metatable
static const luaL_Reg lua_viewer_meta[] =
{
    // Id getter
    { "getId", lua_viewer_id },

    // Type getter
    { "getType", lua_viewer_type },

    // Type setter
    { "setType", lua_viewer_set_type },

    // Twitch username getter
    { "getTwitchUsername", lua_viewer_twitch_username },

    // Twitch username setter
    { "setTwitchUsername", lua_viewer_set_twitch_username },

    // Online time getter
    { "getOnlineTime", lua_viewer_online_time },

    // Online time setter
    { "setOnlineTime", lua_viewer_set_online_time },

    // Messages sent getter
    { "getMessagesSent", lua_viewer_messages_sent },

    // Messages sent setter
    { "setMessagesSent", lua_viewer_set_messages_sent },

    // osu! username getter
    { "getOsuUsername", lua_streamer_osu_username },

    // osu! username setter
    { "setOsuUsername", lua_streamer_set_osu_username },

    // osu! skin link getter
    { "getOsuSkinLink", lua_streamer_osu_skin_link },

    // osu! skin link setter
    { "setOsuSkinLink", lua_streamer_set_osu_skin_link },

    // Stream time getter
    { "getStreamTime", lua_streamer_stream_time },

    // Stream time setter
    { "setStreamTime", lua_streamer_set_stream_time },

    // Privileges getter
    { "getPrivileges", lua_moderator_privileges },

    // Privileges setter
    { "setPrivileges", lua_moderator_set_privileges },

    // Insert
    { "insert", lua_viewer_insert },

    // Erase
    { "erase", lua_viewer_erase },

    // Is
    { "is", lua_viewer_is },

    // Free
    { "free", lua_viewer_free },

    // Sentinel
    { 0, 0 }
};

// Static metatable
static const luaL_Reg lua_viewer_static_meta[] =
{
    // New viewer
    { "newViewer", lua_viewer_static_new_viewer },

    // New streamer
    { "newStreamer", lua_viewer_static_new_streamer },

    // New moderator
    { "newModerator", lua_viewer_static_new_moderator },

    // Exists
    { "exists", lua_viewer_static_exists },

    // Get
    { "get", lua_viewer_static_get },

    // Sentinel
    { 0, 0 }
};

// Register the viewer library
int luaopen_Viewer(lua_State* L)
{
    // Register viewer constants
    lua_pushinteger(L, Viewer::kViewer);
    lua_setglobal(L, "kViewer");
    lua_pushinteger(L, Viewer::kStreamer);
    lua_setglobal(L, "kStreamer");
    lua_pushinteger(L, Viewer::kModerator);
    lua_setglobal(L, "kModerator");

    luaW_register<Viewer>(L, "Viewer", lua_viewer_static_meta, lua_viewer_meta);
}
