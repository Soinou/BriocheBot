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

#include "command.h"

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

static boost::regex command_regex("!([^ ]+)( .+)?");

Command::Command(std::string command, std::vector<std::string> arguments) : command_(command), arguments_(arguments)
{

}

Command::~Command()
{

}

Command* Command::parse(const std::string& message)
{
    // Regex matches
    boost::smatch matches;

    // The command
    Command* command = nullptr;

    // If the command regex matches the message
    if (boost::regex_match(message, matches, command_regex))
    {
        // Command string is the first match
        std::string command_string = matches[1];

        // Arguments string is the second match
        std::string arguments_string = matches[2];

        // The list of arguments
        std::vector<std::string> arguments;

        // If we have an arguments string
        if (!arguments_string.empty())
        {
            // Remove any spaces in the arguments string
            boost::trim(arguments_string);

            // Split on spaces
            boost::split(arguments, arguments_string, boost::is_any_of(" "), boost::token_compress_on);
        }

        // Create a new command
        command = new Command(command_string, arguments);
    }

    // Return the created command
    return command;
}