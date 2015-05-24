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

#include "osu/api.h"

#include "utils/curl.h"
#include "utils/utils.h"

#include <json/json.h>

// Users url
#define USERS_URL "http://osu.ppy.sh/api/get_user"

// Beatmaps url
#define BEATMAPS_URL "http://osu.ppy.sh/api/get_beatmaps"

namespace Osu
{
    // Url to get an user from his username
    static std::string get_user_url(const std::string& key, const std::string& username, int mode)
    {
        return Utils::string_format("%s?k=%s&u=%s&m=%d&type=string", BEATMAPS_URL, key.c_str(), username.c_str(), mode);
    }

    // Url to get an user from his id
    static std::string get_user_url(const std::string& key, long user_id, int mode)
    {
        return Utils::string_format("%s?k=%s&u=%ld&m=%d&type=id", BEATMAPS_URL, key.c_str(), user_id, mode);
    }

    static std::string get_beatmap_url(const std::string& key, long beatmap_id, int mode)
    {
        if (mode != -1)
            return Utils::string_format("%s?k=%s&b=%ld&m=%d", BEATMAPS_URL, key.c_str(), beatmap_id, mode);
        else
            return Utils::string_format("%s?k=%s&b=%ld", BEATMAPS_URL, key.c_str(), beatmap_id);
    }

    static std::string get_beatmap_set_url(const std::string& key, long beatmap_set_id, int mode)
    {
        if (mode != -1)
            return Utils::string_format("%s?k=%s&s=%ld&m=%s", BEATMAPS_URL, key.c_str(), beatmap_set_id, mode);
        else
            return Utils::string_format("%s?k=%s&s=%ld", BEATMAPS_URL, key.c_str(), beatmap_set_id);
    }

    Api::Api() : api_key_()
    {

    }

    Api::~Api()
    {

    }

    User Api::get_user(const std::string& username, int mode)
    {
        // The user we will return
        User user;

        // Get the response from the server
        std::string json = Curly.perform_get(get_user_url(api_key_, username, mode));

        // If the json string is empty (No response, or bad response)
        if (json.empty())
            // User id is -1 (Not found)
            user.user_id = -1;
        // Else (We have a response)
        else
        {
            // Create a json reader
            Json::Reader reader;

            // Create a new document
            Json::Value users;

            // Parse it
            reader.parse(json, users);

            // If we have an array and the size is exactly 1
            if (users.isArray() && users.size() == 1)
                // Copy the json value to the user
                user = users[0];
            // Else
            else
                // User id of -1, not found
                user.user_id = -1;
        }

        // Return the user
        return user;
    }

    std::vector<Beatmap> Api::get_beatmap_set(long beatmap_set_id, int mode)
    {
        // The beatmap
        std::vector<Beatmap> beatmap_set;

        // Get the response from the server
        std::string json = Curly.perform_get(get_beatmap_set_url(api_key_, beatmap_set_id, mode));

        // If we have a response
        if (!json.empty())
        {
            // Create a json reader
            Json::Reader reader;

            // Create a new document
            Json::Value beatmaps;

            // Parse it
            reader.parse(json, beatmaps);

            // If we have a valid array
            if (beatmaps.isArray())
            {
                // For each element in the array
                for (auto i = beatmaps.begin(); i != beatmaps.end(); i++)
                {
                    // Create a new beatmap
                    Beatmap beatmap;

                    // Copy the json element to our beatmap
                    beatmap = *i;

                    // Push this beatmap in our set
                    beatmap_set.push_back(beatmap);
                }
            }
        }

        // Return the beatmap set
        return beatmap_set;
    }

    Beatmap Api::get_beatmap(long beatmap_id, int mode)
    {
        // The beatmap
        Beatmap beatmap;

        // Get the response from the server
        std::string json = Curly.perform_get(get_beatmap_url(api_key_, beatmap_id, mode));

        // If the json string is empty (No response, or bad response)
        if (json.empty())
        {
            // User id is -1 (Not found)
            beatmap.beatmap_id = -1;
        }
        // Else (We have a response)
        else
        {
            // Create a json reader
            Json::Reader reader;

            // Create a new document
            Json::Value beatmaps;

            // Parse it
            reader.parse(json, beatmaps);

            // If we have an array and the size is exactly 1
            if (beatmaps.isArray() && beatmaps.size() == 1)
                // Copy the json value to the beatmap
                beatmap = beatmaps[0];
            // Else
            else
                // Beatmap id of -1, not found
                beatmap.beatmap_id = -1;
        }

        // Return the beatmap
        return beatmap;
    }
}
