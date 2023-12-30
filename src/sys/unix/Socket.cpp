#include "Ende/sys/Socket.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>


auto ende::sys::getAddressInfo(const std::string &node, const std::string &service) -> AddressInfo {
    struct addrinfo hints;
    struct addrinfo* res;

    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (::getaddrinfo(node.c_str(), service.c_str(), &hints, &res) != 0)
        return {};

    AddressInfo ret = *res;
    ::freeaddrinfo(res);
    return ret;
}

ende::sys::Socket::Socket()
    : _socket(-1)
{}

ende::sys::Socket::Socket(i32 fd)
    : _socket(fd)
{}

ende::sys::Socket::~Socket() {
    close();
}

ende::sys::Socket::Socket(Socket &&sock) noexcept {
    std::swap(_socket, sock._socket);
    std::swap(_info, sock._info);
}

auto ende::sys::Socket::open(const AddressInfo &info) -> bool {
    i32 yes = 1;
    _socket = ::socket(info.ai_family, _info.ai_socktype, _info.ai_protocol);
    _info = info;
    ::setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
    return true;
}

auto ende::sys::Socket::close() -> bool {
    return ::close(_socket) == 0;
}

auto ende::sys::Socket::bind(const Address &address, u32 length) -> bool {
    return ::bind(_socket, &address, length) == 0;
}

auto ende::sys::Socket::connect() -> bool {
    return ::connect(_socket, _info.ai_addr, _info.ai_addrlen) ==0;
}

auto ende::sys::Socket::listen(u32 backlog) -> bool {
    return ::listen(_socket, backlog) == 0;
}

auto ende::sys::Socket::accept() -> Socket {
    Address address;
    u32 len = sizeof(address);
    return ::accept(_socket, &address, &len);
}

auto ende::sys::Socket::send(std::span<const char> buffer) -> i32 {
    return static_cast<i32>(::send(_socket, buffer.data(), buffer.size(), 0));
}

auto ende::sys::Socket::receive(std::span<char> buffer) -> i32 {
    return static_cast<i32>(::recv(_socket, buffer.data(), buffer.size(), 0));
}

auto ende::sys::Socket::valid() const -> bool {
    return ::fcntl(_socket, F_GETFD) != -1;
}

auto ende::sys::Socket::handle() const -> i32 {
    return _socket;
}

auto ende::sys::Socket::address() const -> Address* {
    return _info.ai_addr;
}

auto ende::sys::getPeerName(const Socket &socket, Address &address) -> bool {
    u32 len = sizeof(address);
    return ::getpeername(socket.handle(), &address, &len) == 0;
}

auto ende::sys::getHostName() -> std::string {
    std::string buffer;
    buffer.resize(10);
    if (::gethostname(buffer.data(), buffer.size()) != 0)
        return {};
    return buffer;
}