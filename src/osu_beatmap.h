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

#ifndef OSU_BEATMAP_H_
#define OSU_BEATMAP_H_

#include <json/json.h>

#include <string>

// Osu namespace
namespace Osu
{
    // Represents an osu! beatmap
    struct Beatmap
    {
        // Beatmap id
        long beatmap_id;

        // Beatmap set id
        long beatmap_set_id;

        // Played mode
        int mode;

        // Artist
        std::string artist;

        // Title
        std::string title;

        // Version (Difficulty)
        std::string version;

        // Creator (Mapper)
        std::string creator;

        // Source
        std::string source;

        // Bpm
        float bpm;

        // SR
        float star_rating;

        // CS
        float circle_size;

        // OD
        float overall_difficulty;

        // AR
        float approach_rate;

        // HP
        float health_drain;

        // Length from first hit object to last hit object
        long hit_length;

        // Length of the mp3
        long total_length;

        // Approval status
        int approved;

        // Approved date (if any)
        std::string approved_date;

        // Last update
        std::string last_update;

        // Copy values from a json value
        Beatmap& operator=(const Json::Value& json);
    };
}

#endif // OSU_BEATMAP_H_
