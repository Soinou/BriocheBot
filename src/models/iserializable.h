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

#ifndef MODELS_ISERIALIZABLE_H_
#define MODELS_ISERIALIZABLE_H_

#include <json/json.h>

class ISerializable
{
public:
    // Constructor
    ISerializable();

    // Destructor
    ~ISerializable();

    // Get the hash key of the serializable
    virtual std::tuple<std::string, std::string> get_hash() const = 0;

    // Converts the serializable object to a json object
    virtual Json::Value to_json() const = 0;

    // Initializes the object from a json object
    virtual void from_json(const Json::Value& json) = 0;

    // Overload of the operator =, calling from_json
    inline void operator=(const Json::Value& json)
    {
        from_json(json);
    }
};

#endif // MODELS_ISERIALIZABLE_H_
