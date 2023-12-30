#ifndef ENDE_SOCKET_H
#define ENDE_SOCKET_H

#include <Ende/platform.h>
#include <span>
#include <string>
#include <memory>

#ifdef ENDE_UNIX
#include <netdb.h>
#include <sys/socket.h>

#endif

namespace ende::sys {

    typedef sockaddr Address;
    typedef addrinfo AddressInfo;

    auto getAddressInfo(const std::string& node, const std::string& service) -> AddressInfo;

    class Socket {
    public:

        Socket();

        Socket(i32 fd);

        ~Socket();

        Socket(Socket&& sock) noexcept;

        auto open(const AddressInfo& info) -> bool;

        auto close() -> bool;

        auto bind(const Address& address, u32 length) -> bool;

        auto connect() -> bool;

        auto listen(u32 backlog) -> bool;

        auto accept() -> Socket;

        auto send(std::span<const char> buffer) -> i32;

        auto receive(std::span<char> buffer) -> i32;

        auto valid() const -> bool;

        auto handle() const -> i32;

        auto address() const -> Address*;

    private:

        i32 _socket;
        AddressInfo _info;

    };

    auto getPeerName(const Socket& socket, Address& address) -> bool;

    auto getHostName() -> std::string;

}

#endif //ENDE_SOCKET_H
