//
// Created by cstro29 on 26/5/21.
//

#ifndef ENDE_SOCKET_H
#define ENDE_SOCKET_H

#include <Ende/platform.h>
#include <Ende/Span.h>
#include <Ende/Shared.h>
#include <string>

#ifdef ENDE_UNIX
#include <netdb.h>
#include <sys/socket.h>
#endif

namespace ende::sys {

    typedef sockaddr Address;
    typedef addrinfo AddressInfo;

    AddressInfo getAddressInfo(const std::string& node, const std::string& service);

    class Socket {
    public:

        Socket();

        Socket(i32 fd);

        ~Socket();

        Socket(const Socket& sock);

        Socket(Socket&& sock) noexcept;

        bool open(const AddressInfo& info);

        bool close();

        bool bind(const Address& address, u32 length);

        bool connect();

        bool listen(u32 backlog);

        Socket accept();


        i32 send(Span<const char> buffer);

        i32 receive(Span<char> buffer);


        bool detach();

        bool valid() const;

        i32 handle() const;

        Address* address() const;

    private:

        Shared<i32> _socket;
        AddressInfo _info;

    };

    bool getPeerName(const Socket& socket, Address& address);

    std::string getHostName();

}

#endif //ENDE_SOCKET_H
