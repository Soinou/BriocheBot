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

#ifndef REQUEST_H_
#define REQUEST_H_

#include "osu_beatmap.h"

#include <string>

// Utility to parse a request from a message
class Request
{
private:
    // The author of the request
    std::string author_;

    // The osu! beatmap
    Osu::Beatmap beatmap_;

    // The comment at the end of the request
    std::string comment_;

    // Constructor
    Request(const std::string& author, const Osu::Beatmap& beatmap, const std::string& comment);

public:
    // Destructor
    ~Request();

    // Author getter
    inline const std::string& author() const
    {
        return author_;
    }

    // Beatmap getter
    inline const Osu::Beatmap& beatmap() const
    {
        return beatmap_;
    }

    // Comment getter
    inline const std::string& comment() const
    {
        return comment_;
    }

    // Convert the request to a Twitch formatted string
    std::string to_twitch_string() const;

    // Convert the request to an osu! formatted string
    std::string to_osu_string() const;

    // Parses a request object from a message
    static Request* parse(const std::string& sender, const std::string& message);
};

#endif // REQUEST_H_
