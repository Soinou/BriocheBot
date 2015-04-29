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

#include "irc_client.h"

#include "utils.h"
#include "logger.h"

#include <cassert>
#include <string>

// Connect handler
static void event_connect(irc_session_t* session, const char* event, const char* origin, const char** params, unsigned int count)
{
    // Get the client
    IrcClient* client = (IrcClient*)irc_get_ctx(session);

    // We should have a client
    assert(client);

    // Notify of connection
    client->on_connect();
}

// Channel handler
static void event_channel(irc_session_t* session, const char* event, const char* origin, const char** params, unsigned int count)
{
    // Get the client
    IrcClient* client = (IrcClient*)irc_get_ctx(session);

    // We should have a client
    assert(client);

    // Notify of message
    client->on_channel(std::string(origin), std::string(params[0]), std::string(params[1]));
}

// Numeric handler
static void event_numeric(irc_session_t* session, unsigned int event, const char* origin, const char** params, unsigned int count)
{
    // Just ignore it
}

IrcClient::IrcClient()
    : callbacks_(), session_(), server_("localhost"), port_(6667),
    password_(""), nick_("default"), username_("default"), realname_("default")
{
    // Register callbacks
    callbacks_.event_connect = event_connect;
    callbacks_.event_channel = event_channel;
    callbacks_.event_numeric = event_numeric;

    // Create the session
    session_ = irc_create_session(&callbacks_);

    // If there is no session
    if (!session_)
        // Throw an error
        Utils::throw_error("IrcClient", "Constructor", Utils::string_format("Impossible to initialize the irc session: %s", irc_strerror(irc_errno(session_))));

    // Set session context
    irc_set_ctx(session_, this);

    // Enable automatic nickname parsing
    irc_option_set(session_, LIBIRC_OPTION_STRIPNICKS);
}

IrcClient::~IrcClient()
{
    // We should have a session
    assert(session_);

    irc_destroy_session(session_);
}

void IrcClient::connect()
{
    // We should have a session
    assert(session_);

    Meow("irc")->info(Utils::string_format("Connecting to irc server %s", server_.c_str()));

    // Parse the new server address (#Server on SSL and just Server on non-SSL)
    std::string new_server = ssl_ ? Utils::string_format("#%s", server_.c_str()) : server_;

    // If we could not connect
    if (irc_connect(session_, new_server.c_str(), port_, password_.empty() ? 0 : password_.c_str(),
        nick_.c_str(), username_.c_str(), realname_.c_str()))
        // Throw an error
        Utils::throw_error("IrcClient", "connect", Utils::string_format("Impossible to connect to the server: %s", irc_strerror(irc_errno(session_))));
}

void IrcClient::stop()
{
    // We should have a session
    assert(session_);

    // Disconnect the session
    irc_disconnect(session_);
}

void IrcClient::send(const std::string& target, const std::string& message)
{
    // We should have a session
    assert(session_);

    Meow("irc")->info(Utils::string_format("Sending '%s' to '%s'", message.c_str(), target.c_str()));

    // Try to send a message
    if (irc_cmd_msg(session_, target.c_str(), message.c_str()))
        // Throw on error
        Utils::throw_error("IrcClient", "send", Utils::string_format("Impossible to send the message: %s", irc_strerror(irc_errno(session_))));
}

void IrcClient::join(const std::string& channel)
{
    // We should have a session
    assert(session_);

    Meow("irc")->info(Utils::string_format("Joining channel %s", channel.c_str()));

    // Try to join the channel
    if (irc_cmd_join(session_, channel.c_str(), NULL))
        // Throw on error
        Utils::throw_error("IrcClient", "join", Utils::string_format("Impossible to join the channel: %s", irc_strerror(irc_errno(session_))));
}
