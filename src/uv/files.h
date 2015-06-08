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

#ifndef UV_FILES_H_
#define UV_FILES_H_

#include <functional>
#include <string>

namespace Uv
{
    // Read operation (Two parameters: success/not success, content)
    typedef std::function<void(bool, std::string)> file_callback;

    // Read the contents of the given file
    void read_contents(const std::string& file_path, file_callback callback);

    // Writes given content to the given file (Will create the file if it doesn't exist yet)
    void write_contents(const std::string& file_path, const std::string& content, bool append, file_callback callback);

    // Size operation (Two parameters: success/not sucess, size)
    typedef std::function<void(bool, uint64_t)> size_callback;

    // Get file informations
    void file_size(const std::string& file_path, size_callback callback);
}

#endif // UV_FILES_H_
