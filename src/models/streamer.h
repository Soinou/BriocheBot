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

#ifndef MODELS_STREAMERS_H_
#define MODELS_STREAMERS_H_

#include "models/viewer.h"

// Represents a streamer. Basically a viewer with an osu! username and a skin
class Streamer : public Viewer
{
protected:
    // The osu! username
    std::string osu_username_;

    // The osu! skin link
    std::string osu_skin_link_;

    // The stream time
    Json::LargestUInt stream_time_;

public:
    // Constructor
    Streamer();

    // Destructor
    ~Streamer();

    // osu! username getter
    inline const std::string& osu_username() const
    {
        return osu_username_;
    }

    // osu! username setter
    inline void set_osu_username(const std::string osu_username)
    {
        osu_username_ = osu_username;
    }

    // osu! skin link getter
    inline const std::string& osu_skin_link() const
    {
        return osu_skin_link_;
    }

    // osu! skin link setter
    inline void set_osu_skin_link(const std::string& osu_skin_link)
    {
        osu_skin_link_ = osu_skin_link;
    }

    // Stream time getter
    inline Json::LargestUInt stream_time()
    {
        return stream_time_;
    }

    // Stream time setter
    inline void set_stream_time(Json::LargestUInt stream_time)
    {
        stream_time_ = stream_time;
    }

    // Converts the viewer to a json object
    virtual Json::Value to_json() const;

    // Creates the viewer from a json object
    virtual void from_json(const Json::Value& json);

    // Creates the viewer from a viewer object
    virtual void from_viewer(Viewer* viewer);
};

#endif // MODELS_STREAMERS_H_
