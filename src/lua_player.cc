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

#include "lua_player.h"

#include "lua_wrapper.h"
#include "lua_wrapper_utils.h"
#include "lua_wrapper_types.h"

#include "players.h"
#include "player.h"

// Call to the PlayersDb.exists method
static int lua_player_static_exists(lua_State* L)
{
    // Get the twitch username as an argument
    std::string twitch_username(std::string(luaL_checkstring(L, 1)));

    // Check if the player exists
    bool exists = PlayersDb.exists(twitch_username);

    // Push the result
    lua_pushboolean(L, exists ? 1 : 0);

    // One result
    return 1;
}

// Call to the PlayersDb.get method
static int lua_player_static_get(lua_State* L)
{
    // Get the twitch username as an argument
    std::string twitch_username(std::string(luaL_checkstring(L, 1)));

    // Get the player
    Player* player = PlayersDb.get(twitch_username);

    // If the player is not a dummy
    if (player)
        // Push the player
        luaW_push<Player>(L, player);
    // Else
    else
        // Push nil
        lua_pushnil(L);

    // One result
    return 1;
}

// Call to the PlayersDb.add method
static int lua_player_static_add(lua_State* L)
{
    // Get the twitch username
    std::string twitch_username(std::string(luaL_checkstring(L, 1)));

    // Get the osu! username
    std::string osu_username(std::string(luaL_checkstring(L, 1)));

    // Get the result of the add operation
    bool result = PlayersDb.add(twitch_username, osu_username, "null", false);

    // Push the result
    lua_pushboolean(L, result ? 1 : 0);

    // One result
    return 1;
}

// Call to the PlayersDb.remove method
static int lua_player_static_remove(lua_State* L)
{
    // Get the twitch username
    std::string twitch_username(std::string(luaL_checkstring(L, 1)));

    // Execute the remove operation and get the result
    bool result = PlayersDb.remove(twitch_username);

    // Push the result
    lua_pushboolean(L, result ? 1 : 0);

    // One result
    return 1;
}

// Call to the PlayersDb.update method
static int lua_player_set_osu_skin(lua_State* L)
{
    // Get the player
    Player* player = luaW_check<Player>(L, 1);

    // Get the new skin
    std::string osu_skin(std::string(luaL_checkstring(L, 2)));

    // Change the player skin
    player->set_osu_skin(osu_skin);

    // Execute the update operation and return the result
    bool result = PlayersDb.update(player);

    // Push the result
    lua_pushboolean(L, result ? 1 : 0);

    // One result
    return 1;
}

// Call to the PlayersDb.update method
static int lua_player_set_admin(lua_State* L)
{
    // Get the player
    Player* player = luaW_check<Player>(L, 1);

    // Check the new admin flag
    luaL_checktype(L, 2, LUA_TBOOLEAN);

    // Get the new admin flag
    bool admin = lua_toboolean(L, 2) == 1;

    // Change the player admin flag
    player->set_admin(admin);

    // Execute the operation and get the result
    bool result = PlayersDb.update(player);

    // Push the result
    lua_pushboolean(L, result ? 1 : 0);

    // One result
    return 1;
}

// Call to the Player.is method
static int lua_player_is(lua_State* L)
{
    // Get the player
    Player* player = luaW_check<Player>(L, 1);

    // Get the twitch username
    std::string twitch_username(luaL_checkstring(L, 2));

    // Push the result of the is method
    lua_pushboolean(L, player->is(twitch_username) ? 1 : 0);

    // One result
    return 1;
}

// Metatable
static const luaL_reg lua_player_meta[] =
{
    // Twitch username getter
    { "getTwitchUsername", luaU_get < Player, std::string, &Player::twitch_username > },

    // osu! username getter
    { "getOsuUsername", luaU_get < Player, std::string, &Player::osu_username > },

    // Skin getter
    { "getOsuSkin", luaU_get < Player, std::string, &Player::osu_skin > },

    // Skin setter
    { "setOsuSkin", lua_player_set_osu_skin },

    // Admin getter
    { "getAdmin", luaU_get < Player, bool, &Player::admin > },

    // Admin setter
    { "setAdmin", lua_player_set_admin },

    // Is
    { "is", lua_player_is },

    // Sentinel
    { 0, 0 }
};

// Static metatable
static const luaL_reg lua_player_static_meta[] =
{
    // Exists
    { "exists", lua_player_static_exists },

    // Get
    { "get", lua_player_static_get },

    // Add
    { "add", lua_player_static_add },

    // Remove
    { "remove", lua_player_static_remove },

    // Sentinel
    { 0, 0 }
};

// Register the player library
int luaopen_Player(lua_State* L)
{
    luaW_register<Player>(L, "Player", lua_player_static_meta, lua_player_meta);
}
