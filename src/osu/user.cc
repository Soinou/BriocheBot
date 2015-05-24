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

#include "osu/user.h"

#include <cstdlib>

namespace Osu
{
    User& User::operator=(const Json::Value& json)
    {
        // Initialize everything
        user_id = atol(json["user_id"].asString().c_str());
        username = json["username"].asString();
        level = atof(json["level"].asString().c_str());
        playcount = atol(json["playcount"].asString().c_str());
        ranked_score = atol(json["ranked_score"].asString().c_str());
        total_score = atol(json["total_score"].asString().c_str());
        pp_rank = atol(json["pp_rank"].asString().c_str());
        pp_raw = atof(json["pp_raw"].asString().c_str());
        accuracy = atof(json["accuracy"].asString().c_str());
        country = json["country"].asString();
        count_rank_ss = atol(json["count_rank_ss"].asString().c_str());
        count_rank_s = atol(json["count_rank_s"].asString().c_str());
        count_rank_a = atol(json["count_rank_a"].asString().c_str());

        return *this;
    }
}