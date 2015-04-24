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

#ifndef OSU_USER_H_
#define OSU_USER_H_

#include <json/json.h>

#include <string>

// Osu namespace
namespace Osu
{
    // Represents an osu! user
    struct User
    {
        // User id
        long user_id;

        // Username
        std::string username;

        // Level
        float level;

        // Playcount
        long playcount;

        // Ranked score
        long ranked_score;

        // Total score
        long total_score;

        // PP rank
        float pp_rank;

        // Raw pp
        float pp_raw;

        // Accuracy
        float accuracy;

        // Country
        std::string country;

        // Number of ss ranks
        long count_rank_ss;

        // Number of s ranks
        long count_rank_s;

        // Number of a ranks
        long count_rank_a;

        // Copy operator
        User& operator=(const Json::Value& json);
    };
}

#endif // OSU_USER_H_
