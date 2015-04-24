// ircbot2
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

#ifndef OSU_API_H_
#define OSU_API_H_

#include "osu_beatmap.h"
#include "osu_user.h"

#include <string>
#include <vector>

// Osu namespace
namespace Osu
{
    // The api wrapper (Performs all api operations)
    class Api
    {
    private:
        // Api key
        std::string api_key_;

        // Constructor
        Api();

        // Delete copy constructor
        Api(const Api&) = delete;

        // Delete copy operator
        void operator=(const Api&) = delete;

    public:
        // Destructor
        ~Api();

        // Api key getter
        inline const std::string& api_key() const
        {
            return api_key_;
        }

        // Api key setter
        inline void set_api_key(const std::string& api_key)
        {
            api_key_ = api_key;
        }

        // Returns an user from its username
        User get_user(const std::string& username, int mode);

        // Returns a set of beatmap from the set id
        std::vector<Beatmap> get_beatmap_set(long beatmap_set_id, int mode);

        // Returns a beatmap from its id
        Beatmap get_beatmap(long beatmap_id, int mode);

        // Singleton get instance
        static Api& get_instance()
        {
            static Api instance;

            return instance;
        }
    };
}

// Api get instance macro
#define OsuApi Osu::Api::get_instance()

#endif // OSU_API_H_
