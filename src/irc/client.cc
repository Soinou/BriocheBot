// irc
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

#include "irc/client.h"

#include "irc/parser.h"
#include "utils/logger.h"
#include "utils/utils.h"
#include "uv/dns.h"

#include <algorithm>

// If we have to output everything or not
#define VERBOSE false

// Write interval
#define WRITE_INTERVAL 200

namespace Irc
{
    ///////////////////////////////////////////////////////////
    //
    // Constructors/Destructors
    //
    ///////////////////////////////////////////////////////////

    Client::Client() : mutex_(), timer_(WRITE_INTERVAL, std::bind(&Client::on_write_timer, this, std::placeholders::_1)),
        socket_(), state_(State::kInitializing), options_(), users_(), output_()
    {
        socket_.set_on_connect([this](Uv::Socket* socket, bool result) { on_socket_connect(socket, result); });
        socket_.set_on_data([this](Uv::Socket* socket, std::string data) { on_socket_data(socket, data); });
        socket_.set_on_write([this](Uv::Socket* socket, bool result) { on_socket_write(socket, result); });
        socket_.set_on_close([this](Uv::Socket* socket) { on_socket_close(socket); });
    }

    Client::~Client()
    {
    }

    ///////////////////////////////////////////////////////////
    //
    // Libuv callbacks
    //
    ///////////////////////////////////////////////////////////

    void Client::on_write_timer(Uv::Timer* timer)
    {
        // If we are disconnecting or disconnected
        if (state_ == kDisconnecting || state_ == kDisconnected)
            // Stop the timer
            timer->stop();
        // Else
        else
        {
            // While we have something in our output queue and we are in connected state
            while (!output_.empty() && state_ == kConnected)
            {
                // Get the message on the top of the queue
                std::string message = output_.front();

                // Append line endings to the message
                message += "\r\n";

                // Send it
                socket_.write(message);

                // Pop it
                output_.pop();
            }
        }
    }

    void Client::on_socket_connect(Uv::Socket* socket, bool result)
    {
        // Start to read from the socket
        socket_.read_start();

        // Send usual commands
        socket_.write(Utils::string_format("PASS %s\r\n", options_.password.c_str()));
        socket_.write(Utils::string_format("NICK %s\r\n", options_.username.c_str()));
        socket_.write(Utils::string_format("USER %s unknown unknown %s\r\n", options_.username.c_str(), options_.realname.c_str()));

        // We are connecting
        state_ = kConnecting;
    }

    void Client::on_socket_data(Uv::Socket* socket, std::string data)
    {
        // Split lines
        std::vector<std::string> lines = Utils::split(data, '\n');

        // Parse each line
        for (auto i = lines.begin(); i != lines.end(); i++)
        {
            // Create a new parser
            Parser parser((*i));

            // Parse the reply
            Reply reply = parser.parse();

            // Handle the reply
            handle(reply);
        }
    }

    void Client::on_socket_write(Uv::Socket* socket, bool result)
    {
        // Ignore it ?
    }

    void Client::on_socket_close(Uv::Socket* socket)
    {
        // We are disconnected
        state_ = kDisconnected;
    }

    ///////////////////////////////////////////////////////////
    //
    // Irc handlers
    //
    ///////////////////////////////////////////////////////////

    void Client::handle(const Reply& reply)
    {
        // Emit on raw
        on_raw.emit(reply);

        // Do something depending on the reply type
        switch (reply.type)
        {
            case Reply::kConnected:
                state_ = kConnected;
                timer_.start();
                on_connect.emit();
                break;
            case Reply::kName:
                handle_on_name(reply);
                break;
            case Reply::kPing:
                output_.push("PONG");
                break;
            case Reply::kJoin:
                handle_on_join(reply);
                break;
            case Reply::kPart:
                handle_on_part(reply);
                break;
            case Reply::kMessage:
                handle_on_message(reply);
                break;
            case Reply::kInvalid:
            default:
                break;
        }
    }

