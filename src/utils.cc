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

#include "utils.h"

#include <boost/filesystem.hpp>

#include <cstdarg>
#include <stdexcept>
#include <sstream>

namespace Utils
{
    void throw_error(const std::string& file, const std::string& method, const std::string& message)
    {
        // Create a stream
        std::stringstream stream;

        // Put the informations in the stream
        stream << file;
        stream << " - ";
        stream << method;
        stream << ": ";
        stream << message;

        // Throw the error
        throw std::runtime_error(stream.str());
    }

    std::string string_format(const std::string fmt_str, ...)
    {
        int final_n, n = ((int)fmt_str.size()) * 2;

        std::string str;

        std::unique_ptr<char[]> formatted;

        va_list ap;

        while (1)
        {
            formatted.reset(new char[n]);
            strcpy(&formatted[0], fmt_str.c_str());
            va_start(ap, fmt_str);
            final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
            va_end(ap);
            if (final_n < 0 || final_n >= n)
                n += abs(final_n - n + 1);
            else
                break;
        }

        return std::string(formatted.get());
    }

    std::vector<std::string> get_files_from_folder(const std::string& folder_path, const std::string& extension)
    {
        // Get the folder as a boost path
        boost::filesystem::path folder(folder_path);

        // If the folder does not exist
        if (!boost::filesystem::exists(folder))
            // Error
            throw_error("Utils", "get_files_from_folder", string_format("Folder %s does not exist", folder_path.c_str()));

        // If the given folder is not a valid directory
        if (!boost::filesystem::is_directory(folder))
            // Error
            throw_error("Utils", "get_files_from_folder", string_format("Folder %s is not a valid directory", folder_path.c_str()));

        // End iterator
        boost::filesystem::directory_iterator end;

        // The list of files
        std::vector<std::string> files;

        // For each file
        for (boost::filesystem::directory_iterator i(folder); i != end; ++i)
            // If the file is a regular file and ends with the given extension
            if (boost::filesystem::is_regular_file(i->status()) && i->path().extension().string() == extension)
                // Add it to the list of files
                files.push_back(i->path().string());

        // Return the list of files
        return files;
    }
}
