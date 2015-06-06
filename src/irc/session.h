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

#ifndef IRC_SESSION_H_
#define IRC_SESSION_H_

#include "irc/socket.h"

namespace Irc
{
    // The different session states
    enum State
    {
        kConnecting,
        kConnected
    };

    // The session object
    class Session
    {
    private:
        // The socket
        Socket socket_;

        // The current state
        State state_;

    public:
        // Constructor
        Session();

        // Destructor
        ~Session();

        // Socket getter
        inline const Socket& socket() const
        {
            return socket_;
        }

        // State getter
        inline const State& state() const
        {
            return state_;
        }

        // Connects the session to the given host and the given port
        void connect(const std::string& address, int port);

        // Disconnects the session
        void disconnect();

        // Sends the given message
        void send(const std::string& message);

        // Receives a message
        std::string receive();

        // 
        void add_select_descriptors();

        void process_select_descriptors();

    };
}

#endif // IRC_SESSION_H_
