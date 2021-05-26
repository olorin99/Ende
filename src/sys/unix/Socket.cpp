//
// Created by cstro29 on 26/5/21.
//

#include "Ende/sys/Socket.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>


ende::sys::AddressInfo ende::sys::getAddressInfo(const std::string &node, const std::string &service) {
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
    detach();
}

ende::sys::Socket::Socket(const Socket &sock)
    : _socket(sock._socket)
{}

ende::sys::Socket::Socket(Socket &&sock) noexcept {
    std::swap(_socket, sock._socket);
    std::swap(_info, sock._info);
}

bool ende::sys::Socket::open(const AddressInfo &info) {
    i32 yes = 1;
    _socket = ::socket(info.ai_family, _info.ai_socktype, _info.ai_protocol);
    _info = info;
    ::setsockopt(*_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
    return true;
}

bool ende::sys::Socket::close() {
    return ::close(*_socket) == 0;
}

bool ende::sys::Socket::bind(const Address &address, u32 length) {
    return ::bind(*_socket, &address, length) == 0;
}

bool ende::sys::Socket::connect() {
    return ::connect(*_socket, _info.ai_addr, _info.ai_addrlen) ==0;
}

bool ende::sys::Socket::listen(u32 backlog) {
    return ::listen(*_socket, backlog) == 0;
}

ende::sys::Socket ende::sys::Socket::accept() {
    Address address;
    u32 len = sizeof(address);
    return ::accept(*_socket, &address, &len);
}

i32 ende::sys::Socket::send(Span<const char> buffer) {
    return static_cast<i32>(::send(*_socket, buffer.data(), buffer.size(), 0));
}

i32 ende::sys::Socket::receive(Span<char> buffer) {
    return static_cast<i32>(::recv(*_socket, buffer.data(), buffer.size(), 0));
}

bool ende::sys::Socket::detach() {
    if (_socket.unique())
        close();
    return _socket.detach();
}

bool ende::sys::Socket::valid() const {
    return ::fcntl(*_socket, F_GETFD) != -1;
}

i32 ende::sys::Socket::handle() const {
    return *_socket;
}

ende::sys::Address * ende::sys::Socket::address() const {
    return _info.ai_addr;
}

bool ende::sys::getPeerName(const Socket &socket, Address &address) {
    u32 len = sizeof(address);
    return ::getpeername(socket.handle(), &address, &len) == 0;
}

std::string ende::sys::getHostName() {
    std::string buffer;
    buffer.resize(10);
    if (::gethostname(buffer.data(), buffer.size()) != 0)
        return {};
    return buffer;
}