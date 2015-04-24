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

#include "logger.h"

#include <boost/log/expressions.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>

// Typedef to this monster to blog
typedef boost::log::sources::severity_logger<boost::log::trivial::severity_level> blog;

// The static logger
static std::unique_ptr<blog> logger_ptr(nullptr);

Logger::Logger()
{
    // Add a file log
    boost::log::add_file_log(
        // File name
        boost::log::keywords::file_name = "ircbot_%m%d%Y_%H%M%S.log",
        // Rotation size (Every Mb)
        boost::log::keywords::rotation_size = 1024 * 1024,
        // Target folder
        boost::log::keywords::target = "logs",
        // Time based rotation (Every night)
        boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
        // Message format
        boost::log::keywords::format = "[%TimeStamp%]: %Message%",
        // Auto flush
        boost::log::keywords::auto_flush = true
        );

    // Filter for info severity
    boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::info);

    // Add common log attributes
    boost::log::add_common_attributes();

    // Create the new logger
    logger_ptr.reset(new blog());
}

Logger::~Logger()
{

}

// Trace log
void Logger::trace(const std::string& message)
{
    BOOST_LOG_SEV(*logger_ptr, boost::log::trivial::severity_level::trace) << message;
}

// Debug log
void Logger::debug(const std::string& message)
{
    BOOST_LOG_SEV(*logger_ptr, boost::log::trivial::severity_level::debug) << message;
}

// Info log
void Logger::info(const std::string& message)
{
    BOOST_LOG_SEV(*logger_ptr, boost::log::trivial::severity_level::info) << message;
}

// Warning log
void Logger::warning(const std::string& message)
{
    BOOST_LOG_SEV(*logger_ptr, boost::log::trivial::severity_level::warning) << message;
}

// Error log
void Logger::error(const std::string& message)
{
    BOOST_LOG_SEV(*logger_ptr, boost::log::trivial::severity_level::error) << message;
}

// Fatal log
void Logger::fatal(const std::string& message)
{
    BOOST_LOG_SEV(*logger_ptr, boost::log::trivial::severity_level::fatal) << message;
}