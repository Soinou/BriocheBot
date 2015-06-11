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

#include "models/moderator.h"

#include <typeinfo>

// Define json keys
#define PRIVILEGES_KEY "privileges"

Moderator::Moderator() : Streamer(), privileges_(0)
{
    type_ = Viewer::kModerator;
}

Moderator::~Moderator()
{

}

Json::Value Moderator::to_json() const
{
    // Call the streamer to_json
    Json::Value root = Streamer::to_json();

    // Append data to the json we got
    root[PRIVILEGES_KEY] = privileges_;

    // Return the json value
    return root;
}

void Moderator::from_json(const Json::Value& json)
{
    // Call streamer from_json
    Streamer::from_json(json);

    // Get data from the json
    privileges_ = json.get(PRIVILEGES_KEY, 0).asUInt();
}

void Moderator::from_viewer(Viewer* viewer)
{
    // Call streamer from_viewer
    Streamer::from_viewer(viewer);

    try
    {
        // Cast the viewer as a moderator
        Moderator* moderator = dynamic_cast<Moderator*>(viewer);

        // If the viewer is a correct moderator
        if (moderator != nullptr)
            // Copy data from the old moderator
            privileges_ = moderator->privileges_;
    }
    // Ignore on bad cast
    catch (std::bad_cast e)
    {

    }
}
