//
// Created by cstro29 on 19/5/21.
//

#include "Ende/sys/FileDesc.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

ende::sys::FileDesc::FileDesc()
    : _handle(-1),
    _flags(0)
{}

ende::sys::FileDesc::FileDesc(Handle handle)
    : _handle(handle),
    _flags(0)
{}

ende::sys::FileDesc::FileDesc(const FileDesc &fd)
    : _handle(fd._handle),
    _flags(fd._flags)
{}

ende::sys::FileDesc::FileDesc(FileDesc &&fd) noexcept
    : _handle(fd._handle),
    _flags(fd._flags)
{
    fd._handle = -1;
    fd._flags = 0;
}

ende::sys::FileDesc & ende::sys::FileDesc::operator=(const FileDesc &fd) {
    _handle = fd._handle;
    _flags = fd._flags;
    return *this;
}

ende::sys::FileDesc & ende::sys::FileDesc::operator=(FileDesc &&fd) noexcept {
    _handle = fd._handle;
    _flags = fd._flags;
    fd._handle = -1;
    fd._flags = 0;
    return *this;
}



const ende::sys::FileDesc::Handle & ende::sys::FileDesc::handle() const {
    return _handle;
}

bool ende::sys::FileDesc::valid() const {
    return ::fcntl(_handle, F_GETFD) != -1;
}


i32 ende::sys::FileDesc::read(ende::Span<char> buffer) const {
    return static_cast<i32>(::read(_handle, buffer.data(), buffer.size()));
}

std::string ende::sys::FileDesc::read() const {
    std::string buffer(size(), '\0');
    read(buffer);
    return buffer;
}

i32 ende::sys::FileDesc::write(ende::Span<char> buffer) const {
    return static_cast<i32>(::write(_handle, buffer.data(), buffer.size()));
}


bool ende::sys::FileDesc::dup(const FileDesc &fd) const {
    return ::dup2(_handle, fd._handle) != -1;
}

bool ende::sys::FileDesc::flush() const {
    return ::fsync(_handle) == 0;
}

u64 ende::sys::FileDesc::size() const {
    u64 bytes = 0;
    i32 err = ::ioctl(_handle, FIONREAD, &bytes);
    return err < 0 ? 0 : bytes;
}


const ende::sys::FileDesc& ende::sys::FileDesc::stdin() {
    static const FileDesc fd(STDIN_FILENO);
    return fd;
}

const ende::sys::FileDesc& ende::sys::FileDesc::stdout() {
    static const FileDesc fd(STDOUT_FILENO);
    return fd;
}

const ende::sys::FileDesc& ende::sys::FileDesc::stderr() {
    static const FileDesc fd(STDERR_FILENO);
    return fd;
}



ende::sys::FileDesc ende::sys::open(const std::string &path, u32 flags, u32 perms) {
    auto fd = ::open(path.c_str(), flags, perms);
    return FileDesc(fd);
}

bool ende::sys::close(FileDesc &fd) {
    bool ret = ::close(fd._handle) == 0;
    fd._handle = -1;
    return ret;
}