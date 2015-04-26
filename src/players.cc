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

#include "players.h"

#include "player.h"

#include <cstdlib>
#include <sstream>

// Returns the EXISTS command on the given username
static inline std::string player_exists(int hash)
{
    // Create a string stream
    std::stringstream stream;

    // Format: EXISTS <hash>
    stream << "EXISTS " << hash;

    // Return the command
    return stream.str();
}

// Returns the GET command on the given username
static inline std::string player_get(int hash)
{
    // Create a string stream
    std::stringstream stream;

    // Format: GET <hash>
    stream << "GET " << hash;

    // Return the command
    return stream.str();
}

// Returns the SET command with the given informations
static inline std::string player_update(Player* player)
{
    // Create a string stream
    std::stringstream stream;

    // Format: SET <hash> <json>
    stream << "SET " << player->get_hash();
    stream << " " << player->to_json();

    // Return the command
    return stream.str();
}

// Player add and player update are the same command
#define player_add(player) player_update(player)

// Returns the DEL command on the given username
static inline std::string player_remove(int hash)
{
    // Create a new stream
    std::stringstream stream;

    // Format: DEL <hash>
    stream << "DEL " << hash;

    // Return the command
    return stream.str();
}

Players::Players() : connection_("127.0.0.1", 6379), players_()
{
}

Players::~Players()
{
    // For each entry in our map
    for (auto i = players_.begin(); i != players_.end(); i++)
        // Delete the player
        delete i->second;
}

bool Players::exists(const std::string& twitch_username)
{
    // Get the hash of the player
    int hash = Player::get_hash(twitch_username);

    // If this player is currently cached
    if (players_.count(hash) > 0)
        // Then yes
        return true;

    // Execute the command
    Redis::Reply reply = connection_.execute(player_exists(hash).c_str());

    // If the reply is an integer and its value is 1, true, else false
    return reply.type == Redis::Reply::Integer && reply.integer == 1;
}

Player* Players::get(const std::string& twitch_username)
{
    // Get the player hash
    int hash = Player::get_hash(twitch_username);

    // If this player is cached
    if (players_.count(hash) > 0)
        // Directly return him
        return players_[hash];

    // Execute the command
    Redis::Reply reply = connection_.execute(player_get(hash).c_str());

    // If the reply is a string reply
    if (reply.type == Redis::Reply::String)
    {
        // Parse the reply and get a player from it
        Player* player = Player::from_json(reply.string);

        // If this player is not a dummy
        if (player)
            // Return him
            return players_.insert({ hash, player }).first->second;
        // Else
        else
            // Return nullptr
            return nullptr;
    }
    // Else
    else
        // Return nullptr
        return nullptr;
}

bool Players::update(Player* player)
{
    // Execute the command
    Redis::Reply reply = connection_.execute(player_update(player).c_str());

    // Get the result
    return reply.type == Redis::Reply::Integer && reply.integer == 0;
}

bool Players::add(const std::string& twitch_username, const std::string& osu_username, const std::string& osu_skin, bool admin)
{
    // Get the player hash
    int hash = Player::get_hash(twitch_username);

    // If this player is cached
    if (players_.count(hash) > 0)
        // Then we can't add him
        return false;

    // Create a new player
    Player* player = new Player();

    // Change the different values of the player
    player->set_twitch_username(twitch_username);
    player->set_osu_username(osu_username);
    player->set_osu_skin(osu_skin);
    player->set_admin(admin);

    // Execute the command
    Redis::Reply reply = connection_.execute(player_add(player).c_str());

    // Get the result and return it
    bool result = reply.type == Redis::Reply::Status && reply.string == "OK";

    // If we could add him to the database
    if (result)
        // Add him to the cache
        players_.insert({ hash, player });

    return result;
}

bool Players::remove(const std::string& twitch_username)
{
    // Get the player hash
    int hash = Player::get_hash(twitch_username);

    // Execute the command
    Redis::Reply reply = connection_.execute(player_remove(hash).c_str());

    // Get the result
    bool result = reply.type == Redis::Reply::Integer && reply.integer == 1;

    // If we removed him and this player was cached
    if (result && players_.count(hash) > 0)
    {
        // Delete the player we had
        delete players_[hash];

        // Then remove him from the cache
        players_.erase(hash);
    }

    // Return the result
    return result;
}
