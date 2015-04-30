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

#include "logger.h"

#include "utils.h"

#include <cstdio>
#include <ctime>
#include <sys/stat.h>
#include <unistd.h>

// Log file
#define LOG_FILE "logs/%s_%d.log"

// Max size
#define MAX_SIZE 1024*1024

// Sleep time of 200 milliseconds (Makes the logger write message at a rate of approximatively five per second)
#define SLEEP_TIME 200

Log::Log(const std::string& file_name) : file_name_(file_name), running_(true), thread_(&Log::thread_worker, this), messages_()
{

}

Log::~Log()
{

}

void Log::trace(const std::string& message)
{
    append(Utils::string_format("[Trace] %s", message.c_str()));
}

void Log::debug(const std::string& message)
{
    append(Utils::string_format("[Debug] %s", message.c_str()));
}

void Log::info(const std::string& message)
{
    append(Utils::string_format("[Info] %s", message.c_str()));
}

void Log::warning(const std::string& message)
{
    append(Utils::string_format("[Warning] %s", message.c_str()));
}

void Log::error(const std::string& message)
{
    append(Utils::string_format("[Error] %s", message.c_str()));
}

void Log::fatal(const std::string& message)
{
    append(Utils::string_format("[Fatal] %s", message.c_str()));
}

void Log::wait()
{
    // Pass running to false, so the thread will stop
    running_ = false;

    // Wait for the thread to terminate
    thread_.join();
}

void Log::append(const std::string& message)
{
    // Lock the mutex
    mutex_.lock();

    // Push the message
    messages_.push(message);

    // Unlock the mutex
    mutex_.unlock();
}

static inline FILE* file_open(const std::string& file_path)
{
    // Open the file
    FILE* file = fopen(file_path.c_str(), "a");

    // File couldn't not be opened
    if (file == nullptr)
        // Error
        Utils::throw_error("Logger", "thread_worker", Utils::string_format("Couldn't open log file %s", file_path.c_str()));

    // Return the file
    return file;
}

void Log::thread_worker()
{
    // The actual number appended to the log file end
    int count = 0;

    // The file pointer
    FILE* file = nullptr;

    // While the logger is running or we have messages to log
    while (running_ || !messages_.empty())
    {
        try
        {
            // While the file is not found
            while (file == nullptr)
            {
                // Get the file name
                std::string file_path = Utils::string_format(LOG_FILE, file_name_.c_str(), count);

                // If the file exists
                if (Utils::file_exists(file_path))
                {
                    // Get the file size
                    int size = Utils::file_size(file_path);

                    // If the file size is negative
                    if (size == -1)
                        // error
                        throw std::runtime_error("Logger - thread_worker: Couldn't get the log file size");
                    // Else, if the file size is correct
                    else if (size < MAX_SIZE)
                        // Open the file
                        file = file_open(file_path);
                    // Else (The file is too big)
                    else
                        // Increase our counter
                        count++;
                }
                // Else, the file doesn't exist
                else
                    // Open this file
                    file = file_open(file_path);
            }

            // Now that we have the right file, get the current time
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

            // Lock the messages mutex
            mutex_.lock();

            // If we have a message in our list
            if (!messages_.empty())
            {
                // Get one item from the queue of messages
                std::string message = messages_.front();

                // Pop the item
                messages_.pop();

                // Write this item to the file
                fprintf(file, "[%s] %s\n", time_string.c_str(), message.c_str());
            }

            // Unlock the mutex
            mutex_.unlock();

            // Close the file
            fclose(file);

            // Delete the file
            file = nullptr;
        }
        catch (std::runtime_error e)
        {
            // Log errors to standard error stream (But don't stop the thread)
            fprintf(stderr, "Error: %s\n", e.what());
        }

        // Sleep a while
        usleep(SLEEP_TIME);
    }

}

Logger::Logger() : loggers_()
{

}

Logger::~Logger()
{
    for (auto i = loggers_.begin(); i != loggers_.end(); i++)
        delete (*i);
}

Log* Logger::get_logger(const std::string& file_name)
{
    for (auto i = loggers_.begin(); i != loggers_.end(); i++)
        if ((*i)->file_name() == file_name)
            return (*i);

    Log* new_log = new Log(file_name);

    loggers_.push_back(new_log);

    return new_log;
}

void Logger::wait()
{
    for (auto i = loggers_.begin(); i != loggers_.end(); i++)
        (*i)->wait();
}