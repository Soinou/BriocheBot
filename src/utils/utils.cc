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

#include "utils/utils.h"

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
#include <sstream>
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

    std::string to_lower(const std::string& string)
    {
        // Copy the string
        std::string result = string;

        // Change locale to utf8
        ::setlocale(LC_ALL, "fr_FR.utf-8");

        // Convert the string to lower
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);

        // Return result
        return result;
    }

    std::string current_time()
    {
        // Get the current time
        time_t current_time = time(nullptr);

        // Get the local time
        struct tm local_time = *localtime(&current_time);

        // Format the time as we want it
        std::string time_string = Utils::string_format(
            "%04d/%02d/%02d %02d:%02d:%02d",
            local_time.tm_year + 1900,
            local_time.tm_mon,
            local_time.tm_mday,
            local_time.tm_hour,
            local_time.tm_min,
            local_time.tm_sec
            );

        // Return the time string
        return time_string;
    }

    std::string time_format(int seconds)
    {
        // Get the minutes
        int minutes = seconds / 60;

        // Get the hours
        int hours = minutes / 60;

        // Format a string
        return string_format("%d hour(s) %d minute(s) %d second(s)", hours, minutes % 60, seconds % 60);
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

    std::vector<std::string>& split(const std::string& s, char delim, std::vector<std::string>& elems)
    {
        std::stringstream ss(s);
        std::string item;

        while (std::getline(ss, item, delim))
        {
            if (item.length() > 0)
            {
                elems.push_back(item);
            }
        }

        return elems;
    }

    std::vector<std::string> split(const std::string& s, char delim)
    {
        std::vector<std::string> elems;

        split(s, delim, elems);

        return elems;
    }

    // TODO: This needs to go
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
