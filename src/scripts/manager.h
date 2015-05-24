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

#ifndef SCRIPT_MANAGER_H_
#define SCRIPT_MANAGER_H_

#include "models/icommand_listener.h"
#include "models/imessage_listener.h"
#include "scripts/script.h"

#include <memory>

// Represents a script manager
class ScriptManager : public ICommandListener, public IMessageListener
{
private:
    // All the scripts
    std::vector <std::unique_ptr<Script>> scripts_;

public:
    // Constructor
    ScriptManager();

    // Destructor
    ~ScriptManager();

    // Loads the script manager with the given server
    void load(Server* server);

    // Called when a command is triggered
    void on_command(const std::string& sender, const std::string& command_string, const std::vector<std::string>& arguments);

    // Called when a message is sent
    void on_message(const std::string& sender, const std::string& message);
};

#endif // SCRIPT_MANAGER_H_
