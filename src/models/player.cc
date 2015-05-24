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

#include "models/player.h"

#include "utils/utils.h"

#include <json/json.h>
#include <algorithm>
#include <string> 

Player::Player() : twitch_username_(""), osu_username_(""), osu_skin_(""), admin_(false)
{

}

Player::~Player()
{

}

int Player::get_hash() const
{
    return get_hash(twitch_username_);
}

std::string Player::to_json() const
{
    // Create a new Json object
    Json::Value root;

    // Put some informations in it
    root["twitch_username"] = twitch_username_;
    root["osu_username"] = osu_username_;
    root["osu_skin"] = osu_skin_;
    root["admin"] = admin_;

    // Create a Json writer
    Json::StreamWriterBuilder writer;

    // Set indetation to none
    writer["indentation"] = "";

    // Build a string from the Json object and return it
    return Json::writeString(writer, root);
}

int Player::get_hash(const std::string& twitch_username)
{


    // Make a static hash maker
    static std::hash<std::string> hash;

    // Copy the username in a local string
    std::string input(twitch_username);

    // Change the locale
    std::setlocale(LC_ALL, "en_US.UTF-8");

    // Transform the input to lower case
    std::transform(input.begin(), input.end(), input.begin(), ::tolower);

    // Trim the input
    Utils::trim(input);

    // Make a hash out of it
    return hash(input);
}

Player* Player::from_json(const std::string& json)
{
    // Create a Json reader
    Json::Reader reader;

    // Parse the value out of the Json string
    Json::Value root;

    // Parse the json value to the root
    reader.parse(json, root);

    // Create a new player
    Player* player = new Player();

    // Get the player values from the Json object
    player->set_twitch_username(root["twitch_username"].asString());
    player->set_osu_username(root["osu_username"].asString());
    player->set_osu_skin(root["osu_skin"].asString());
    player->set_admin(root["admin"].asBool());

    // Return the player
    return player;
}
