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

#ifndef MODELS_VIEWER_H_
#define MODELS_VIEWER_H_

#include "models/iserializable.h"

// Represents a viewer
class Viewer : public ISerializable
{
public:
    // Represents the type of a viewer
    enum Type
    {
        kViewer = 0,
        kStreamer,
        kModerator
    };

protected:
    // The viewer id
    Json::UInt id_;

    // The twitch username of the viewer
    std::string twitch_username_;

    // The type of the viewer
    Json::UInt type_;

    // The amount of time the viewer spent online (In seconds)
    Json::LargestUInt online_time_;

    // The number of messages sent by the viewer
    Json::LargestUInt messages_sent_;

public:
    // Constructor
    Viewer();

    // Destructor
    ~Viewer();

    // Id getter
    inline int id() const
    {
        return id_;
    }

    // Id setter
    inline void set_id(int id)
    {
        id_ = id;
    }

    // Twitch username getter
    inline const std::string& twitch_username() const
    {
        return twitch_username_;
    }

    // Twitch username setter
    inline void set_twitch_username(const std::string& twitch_username)
    {
        twitch_username_ = twitch_username;
    }

    // Type getter
    inline Json::UInt type() const
    {
        return type_;
    }

    // Type setter
    inline void set_type(Json::UInt type)
    {
        type_ = type;
    }

    // Online time getter
    inline Json::LargestUInt online_time() const
    {
        return online_time_;
    }

    // Online time setter
    inline void set_online_time(Json::LargestUInt online_time)
    {
        online_time_ = online_time;
    }

    // Messages sent getter
    inline Json::LargestUInt messages_sent() const
    {
        return messages_sent_;
    }

    // Messages sent setter
    inline void set_messages_sent(Json::LargestUInt messages_sent)
    {
        messages_sent_ = messages_sent;
    }

    // Inserts the viewer to the viewers database
    bool insert();

    // Erases the viewer from the viewers database
    bool erase();

    // Converts the viewer to a json object
    virtual Json::Value to_json() const;

    // Converts the viewer to a json string
    std::string to_json_string() const;

    // Creates the viewer from a json object
    virtual void from_json(const Json::Value& json);

    // Creates the viewer from a viewer object
    virtual void from_viewer(Viewer* viewer);

    // Gets the hash key of the viewer
    std::tuple<std::string, std::string> get_hash() const;
};

#endif // MODELS_VIEWER_H_