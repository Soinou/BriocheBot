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

// If we have to output everything or not
#define VERBOSE false

// Write interval
#define WRITE_INTERVAL 200

namespace Irc
{
    Client::Client()
        : timer_(WRITE_INTERVAL, std::bind(&Client::on_write_timer, this, std::placeholders::_1)),
        socket_(), state_(State::kInitializing), options_(), output_()
    {
        socket_.set_on_connect([this](Uv::Socket* socket, bool result) { on_socket_connect(socket, result); });
        socket_.set_on_data([this](Uv::Socket* socket, std::string data) { on_socket_data(socket, data); });
        socket_.set_on_write([this](Uv::Socket* socket, bool result) { on_socket_write(socket, result); });
        socket_.set_on_close([this](Uv::Socket* socket) { on_socket_close(socket); });
    }

    Client::~Client()
    {
    }

    void Client::on_write_timer(Uv::Timer* timer)
    {
        // If we are disconnecting or disconnected
        if (state_ == kDisconnecting || state_ == kDisconnected)
            // Stop the timer
            timer->stop();
        // Else
        else
        {
            // While we have something in our output queue
            while (!output_.empty())
            {
                // Get the message on the top of the queue
                std::string message = output_.front();

                // Append line endings to the message
                message += 0x0D;
                message += 0x0A;

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
        output_.push(Utils::string_format("PASS %s", options_.password.c_str()));
        output_.push(Utils::string_format("NICK %s", options_.username.c_str()));
        output_.push(Utils::string_format("USER %s unknown unknown %s", options_.username.c_str(), options_.realname.c_str()));

        // We are connected
        state_ = State::kConnected;

        // Start the write timer
        timer_.start();
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

            // Parse the message
            Message message = parser.parse();

            // Handle the message
            handle(message);
        }
    }

    void Client::on_socket_write(Uv::Socket* socket, bool result)
    {
        // Ignore it ?
    }

    void Client::on_socket_close(Uv::Socket* socket)
    {
        // We are disconnected
        state_ = State::kDisconnected;
    }

    void Client::handle(const Message& message)
    {
        // Call on raw
        on_raw(message);

        // Copy the command in a local variable
        std::string command = message.command;

        // RPL_ENDOFMOTD
        if (command == "376")
            // We are officially connected
            on_connect();
        // PING
        else if (command == "PING")
            // Respond with a pong
            output_.push("PONG");
        // PRIVMSG
        else if (command == "PRIVMSG")
        {
            // First character of the destination is a #
            if (message.middle[0] == '#')
                // It's a channel message
                on_message(message.nick, message.middle, message.trailing);
            // No #
            else
                // It's a private message
                on_private_message(message.nick, message.trailing);
        }
    }

    void Client::start()
    {
        // Get the server ip/port
        Uv::get_address(options_.server, std::to_string(options_.port), [this](bool result, std::string message, sockaddr address)
        {
            // Connect the socket
            socket_.connect(address);

            // We are connecting
            state_ = State::kConnecting;
        });
    }

    void Client::stop()
    {
        // Stop the socket
        socket_.close();

        // We are disconnecting
        state_ = State::kDisconnecting;
    }

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

    // Default handlers

    void Client::on_connect()
    {
        if (VERBOSE)
            Meow("irc")->info("[Irc::Client](on_connect): Connected!");
    }

    void Client::on_message(const std::string& sender, const std::string& channel, const std::string& message)
    {
        if (VERBOSE)
            Meow("irc")->info(Utils::string_format("[Irc::Client](on_message): [%s] %s: %s", channel.c_str(), sender.c_str(), message.c_str()));
    }

    void Client::on_private_message(const std::string& sender, const std::string& message)
    {
        if (VERBOSE)
            Meow("irc")->info(Utils::string_format("[Irc::Client](on_private_message): %s: %s", sender.c_str(), message.c_str()));
    }

    void Client::on_raw(const Message& message)
    {
        if (VERBOSE)
            Meow("irc")->info(Utils::string_format("[Irc::Client](on_raw): %s / %s ! %s @ %s [ %s ] : %s - %s\n",
            message.server.c_str(),
            message.nick.c_str(), message.user.c_str(), message.host.c_str(),
            message.command.c_str(), message.middle.c_str(), message.trailing.c_str()));
    }
}
