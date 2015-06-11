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

#include "models/viewer.h"

#include "models/viewers.h"
#include "utils/utils.h"

#include <algorithm>

// Define json keys
#define ID_KEY "id"
#define TWITCH_USERNAME_KEY "twitch_username"
#define TYPE_KEY "type"
#define ONLINE_TIME_KEY "online_time"
#define MESSAGES_SENT_KEY "messages_sent"

Viewer::Viewer() : id_(-1), type_(Viewer::kViewer), twitch_username_(""), online_time_(0), messages_sent_(0)
{
}

Viewer::~Viewer()
{
}

bool Viewer::insert()
{
    // If our id is not set yet
    if (id_ == -1)
        // Set him
        id_ = the_viewers.max_id() + 1;

    // If a viewer with our twitch username already exists
    if (the_viewers.exists(twitch_username_))
        // Update him
        return the_viewers.update(this);
    // Else
    else
        // Add him
        return the_viewers.insert(this);
}

bool Viewer::erase()
{
    // If we exist in the database
    if (the_viewers.exists(twitch_username_))
        // Erase ourselves
        return the_viewers.erase(twitch_username_);
    // Else
    else
        // Nope
        return false;
}

Json::Value Viewer::to_json() const
{
    // Create a new json value
    Json::Value root;

    // Store data in this json value
    root[ID_KEY] = id_;
    root[TWITCH_USERNAME_KEY] = twitch_username_;
    root[TYPE_KEY] = type_;
    root[ONLINE_TIME_KEY] = online_time_;
    root[MESSAGES_SENT_KEY] = messages_sent_;

    // Return the json value
    return root;
}

std::string Viewer::to_json_string() const
{
    Json::StreamWriterBuilder writer;

    writer["indentation"] = "";

    return Json::writeString(writer, to_json());
}

void Viewer::from_json(const Json::Value& json)
{
    // Get all the data from the json value
    id_ = json.get(ID_KEY, -1).asUInt();
    twitch_username_ = json.get(TWITCH_USERNAME_KEY, "null").asString();
    type_ = json.get(TYPE_KEY, Type::kViewer).asUInt();
    online_time_ = json.get(ONLINE_TIME_KEY, 0).asLargestUInt();
    messages_sent_ = json.get(MESSAGES_SENT_KEY, 0).asLargestUInt();
}

void Viewer::from_viewer(Viewer* viewer)
{
    // Get all the data from the viewer
    id_ = viewer->id_;
    twitch_username_ = viewer->twitch_username_;
    type_ = viewer->type_;
    online_time_ = viewer->online_time_;
    messages_sent_ = viewer->messages_sent_;
}

std::tuple<std::string, std::string> Viewer::get_hash() const
{
    // Left part of the key
    std::string left("viewers:");

    // Right part of the key
    std::string right("");

    // Make the hash
    std::string key(std::to_string(id_));

    // Pad the key with 0
    key.insert(key.begin(), 10 - key.length(), '0');

    // Add 5 chars to the left part of the key
    left += key.substr(0, 8);

    // Add 5 chars to the right part of the key
    right += key.substr(8, 2);

    // Return a tuple of the two key parts
    return std::make_tuple(left, right);
}