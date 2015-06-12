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

#include "models/viewers.h"

#include "models/viewer.h"
#include "models/streamer.h"
#include "models/moderator.h"
#include "utils/utils.h"
#include "utils/logger.h"

Viewers::Viewers() : connection_("127.0.0.1", 6379), viewers_(), json_writer_(), max_id_(0)
{
    // Set json writer indentation
    json_writer_["indentation"] = "";
}

Viewers::~Viewers()
{
    // For each viewer in our map
    for (auto i = viewers_.begin(); i != viewers_.end(); i++)
        // Delete them
        delete i->second;
}

void Viewers::load_viewer(const std::string& json_string)
{
    // Create a json reader
    Json::Reader reader;

    // Create a json value
    Json::Value json;

    // Parse the json into the value we created
    reader.parse(json_string, json);

    // Create a new viewer (Nullptr for now)
    Viewer* viewer = nullptr;

    // Create the correct holder depending on the viewer type
    switch (json["type"].asUInt())
    {
        case Viewer::kViewer:
            viewer = new Viewer();
            break;
        case Viewer::kStreamer:
            viewer = new Streamer();
            break;
        case Viewer::kModerator:
            viewer = new Moderator();
            break;
        default:
            break;
    }

    // If we now have a viewer
    if (viewer)
    {
        // Initialize him with the json we have
        viewer->from_json(json);

        // Add this viewer to our map
        viewers_.insert(std::make_pair(Utils::to_lower(viewer->twitch_username()), viewer));

        // If the viewer id is greater than our current max id
        if (viewer->id() > max_id_)
            // Replace our max id by his id
            max_id_ = viewer->id();
    }
}

void Viewers::load_viewers(const std::string& key)
{
    // Execute the request to get all values of the given set
    Redis::Reply reply = connection_.execute(Utils::string_format("HVALS %s", key.c_str()));

    // If we have an error
    if (reply.type == Redis::Reply::Error)
        // Display it
        Meow("server")->error(Utils::string_format("Redis error when executing an hvals command: %s", reply.string.c_str()));

    // If we have an array
    if (reply.type == Redis::Reply::Array)
        // For each pair of elements (Just iterate two by two)
        for (auto i = reply.elements.begin(); i != reply.elements.end(); i++)
            // Load a new viewer
            load_viewer(i->string);
}

void Viewers::load()
{
    // Get all the viewers keys
    Redis::Reply reply = connection_.execute("KEYS viewers:*");

    // If we have an error
    if (reply.type == Redis::Reply::Error)
        // Display it
        Meow("server")->error(Utils::string_format("Redis error when executing an hkeys command: %s", reply.string.c_str()));

    // If we have an array
    if (reply.type == Redis::Reply::Array)
        // For each hashset in our array
        for (auto i = reply.elements.begin(); i != reply.elements.end(); i++)
            // Load the viewers in this hashset
            load_viewers(i->string);
}

bool Viewers::update(Viewer* viewer)
{
    // Get the hash of the viewer
    auto hash = viewer->get_hash();

    // Get the two parts of the key
    std::string left = std::get<0>(hash);
    std::string right = std::get<1>(hash);

    // Get the viewer's json
    std::string json = Json::writeString(json_writer_, viewer->to_json());

    // Format the redis command (HSET [left] [right] [json])
    std::string command = Utils::string_format("HSET %s %s %s", left.c_str(), right.c_str(), json.c_str());

    // Send this command to redis
    Redis::Reply reply = connection_.execute(command);

    // If we have an error
    if (reply.type == Redis::Reply::Error)
        // Display it
        Meow("server")->error(Utils::string_format("Redis error when executing an hset command: %s", reply.string.c_str()));

    // Return the command result
    return reply.type == Redis::Reply::Integer && (reply.integer == 1 || reply.integer == 0);
}

bool Viewers::exists(const std::string& twitch_username)
{
    // If we have at least one, that's good
    return viewers_.count(Utils::to_lower(twitch_username)) > 0;
}

bool Viewers::insert(Viewer* viewer)
{
    // Update the viewer
    bool ok = update(viewer);

    // He could be updated
    if (ok)
    {
        // Add him to our map
        viewers_.insert(std::make_pair(Utils::to_lower(viewer->twitch_username()), viewer));

        // If the viewer id is greater than our max id
        if (viewer->id() > max_id_)
            // Replace our max id with his id
            max_id_ = viewer->id();
    }

    // Return the result
    return ok;
}

bool Viewers::erase(const std::string& twitch_username)
{
    // Lower case the twitchusername
    std::string lower_twitch_username = Utils::to_lower(twitch_username);

    // Find the viewer
    auto i = viewers_.find(lower_twitch_username);

    // Viewer doesn't exist
    if (i == viewers_.end())
        // Nope
        return false;

    // Viewer exists, get him
    Viewer* viewer = i->second;

    // Get the viewer hash
    auto hash = viewer->get_hash();

    // Get the two parts of the key
    std::string left = std::get<0>(hash);
    std::string right = std::get<1>(hash);

    // Get the command to delete the viewer
    std::string command = Utils::string_format("HDEL %s %s", left.c_str(), right.c_str());

    // Send this command to redis
    Redis::Reply reply = connection_.execute(command);

    // If we have an error
    if (reply.type == Redis::Reply::Error)
        // Display it
        Meow("server")->error(Utils::string_format("Redis error when executing an hdel command: %s", reply.string.c_str()));

    // Check if the command is ok
    bool ok = reply.type == Redis::Reply::Integer && reply.integer == 1;

    // If the command is ok
    if (ok)
        // Remove the viewer from the map
        viewers_.erase(lower_twitch_username);

    // Return if the command is ok
    return ok;
}

bool Viewers::replace(const std::string& twitch_username, Viewer* viewer)
{
    // Find the previous viewer
    auto i = viewers_.find(Utils::to_lower(twitch_username));

    // If we have one
    if (i != viewers_.end())
    {
        // Delete him
        delete i->second;

        // Replace the old value by the new one
        i->second = viewer;

        // Then update the viewer with the new values
        return update(viewer);
    }

    // Not ok
    return false;
}

Viewer* Viewers::get(const std::string& twitch_username)
{
    // Find the viewer
    auto i = viewers_.find(Utils::to_lower(twitch_username));

    // Viewer doesn't exists
    if (i == viewers_.end())
        // Nope
        return nullptr;
    // Viewer exists
    else
        // Return him
        return i->second;
}