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

#ifndef SCRIPT_H_
#define SCRIPT_H_

#include "icommand_listener.h"
#include "imessage_listener.h"
#include "lua_state.h"

// Forward declaration of the server class
class Server;

// Represents a script
class Script : public ICommandListener, public IMessageListener
{
public:
    // The different script types
    enum Type
    {
        None,
        Command,
        Message
    };

private:
    // The file of the script
    std::string file_path_;

    // The lua state
    Lua::State state_;

    // The script type
    Type type_;

    // The command string (Only for command scripts)
    std::string command_string_;

    // The minimum number of arguments (Only for command scripts)
    int min_arguments_;

    // The maximum number of arguments (Only for command scripts)
    int max_arguments_;

public:
    // Constructor
    Script(const std::string& file_path);

    // Destructor
    ~Script();

    // File path getter
    inline const std::string& file_path() const
    {
        return file_path_;
    }

    // State getter
    inline const Lua::State& state() const
    {
        return state_;
    }

    // Type getter
    inline const Type& type() const
    {
        return type_;
    }

    // Command string getter
    inline const std::string& command_string() const
    {
        return command_string_;
    }

    // Min arguments getter
    inline int min_arguments() const
    {
        return min_arguments_;
    }

    // Max arguments getter
    inline int max_arguments() const
    {
        return max_arguments_;
    }

    // Loads the script using the given server
    void load(Server* server);

    // Called when a command is triggered
    void on_command(const std::string& sender, const std::string& command_string, const std::vector<std::string>& arguments);

    // Called when a message is received
    void on_message(const std::string& sender, const std::string& message);
};

#endif // SCRIPT_H_
