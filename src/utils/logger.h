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

#include "uv/file.h"
#include "uv/timer.h"

#include <queue>
#include <string>
#include <list>

// Represents a logger
class Log
{
private:
    // If we're logging in debug mode
    bool debug_;

    // The file where we write to
    Uv::File file_;

    // The file name to log to
    std::string file_name_;

    // The current file count (Number appended to the file name)
    int file_count_;

    // If the logger is running
    bool running_;

    // The things we want to write to the file
    std::queue<std::string> messages_;

    // The timer
    Uv::Timer timer_;

    // Append a message to our queue of messages
    void append(const std::string& status, const std::string& message);

    // The timer callback
    void timer_callback(Uv::Timer* timer);

    // File callbacks
    void on_file_open(Uv::File* file, bool result);
    void on_file_size(Uv::File* file, uint64_t size);
    void on_file_read(Uv::File* file, std::string data, bool result);
    void on_file_write(Uv::File* file, bool result);
    void on_file_close(Uv::File* file);

public:
    // Private constructor
    Log(const std::string& file_name);

    // Destructor
    ~Log();

    // File name getter
    inline const std::string& file_name() const
    {
        return file_name_;
    }

    // If we're logging in debug mode
    inline bool debug() const
    {
        return debug_;
    }

    // Change the debug flag
    inline void set_debug(bool debug)
    {
        debug_ = debug;
    }

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

    // Stops the logger
    void stop();
};

class Logger
{
private:
    // The list of loggers
    std::list<Log*> loggers_;

    // Constructor
    Logger();

    // Delete copy constructor
    Logger(const Logger&) = delete;

    // Delete copy operator
    void operator=(const Logger&) = delete;

public:
    // Destructor
    ~Logger();

    // Get a logger by its filename
    Log* get_logger(const std::string& file_name);

    // Stop all the loggers
    void stop();

    // Singleton get instance
    static Logger& get_instance()
    {
        static Logger instance;

        return instance;
    }
};

// Meow to log
#define Meow Logger::get_instance().get_logger

#endif // LOGGER_H_
