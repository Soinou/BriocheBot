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

#ifndef IRC_SCANNER_H_
#define IRC_SCANNER_H_

#include <string>

namespace Irc
{
    // Represents a token
    struct Token
    {
        // Represents a token type
        enum Type
        {
            // Word
            kWord,

            // Exclamation mark (!)
            kExclamation,

            // At (@)
            kAt,

            // Semi colon (:)
            kSemiColon,

            // Line end
            kEnd
        };

        // The token type
        Type type;

        // The token representation
        std::string representation;
    };


    // Irc reply scanner
    class Scanner
    {
    private:
        // The line we are reading
        std::string line_;

        // The offset we are currently at
        int offset_;

        // The next token
        Token next_;

        // A string buffer
        std::string buffer_;

    public:
        // Constructor
        Scanner(const std::string& line);

        // Destructor
        ~Scanner();

        // Line getter
        inline const std::string& line() const
        {
            return line_;
        }

        // Next token getter
        inline const Token& next() const
        {
            return next_;
        }

        // Skip whitespaces
        void skip();

        // Advances to the next token and return him
        void advance();

        // Get the remaining characters and concatenate it
        std::string remaining();
    };
}

#endif // IRC_SCANNER_H_
