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

#ifndef MODELS_MODERATOR_H_
#define MODELS_MODERATOR_H_

#include "models/streamer.h"

// Represents a moderator, a streamer with privileges
class Moderator : public Streamer
{
protected:
    // The privileges of the moderator
    Json::UInt privileges_;

public:
    // Constructor
    Moderator();

    // Destructor
    ~Moderator();

    // Privileges getter
    inline Json::UInt privileges() const
    {
        return privileges_;
    }

    // Privileges setter
    inline void set_privileges(Json::UInt privileges)
    {
        privileges_ = privileges;
    }

    // Converts the viewer to a json object
    virtual Json::Value to_json() const;

    // Creates the viewer from a json object
    virtual void from_json(const Json::Value& json);

    // Creates the viewer from a viewer object
    virtual void from_viewer(Viewer* viewer);
};

#endif // MODELS_MODERATOR_H_
