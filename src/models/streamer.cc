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

#include "models/streamer.h"

#include <typeinfo>

// Define json keys
#define OSU_USERNAME_KEY "osu_username"
#define OSU_SKIN_LINK_KEY "osu_skin_link"
#define STREAM_TIME_KEY "stream_time"

Streamer::Streamer() : Viewer(), osu_username_(""), osu_skin_link_(""), stream_time_(0)
{
    type_ = Viewer::kStreamer;
}

Streamer::~Streamer()
{

}

Json::Value Streamer::to_json() const
{
    // Call the viewer to_json
    Json::Value root = Viewer::to_json();

    // Append data to the json we got
    root[OSU_USERNAME_KEY] = osu_username_;
    root[OSU_SKIN_LINK_KEY] = osu_skin_link_;
    root[STREAM_TIME_KEY] = stream_time_;

    // Return the json value
    return root;
}

void Streamer::from_json(const Json::Value& json)
{
    // Call viewer from_json
    Viewer::from_json(json);

    // Get data from the json
    osu_username_ = json.get(OSU_USERNAME_KEY, "null").asString();
    osu_skin_link_ = json.get(OSU_SKIN_LINK_KEY, "null").asString();
    stream_time_ = json.get(STREAM_TIME_KEY, 0).asLargestUInt();
}

void Streamer::from_viewer(Viewer* viewer)
{
    // Call viewer from_viewer
    Viewer::from_viewer(viewer);

    try
    {
        // Cast the viewer as a streamer
        Streamer* streamer = dynamic_cast<Streamer*>(viewer);

        // If the viewer is a correct streamer
        if (streamer != nullptr)
        {
            // Copy data from the old streamer
            osu_username_ = streamer->osu_username_;
            osu_skin_link_ = streamer->osu_skin_link_;
            stream_time_ = streamer->stream_time_;
        }
    }
    // Ignore on bad cast
    catch (std::bad_cast e)
    {

    }
}
