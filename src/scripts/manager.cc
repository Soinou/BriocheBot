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

#include "scripts/manager.h"

#include "utils/logger.h"
#include "utils/utils.h"

// Constructor
ScriptManager::ScriptManager() : scripts_()
{

}

// Destructor
ScriptManager::~ScriptManager()
{

}

// Loads the script manager with the given server
void ScriptManager::load(Server* server)
{
    // Get the list of files in the scripts folder
    std::vector<std::string> scripts = Utils::get_files_from_folder("scripts", ".lua");

    // For each file
    for (auto i = scripts.begin(); i != scripts.end(); i++)
    {
        // Create a new script with this file
        Script* script = new Script(*i);

        // Load this script
        script->load(server);

        // Add this script to our list of scripts
        scripts_.push_back(std::unique_ptr<Script>(script));
    }
}

// Called when a command is triggered
void ScriptManager::on_command(const std::string& sender, const std::string& command_string, const std::vector<std::string>& arguments)
{
    try
    {
        // For each script
        for (auto i = scripts_.begin(); i != scripts_.end(); i++)
        {
            // If the script is a command script and his command string matches our command string
            if ((*i)->type() == Script::Type::Command && (*i)->command_string() == command_string)
            {
                bool should_call = true;

                // The script should have a min arguments of - 1, or the arguments should be more than this min
                should_call &= (*i)->min_arguments() == -1 || arguments.size() >= (*i)->min_arguments();

                // Same thing for the max arguments
                should_call &= (*i)->max_arguments() == -1 || arguments.size() <= (*i)->max_arguments();

                // If we have the right number of arguments
                if (should_call)
                    // Call him
                    (*i)->on_command(sender, command_string, arguments);
            }
        }
    }
    // Catch lua exceptions
    catch (std::runtime_error e)
    {
        // Log them
        Meow("errors")->error(e.what());
        fprintf(stderr, "Lua Error: %s\n", e.what());
    }
}

// Called when a message is sent
void ScriptManager::on_message(const std::string& sender, const std::string& message)
{
    try
    {
        // For each script
        for (auto i = scripts_.begin(); i != scripts_.end(); i++)
            // If the script is a message script
            if ((*i)->type() == Script::Type::Message)
                // Call him
                (*i)->on_message(sender, message);
    }
    // Catch lua exceptions
    catch (std::runtime_error e)
    {
        // Log them
        Meow("errors")->error(e.what());
        fprintf(stderr, "Lua Error: %s\n", e.what());
    }
}
