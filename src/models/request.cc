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

#include "models/request.h"

#include "osu/api.h"

#include <cstdlib>
#include <sstream>
#include <vector>
#include <regex>

// The request regex
static std::regex request_regex("https?://osu\\.ppy\\.sh/([bs])/([0-9]+)(&m=([0-9])+)?( *\\+ *(.+))?");

Request::Request(const std::string& author, const Osu::Beatmap& beatmap, const std::string& comment)
    : author_(author), beatmap_(beatmap), comment_(comment)
{

}

Request::~Request()
{

}

std::string Request::to_twitch_string() const
{
    // Create a stream
    std::stringstream stream;

    // Format: <author>: <artist> - <title> [<version>] + <comment>
    stream << author_ << ": ";
    stream << beatmap_.artist << " - " << beatmap_.title << " [" << beatmap_.version << "]";

    // If we have a comment
    if (!comment_.empty())
        // Add it
        stream << " + " << comment_;

    // Return the stream
    return stream.str();
}

std::string Request::to_osu_string() const
{
    // Create a stream
    std::stringstream stream;

    // Format: <author>: ["http://osu.ppy.sh/b/<beatmapid>" <artist> - <title> [<version>]] (AR: xx, CS: xx, HP: xx, OD: xx) + <comment>
    stream << author_ << ": ";
    stream << "[";
    stream << "http://osu.ppy.sh/b/";
    stream << beatmap_.beatmap_id;
    stream << " ";
    stream << beatmap_.artist << " - " << beatmap_.title << " [" << beatmap_.version << "]";
    stream << "]";
    stream << " (AR: " << beatmap_.approach_rate;
    stream << ", CS: " << beatmap_.circle_size;
    stream << ", HP: " << beatmap_.health_drain;
    stream << ", OD: " << beatmap_.overall_difficulty << ")";

    // If we have a comment
    if (!comment_.empty())
        // Add it
        stream << " + " << comment_;

    // Return the stream
    return stream.str();
}

Request* Request::parse(const std::string& sender, const std::string& message)
{
    // Request is by default a nullptr
    Request* request = nullptr;

    // The matches holder
    std::smatch matches;

    // If the regex matches against the message
    if (std::regex_match(message, matches, request_regex))
    {
        // Type is the first match
        std::string type = matches[1];

        // Beatmap id is the second match
        long id = atoi(matches[2].str().c_str());

        // Game mode default is 0
        int mode = 0;

        // If we don't have a mode specified
        if (matches[4].str().empty())
            // The mode is -1 (Auto)
            mode = -1;
        // Else
        else
            // Get the game mode
            mode = atoi(matches[4].str().c_str());

        // Get the comment
        std::string comment = matches[6].str();

        // Create the osu! beatmap
        Osu::Beatmap beatmap;

        // If the id type is b
        if (type == "b")
            // Get the osu! beatmap from the api
            beatmap = OsuApi.get_beatmap(id, mode);
        // Else, if the type is s
        else if (type == "s")
        {
            // Get the list of beatmaps in the set
            std::vector<Osu::Beatmap> beatmaps = OsuApi.get_beatmap_set(id, mode);

            // If we have at least one beatmap in the set
            if (beatmaps.size() >= 1)
                // Get the first
                beatmap = beatmaps[0];
        }

        // If the beatmap id is not -1 (Meaning the beatmap doesn't exist)
        if (beatmap.beatmap_id != -1)
            // Create a new request with the author, beatmap we got and the comment (Which can be empty)
            request = new Request(sender, beatmap, comment);
    }

    // Return the request (Can be a nullptr or a new request)
    return request;
}