    void Client::handle_on_name(const Reply& reply)
    {
        // Lock the mutex
        mutex_.lock();

        // Get the first object of our parameters list
        auto i = reply.parameters.begin();

        // Get the channel
        std::string channel = *i;

        // While we have names
        while (++i != reply.parameters.end())
            // Push the name to the list of users for this channel
            users_[channel].push_back(*i);

        // Unlock the mutex
        mutex_.unlock();
    }

    void Client::handle_on_join(const Reply& reply)
    {
        // Lock the mutex
        mutex_.lock();

        // Get the channel and the username
        std::string channel = reply.parameters[0];
        std::string username = reply.username;

        // Get the channel list
        std::vector<std::string>& channel_list = users_[channel];

        // If the user doesn't exist yet in our list
        if (std::find(channel_list.begin(), channel_list.end(), username) == channel_list.end())
        {
            // Add him to our list
            channel_list.push_back(username);

            // Emit on join
            on_join.emit(channel, username);
        }

        // Unlock the mutex
        mutex_.unlock();
    }

    void Client::handle_on_part(const Reply& reply)
    {
        // Lock the mutex
        mutex_.lock();

        // Get the channel and the username
        std::string channel = reply.parameters[0];
        std::string username = reply.username;

        // Get the channel list
        std::vector<std::string>& channel_list = users_[channel];

        // Try to remove the user from our list
        auto i = std::remove_if(channel_list.begin(), channel_list.end(), [&username](const std::string& element)
        {
            return element == username;
        });

        // If we could remove him
        if (i != channel_list.end())
        {
            // Reprocess the vector
            channel_list.erase(i);

            // Emit on part
            on_part.emit(channel, username);
        }

        // Unlock the mutex
        mutex_.unlock();
    }

    void Client::handle_on_message(const Reply& reply)
    {
        // Get the target
        std::string target = reply.parameters[0];

        // Get the sender username
        std::string username = reply.username;

        // Get the message
        std::string message = reply.parameters[1];

        // First character of the target is #
        if (target[0] == '#')
            // It's a channel message
            on_message.emit(target, username, message);
        // Else
        else
            // It's a private message
            on_private_message.emit(username, message);
    }

    ///////////////////////////////////////////////////////////
    //
    // Protected methods
    //
    ///////////////////////////////////////////////////////////

    void Client::add_users(const std::string& channel, const std::vector<std::string>& list)
    {
        // Lock the mutex
        mutex_.lock();

        // Get the right list
        std::vector<std::string>& channel_list = users_[channel];

        // For each name in the given list
        for (auto i = list.begin(); i != list.end(); i++)
        {
            // If we can't find this user in our list
            if (std::find(channel_list.begin(), channel_list.end(), *i) == channel_list.end())
            {
                // Append him
                channel_list.push_back(*i);

                // Emit on join
                on_join.emit(channel, *i);
            }
        }

        // Unlock the mutex
        mutex_.unlock();
    }

    ///////////////////////////////////////////////////////////
    //
    // Start/Stop
    //
    ///////////////////////////////////////////////////////////

    void Client::start()
    {
        // Get the server ip/port
        Uv::get_address(options_.server, std::to_string(options_.port), [this](bool result, std::string message, sockaddr address)
        {
            // Connect the socket
            socket_.connect(address);
        });
    }

    void Client::stop()
    {
        // Stop the socket
        socket_.close();

        // We are disconnecting
        state_ = kDisconnecting;
    }

    ///////////////////////////////////////////////////////////
    //
    // Irc commands
    //
    ///////////////////////////////////////////////////////////

    void Client::send(const std::string& target, const std::string& message)
    {
        output_.push(Utils::string_format("PRIVMSG %s :%s", target.c_str(), message.c_str()));
    }

    void Client::join(const std::string& channel)
    {
        output_.push(Utils::string_format("JOIN %s", channel.c_str()));
    }

    void Client::join(const std::string& channel, const std::string& password)
    {
        output_.push(Utils::string_format("JOIN %s :%s", channel.c_str(), password.c_str()));
    }

    void Client::part(const std::string& channel)
    {
        output_.push(Utils::string_format("PART %s", channel.c_str()));
    }
}
