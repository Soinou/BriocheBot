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

#include "irc/scanner.h"

#include "utils/utils.h"

#include <cctype>

namespace Irc
{
    Scanner::Scanner(const std::string& line) : line_(line), offset_(0), next_(), buffer_()
    {
        // Trim the line
        Utils::trim(line_);
    }

    Scanner::~Scanner()
    {
    }

    void Scanner::skip()
    {
        // While the offset is not outside of bounds and the current character is a space
        while (offset_ < line_.length() && std::isspace(line_[offset_]))
            // Advance
            offset_++;
    }

    void Scanner::advance()
    {
        // Clear the buffer
        buffer_.clear();

        // Skip whitespaces
        skip();

        // We are at the end
        if (offset_ >= line_.length())
        {
            // Return an end token
            next_.type = Token::Type::kEnd;
            next_.representation = "";
        }
        // We are not at the end
        else
        {
            // Based on the current character
            switch (line_[offset_])
            {
                case ':':
                    // Semi colon
                    next_.type = Token::Type::kSemiColon;
                    next_.representation = ":";
                    offset_++;
                    break;
                case '@':
                    // At
                    next_.type = Token::Type::kAt;
                    next_.representation = "@";
                    offset_++;
                    break;
                case '!':
                    // Exclamation
                    next_.type = Token::Type::kExclamation;
                    next_.representation = "!";
                    offset_++;
                    break;
                default:
                {
                    // Word
                    next_.type = Token::Type::kWord;

                    // Current character holder
                    char current = line_[offset_];

                    // While the current character is valid
                    while (offset_ < line_.length() && current != '!' && current != '@' && current != ':' && !std::isspace(current))
                    {
                        // Append the current character to the buffer
                        buffer_ += current;

                        // Get the next character
                        current = line_[++offset_];
                    }

                    // Set token representation (Trim it while we're at it)
                    next_.representation = Utils::trim(buffer_);
                    break;
                }
            }
        }
    }

    std::string Scanner::remaining()
    {
        // Clear the buffer
        buffer_.clear();

        // Add the last token to our buffer
        buffer_ += next_.representation;

        // While we're not at the end
        while (offset_ < line_.length())
            // Append the current character to our buffer and increment the offset
            buffer_ += line_[offset_++];

        // Trim our buffer before returning it
        Utils::trim(buffer_);

        // Return our now complete buffer
        return buffer_;
    }
}
