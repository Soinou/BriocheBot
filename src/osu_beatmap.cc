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

#include "osu_beatmap.h"

#include <cstdlib>

namespace Osu
{
    Beatmap& Beatmap::operator=(const Json::Value& json)
    {
        // Get everything from the json value
        beatmap_id = atol(json["beatmap_id"].asString().c_str());
        beatmap_set_id = atol(json["beatmapset_id"].asString().c_str());
        mode = atoi(json["mode"].asString().c_str());
        artist = json["artist"].asString();
        title = json["title"].asString();
        version = json["version"].asString();
        creator = json["creator"].asString();
        source = json["source"].asString();
        bpm = atof(json["bpm"].asString().c_str());
        star_rating = atof(json["difficultyrating"].asString().c_str());
        circle_size = atof(json["diff_size"].asString().c_str());
        overall_difficulty = atof(json["diff_overall"].asString().c_str());
        approach_rate = atof(json["diff_approach"].asString().c_str());
        health_drain = atof(json["diff_drain"].asString().c_str());
        hit_length = atol(json["hit_length"].asString().c_str());
        total_length = atol(json["total_length"].asString().c_str());
        approved = atoi(json["approved"].asString().c_str());
        approved_date = json["approved_date"].asString();
        last_update = json["last_update"].asString();

        return *this;
    }
}
