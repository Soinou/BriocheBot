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

#ifndef MODELS_VIEWERS_H_
#define MODELS_VIEWERS_H_

#include "utils/redis.h"
#include "utils/non_copyable.h"

#include <json/json.h>

#include <map>

// Forward declaration of the viewer class
class Viewer;

// Represents the viewers database
class Viewers : public NonCopyable
{
    // Declare viewer as a friend
    friend class Viewer;

private:
    // Redis connection
    Redis::Connection connection_;

    // The list of viewers
    std::map<std::string, Viewer*> viewers_;

    // The current max id
    int max_id_;

    // The json writer we're using
    Json::StreamWriterBuilder json_writer_;

    // Loads a viewer from a key and a json
    void load_viewer(const std::string& json);

    // Loads a set of viewer from a key
    void load_viewers(const std::string& key);

    // Updates a viewer
    bool update(Viewer* viewer);

    // Constructor
    Viewers();

public:
    // Destructor
    ~Viewers();

    // Loads the viewer list from the redis connection
    void load();

    // Actual max id we have
    inline int max_id() const
    {
        return max_id_;
    }

    // Checks if a viewer with the given username exists
    bool exists(const std::string& twitch_username);

    // Inserts a new viewer to the database
    bool insert(Viewer* viewer);

    // Erases a viewer from the database
    bool erase(const std::string& twitch_username);

    // Replace a viewer by a new object (For example when changing a viewer type)
    bool replace(const std::string& twitch_username, Viewer* viewer);

    // Find a viewer by its twitch username
    Viewer* get(const std::string& twitch_username);

    // Overload of operator= (Inline call to get)
    inline Viewer* operator[](const std::string& twitch_username)
    {
        return get(twitch_username);
    }

    // Singleton instance getter
    inline static Viewers& instance()
    {
        static Viewers instance;

        return instance;
    }
};

#define the_viewers Viewers::instance()

#endif // MODELS_VIEWERS_H_
