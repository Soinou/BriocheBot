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

#include "utils.h"

#include <algorithm>
#include <cctype>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <functional>
#include <locale>
#include <memory>
#include <stdexcept>
#include <sys/stat.h>
#include <unistd.h>

namespace Utils
{
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

    void throw_error(const std::string& file, const std::string& method, const std::string& message)
    {
        // Throw the error (Format <File> - <Method>: <Message>)
        throw std::runtime_error(string_format("%s - %s: %s", file.c_str(), method.c_str(), message.c_str()));
    }

    bool ends_with(const std::string& value, const std::string& ending)
    {
        if (ending.size() > value.size())
            return false;

        return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
    }

    bool file_exists(const std::string& file_path)
    {
        return access(file_path.c_str(), F_OK) != -1;
    }

    int file_size(const std::string& file_path)
    {
        struct stat status;

        if (stat(file_path.c_str(), &status))
            return -1;
        else
            return status.st_size;
    }

    std::string& ltrim(std::string& s)
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
    }

    std::string& rtrim(std::string& s)
    {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
    }

    std::string& trim(std::string& s)
    {
        return ltrim(rtrim(s));
    }

    std::vector<std::string> get_files_from_folder(const std::string& folder_path, const std::string& extension)
    {
        // The files holder
        std::vector<std::string> files;

        // The folder holder
        DIR* directory;

        // The entity
        struct dirent* entity;

        // Try to open the folder
        directory = opendir(folder_path.c_str());

        // If we can't open it
        if (directory == nullptr)
            // Error
            throw_error("Utils", "get_files_from_folder", Utils::string_format("Impossible to open folder : %s", folder_path.c_str()));

        // Read the first file of the directory
        entity = readdir(directory);

        // While we have a file
        while (entity != nullptr)
        {
            // Get a string with this file name
            std::string file(entity->d_name);

            // If the file ends with the extension we need
            if (ends_with(file, extension))
                // Add this file to our list (Format <Folder path>/<File name>)
                files.push_back(Utils::string_format("%s/%s", folder_path.c_str(), file.c_str()));

            // Read another entry in the folder
            entity = readdir(directory);
        }

        // Close the directory
        closedir(directory);

        // Return the list of files
        return files;
    }
}
