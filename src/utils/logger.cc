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

#include "utils/logger.h"

#include "utils/utils.h"

#include <cstdio>

// Log file
#define LOG_FILE "logs/%s_%d.log"

// Max size
#define MAX_SIZE 1024*1024

// Sleep time of 200 milliseconds (Makes the logger write message at a rate of approximatively five per second)
#define SLEEP_TIME 200

Log::Log(const std::string& file_name)
    : debug_(true), file_(), file_name_(file_name), file_count_(0),
    running_(true), messages_(), timer_(SLEEP_TIME, std::bind(&Log::timer_callback, this, std::placeholders::_1))
{
    // Set file callbacks
    file_.set_on_open([this](Uv::File* file, bool result) { on_file_open(file, result); });
    file_.set_on_size([this](Uv::File* file, uint64_t size) { on_file_size(file, size); });
    file_.set_on_read([this](Uv::File* file, std::string data, bool result) { on_file_read(file, data, result); });
    file_.set_on_write([this](Uv::File* file, bool result) { on_file_write(file, result); });
    file_.set_on_close([this](Uv::File* file) { on_file_close(file); });

    // Start the timer directly
    timer_.start();
}

Log::~Log()
{

}

void Log::trace(const std::string& message)
{
    append("Trace", message);
}

void Log::debug(const std::string& message)
{
    append("Debug", message);
}

void Log::info(const std::string& message)
{
    append("Info", message);
}

void Log::warning(const std::string& message)
{
    append("Warning", message);
}

void Log::error(const std::string& message)
{
    append("Error", message);
}

void Log::fatal(const std::string& message)
{
    append("Fatal", message);
}

void Log::stop()
{
    // Pass running to false, so the timer will automatically stop itself
    running_ = false;
}

void Log::append(const std::string& status, const std::string& message)
{
    // If we're in debug mode
    if (debug_)
        // Also append to stdout
        printf("[%s] %s\n", status.c_str(), message.c_str());

    // Push the message
    messages_.push(Utils::string_format("[%s] %s", status.c_str(), message.c_str()));
}

void Log::timer_callback(Uv::Timer* timer)
{
    // Check if we have messages
    bool empty = messages_.empty();

    // If we have to stop and our message queue is empty
    if (!running_ && empty)
    {
        // Close the file
        file_.close();

        // Stop the timer
        timer->stop();
    }
    // Else if we have something to write and the file is not opening
    else if (!empty && !file_.opening())
    {
        // If the file is not opened
        if (!file_.opened())
        {
            // Get the file name
            std::string file_path = Utils::string_format(LOG_FILE, file_name_.c_str(), file_count_);

            // Open this file in append mode
            file_.open(file_path, true);
        }
        // Else, file is opened
        else
        {
            // Get the current time
            std::string current_time = Utils::current_time();

            // Get the first item from the queue of messages
            std::string message = messages_.front();

            // Pop the item
            messages_.pop();

            // Write the message to the file
            file_.write(Utils::string_format("[%s] %s\n", current_time.c_str(), message.c_str()));

            // Request the new file size, to handle closing the file and opening a new one if we need to
            file_.size();
        }
    }
}

void Log::on_file_open(Uv::File* file, bool result)
{
    // File could not be opened, stop the timer
    if (!result)
        timer_.stop();
    // File is opened
    else
        // Get the file size
        file_.size();
}

void Log::on_file_size(Uv::File* file, uint64_t size)
{
    // Size could not be retrieved, stop the timer
    if (size < 0)
        timer_.stop();
    // File is too big
    else if (size > MAX_SIZE)
    {
        // Increase the counter
        file_count_++;

        // Close the file
        file_.close();
    }
}

void Log::on_file_read(Uv::File* file, std::string data, bool result)
{
    // We don't read, we don't care
}

void Log::on_file_write(Uv::File* file, bool result)
{
    // We don't care
}

void Log::on_file_close(Uv::File* file)
{
    // We don't care
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

void Logger::stop()
{
    for (auto i = loggers_.begin(); i != loggers_.end(); i++)
        (*i)->stop();
}