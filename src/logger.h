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

#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>

// Represents a logger
class Logger
{
private:
    // Private constructor
    Logger();
    
    // Delete copy operations
    Logger(const Logger&) = delete;

    // Delete = operator
    void operator=(const Logger&) = delete;

public:
    // Destructor
    ~Logger();

    // Trace log
    void trace(const std::string& message);

    // Debug log
    void debug(const std::string& message);

    // Info log
    void info(const std::string& message);

    // Warning log
    void warning(const std::string& message);

    // Error log
    void error(const std::string& message);

    // Fatal log
    void fatal(const std::string& message);

    // Instance getter
    inline static Logger& get_instance()
    {
        static Logger instance;

        return instance;
    }
};

// Log define
#define Log Logger::get_instance()

#endif // LOGGER_H_
