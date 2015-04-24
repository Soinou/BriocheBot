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

#ifndef PLAYER_H_
#define PLAYER_H_

#include <memory>
#include <string>

// Represents a player
class Player
{
private:
    // Twitch username
    std::string twitch_username_;

    // Osu username
    std::string osu_username_;

    // Osu skin
    std::string osu_skin_;

    // If the user is an admin
    bool admin_;

public:
    // Ptr typedef
    typedef std::shared_ptr<Player> Ptr;

    // Constructor
    Player();

    // Destructor
    ~Player();

    // Twitch username getter
    inline const std::string& twitch_username() const
    {
        return twitch_username_;
    }

    // Twitch username setter
    inline void set_twitch_username(const std::string& twitch_username)
    {
        twitch_username_ = twitch_username;
    }

    // osu! username getter
    inline const std::string& osu_username() const
    {
        return osu_username_;
    }

    // osu! username setter
    inline void set_osu_username(const std::string& osu_username)
    {
        osu_username_ = osu_username;
    }

    // osu! skin getter
    inline const std::string& osu_skin() const
    {
        return osu_skin_;
    }

    // osu! skin setter
    inline void set_osu_skin(const std::string osu_skin)
    {
        osu_skin_ = osu_skin;
    }

    // Admin getter
    inline bool admin() const
    {
        return admin_;
    }

    // Admin setter
    inline void set_admin(bool admin)
    {
        admin_ = admin;
    }

    // Checks if the player is equals to the given username
    inline bool is(const std::string& twitch_username)
    {
        // Checking if both hash are equals will do the trick
        return get_hash() == Player::get_hash(twitch_username);
    }

    // Create an unique hash from the player's twitch username
    int get_hash() const;

    // Transforms the player to a compressed json string
    std::string to_json() const;

    // Get a hash from a twitch username only
    static int get_hash(const std::string& twitch_username);

    // Creates a new player from a compressed json string
    static Player* from_json(const std::string& json);
};

#endif // PLAYER_H_
