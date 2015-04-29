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

#ifndef UTILS_H_
#define UTILS_H_

#include <string>
#include <vector>

namespace Utils
{
    // Formats a string using a sprintf-like syntax (God I love that. Taken from http://stackoverflow.com/a/8098080)
    std::string string_format(const std::string fmt_str, ...);

    // Throws a formatted error with the given arguments
    void throw_error(const std::string& file, const std::string& method, const std::string& message);

    // Tests if a string ends with a given string (Taken from http://stackoverflow.com/a/2072890/4832906)
    bool ends_with(const std::string& value, const std::string& ending);

    // Tests if a file exists (Taken from http://stackoverflow.com/a/230068/4832906)
    bool file_exists(const std::string& file_path);

    // Gets the size of a file (Taken from http://stackoverflow.com/a/8384/4832906)
    int file_size(const std::string& file_path);

    // Trim from start (Taken from http://stackoverflow.com/a/217605/4832906)
    std::string& ltrim(std::string& s);

    // Trim from end (Taken from http://stackoverflow.com/a/217605/4832906)
    std::string& rtrim(std::string& s);

    // Trim from both ends (Taken from http://stackoverflow.com/a/217605/4832906)
    std::string& trim(std::string& s);

    // Get all files from a folder with the given extension (Rough idea taken from http://stackoverflow.com/a/612176/4832906)
    std::vector<std::string> get_files_from_folder(const std::string& folder_path, const std::string& extension);

    // Tokenize a string using delimiters (Taken from http://stackoverflow.com/a/1493195/4832906)
    template < class ContainerT >
    void tokenize(const std::string& str, ContainerT& tokens,
                  const std::string& delimiters = " ", bool trimEmpty = false)
    {
        std::string::size_type pos, lastPos = 0;

        using value_type = typename ContainerT::value_type;
        using size_type = typename ContainerT::size_type;

        while (true)
        {
            pos = str.find_first_of(delimiters, lastPos);
            if (pos == std::string::npos)
            {
                pos = str.length();

                if (pos != lastPos || !trimEmpty)
                    tokens.push_back(value_type(str.data() + lastPos,
                    (size_type)pos - lastPos));

                break;
            }
            else
            {
                if (pos != lastPos || !trimEmpty)
                    tokens.push_back(value_type(str.data() + lastPos,
                    (size_type)pos - lastPos));
            }

            lastPos = pos + 1;
        }
    }
}

#endif // UTILS_H_
