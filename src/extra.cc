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

///////////////////////////////////////////////
//
// Current mode: Database migration
// Will migrate everything from the old players
// to the new viewers-models
//
///////////////////////////////////////////////

#include "models/viewers.h"
#include "models/viewer.h"
#include "models/streamer.h"
#include "models/moderator.h"
#include "utils/redis.h"
#include "utils/utils.h"

// Extra program, used to do some extra things
int main()
{
    the_viewers.load();

    // Connect to redis
    Redis::Connection connection("127.0.0.1", 6379);

    // Json reader we'll use
    Json::Reader reader;

    printf("Getting all keys...\n\n");

    // Get all the keys we have in redis
    Redis::Reply reply = connection.execute("KEYS *");

    printf("Got all keys, scanning keys...\n");

    // If we got an array
    if (reply.type == Redis::Reply::Array)
    {
        int count = the_viewers.max_id();

        // For each key
        for (auto i = reply.elements.begin(); i != reply.elements.end(); i++)
        {
            // Get this element
            Redis::Reply element = *i;

            printf("  Scanning key \"%s\"...\n", element.string.c_str());

            // Get the element value
            int value = atoi(element.string.c_str());

            // We got 0
            if (value == 0)
                // Not valid
                printf("    Key is not valid, skipping\n\n");
            // We got a number
            else
            {
                // Valid
                printf("    Key is a streamer, getting data...\n", value);

                // Get the streamer
                Redis::Reply streamer = connection.execute(Utils::string_format("GET %d", value));

                // Reply is a string
                if (streamer.type == Redis::Reply::String)
                {
                    printf("      Got data, creating streamer...\n");

                    // Json we'll need
                    Json::Value json;

                    // Parse the json
                    reader.parse(streamer.string, json);

                    // If the streamer already exists, skip
                    if (the_viewers.exists(json["twitch_username"].asString()))
                        printf("      Streamer already exists, skipping\n\n");
                    // If the streamer doesn't already exists
                    else
                    {
                        // Create a new streamer
                        Streamer* new_streamer = nullptr;

                        // If this streamer was an admin
                        if (json["admin"].asBool())
                        {
                            // Create a new moderator
                            Moderator* moderator = new Moderator();
                            // Set his privileges
                            moderator->set_privileges(1);
                            // Pass it to our viewer
                            new_streamer = moderator;
                        }
                        // Else
                        else
                            // Just create a new streamer
                            new_streamer = new Streamer();

                        // Set streamer data
                        new_streamer->set_id(count++);
                        new_streamer->set_twitch_username(json["twitch_username"].asString());
                        new_streamer->set_osu_username(json["osu_username"].asString());
                        new_streamer->set_osu_skin_link(json["osu_skin"].asString());

                        printf("      Streamer created, adding him...\n");

                        // Insert the streamer
                        new_streamer->insert();

                        printf("      Streamer added!\n\n");
                    }
                }
            }
        }
    }

    return 0;
}
