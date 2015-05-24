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

#ifndef PLAYERS_H_
#define PLAYERS_H_

#include "utils/redis.h"

#include <map>
#include <string>

// Forward declaration of the Player class
class Player;

// Players database
class Players
{
private:
    // Redis connection
    Redis::Connection connection_;

    // The map of the last cached players
    std::map<int, Player*> players_;

    // Constructor
    Players();

    // Delete copy constructor
    Players(const Players&) = delete;

    // Delete copy operator
    void operator=(const Players&) = delete;

public:
    // Destructor
    ~Players();

    // Checks if a player exists
    bool exists(const std::string& twitch_username);

    // Get a player from its twitch username
    Player* get(const std::string& twitch_username);

    // Changes the skin of a player
    bool update(Player* player);

    // Adds a new player
    bool add(const std::string& twitch_username, const std::string& osu_username, const std::string& osu_skin, bool admin);

    // Removes a players
    bool remove(const std::string& twitch_username);

    // Singleton get instance
    static Players& get_instance()
    {
        static Players instance;

        return instance;
    }
};

// Players get instance shortcut
#define PlayersDb Players::get_instance()

#endif // PLAYERS_H_
