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

#ifndef IRC_CLIENT_H_
#define IRC_CLIENT_H_

#include "libircclient.h"
#include "libirc_rfcnumeric.h"

#include <string>

// Represents an irc client
class IrcClient
{
private:
    // The irc session
    irc_session_t* session_;

    // The irc callbacks
    irc_callbacks_t callbacks_;

    // The server
    std::string server_;

    // The port
    unsigned int port_;

    // The server password
    std::string password_;

    // The nick
    std::string nick_;

    // The username
    std::string username_;

    // The real name
    std::string realname_;

    // If we should use ssl
    bool ssl_;

public:
    // Constructor
    IrcClient();

    // Destructor
    ~IrcClient();

    // Connects the client using the given settings
    void connect();

    // Stops the client
    void stop();

    // Sends a message to the given target
    void send(const std::string& target, const std::string& message);

    // Joins a given channel
    void join(const std::string& channel);

    // Callbacks getter
    inline const irc_callbacks_t& callbacks() const
    {
        return callbacks_;
    }

    // Session getter
    inline irc_session_t* session() const
    {
        return session_;
    }

    // Checks if the server is connected
    inline bool connected()
    {
        return irc_is_connected(session_) == 1;
    }

    // Server getter
    inline const std::string& server() const
    {
        return server_;
    }

    // Server setter
    inline void set_server(const std::string& server)
    {
        server_ = server;
    }

    // Port getter
    inline unsigned int port() const
    {
        return port_;
    }

    // Port setter
    inline void set_port(unsigned int port)
    {
        port_ = port;
    }

    // Password getter
    inline const std::string& password() const
    {
        return password_;
    }

    // Password setter
    inline void set_password(const std::string& password)
    {
        password_ = password;
    }

    // Nick getter
    inline const std::string& nick() const
    {
        return nick_;
    }

    // Nick setter
    inline void set_nick(const std::string& nick)
    {
        nick_ = nick;
    }

    // Username getter
    inline const std::string& username() const
    {
        return username_;
    }

    // Username setter
    inline void set_username(const std::string& username)
    {
        username_ = username;
    }

    // Realname getter
    inline const std::string& realname() const
    {
        return realname_;
    }

    // Realname setter
    inline void set_realname(const std::string& realname)
    {
        realname_ = realname;
    }

    // SSL getter
    inline bool ssl() const
    {
        return ssl_;
    }

    // SSL setter
    inline void set_ssl(bool ssl)
    {
        ssl_ = ssl;
    }

    // SSL Verify setter
    inline void set_ssl_verify(bool verify)
    {
        if (verify)
            irc_option_reset(session_, LIBIRC_OPTION_SSL_NO_VERIFY);
        else
            irc_option_set(session_, LIBIRC_OPTION_SSL_NO_VERIFY);
    }

    // Get the latest session error
    inline const char* get_error() const
    {
        return irc_strerror(irc_errno(session_));
    }

    // Connect handler
    virtual void on_connect() = 0;

    // Channel message handler
    virtual void on_channel(const std::string& sender, const std::string& channel, const std::string& message) = 0;
};

#endif // IRC_CLIENT_H_
