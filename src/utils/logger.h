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

#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <list>

// Represents a logger
class Log
{
private:
    // If we're logging in debug mode
    bool debug_;

    // The file name to log to
    std::string file_name_;

    // If the logger is running
    bool running_;

    // The background thread
    std::thread thread_;

    // The message mutex
    std::mutex mutex_;

    // The things we want to write to the file
    std::queue<std::string> messages_;

    // Append a message to our queue of messages
    void append(const std::string& status, const std::string& message);

    // The background thread worker
    void thread_worker();

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

    // Wait for the logger to stop
    void wait();

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

    // Wait for the logger to terminate
    void wait();

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
