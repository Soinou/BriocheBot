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

#include "script.h"

#include "lua_wrapper.h"
#include "utils.h"

#include <cstdio>

Script::Script(const std::string& file_path)
    : file_path_(file_path), state_(), type_(Type::None), command_string_(), min_arguments_(0), max_arguments_(0)
{
    state_.open_libs();
}

Script::~Script()
{

}

void Script::load(Server* server)
{
    // Push the server on the stack
    luaW_push(*state_, server);

    // Set the server variable name as "server"
    lua_setglobal(*state_, "server");

    // Load the lua file
    state_.do_file(file_path_);

    // Get this global as a string
    std::string script_type = state_.get_string("ScriptType");

    // Script type is a command
    if (script_type == "command")
    {
        // Change the script type
        type_ = Type::Command;

        // Get some script settings
        command_string_ = state_.get_string("CommandString");
        min_arguments_ = state_.get_integer("MinArguments");
        max_arguments_ = state_.get_integer("MaxArguments");
    }
    // Script type is a message
    else if (script_type == "message")
    {
        // Change the script type
        type_ = Type::Message;
    }
    // Script type does not exist
    else
        // Error
        Utils::throw_error("Script", "load", Utils::string_format("Script type %s does not exist", script_type.c_str()));
}

void Script::on_command(const std::string& sender, const std::string& command_string, const std::vector<std::string>& arguments)
{
    // Get the onCommand function on the stack
    state_.get_function("onCommand");

    // Push the sender to the stack
    state_.push(sender);

    // For each argument
    for (auto i = arguments.begin(); i < arguments.end(); i++)
        // Push it to the stack
        state_.push(*i);

    // Call the onCommand function
    state_.call(arguments.size() + 1, 0);
}

void Script::on_message(const std::string& sender, const std::string& message)
{
    // Get the onMessage function on the stack
    state_.get_function("onMessage");

    // Push the sender
    state_.push(sender);

    // Push the message
    state_.push(message);

    // Call the onMessage function
    state_.call(2, 0);
}
