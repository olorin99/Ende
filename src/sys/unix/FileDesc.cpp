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



auto ende::sys::FileDesc::handle() const -> const FileDesc::Handle& {
    return _handle;
}

auto ende::sys::FileDesc::valid() const -> bool {
    return ::fcntl(_handle, F_GETFD) != -1;
}


auto ende::sys::FileDesc::read(std::span<char> buffer) const -> i32 {
    return static_cast<i32>(::read(_handle, buffer.data(), buffer.size()));
}

auto ende::sys::FileDesc::read() const -> std::string {
    std::string buffer(size(), '\0');
    read(buffer);
    return buffer;
}

auto ende::sys::FileDesc::write(std::span<const char> buffer) const -> i32 {
    return static_cast<i32>(::write(_handle, buffer.data(), buffer.size()));
}


auto ende::sys::FileDesc::dup(const FileDesc &fd) const -> bool {
    return ::dup2(_handle, fd._handle) != -1;
}

auto ende::sys::FileDesc::flush() const -> bool {
    return ::fsync(_handle) == 0;
}

auto ende::sys::FileDesc::size() const -> u64 {
    u64 bytes = 0;
    i32 err = ::ioctl(_handle, FIONREAD, &bytes);
    return err < 0 ? 0 : bytes;
}


auto ende::sys::FileDesc::stdin() -> const FileDesc& {
    static const FileDesc fd(STDIN_FILENO);
    return fd;
}

auto ende::sys::FileDesc::stdout() -> const FileDesc& {
    static const FileDesc fd(STDOUT_FILENO);
    return fd;
}

auto ende::sys::FileDesc::stderr() -> const FileDesc& {
    static const FileDesc fd(STDERR_FILENO);
    return fd;
}



auto ende::sys::open(const std::string &path, u32 flags, u32 perms) -> FileDesc {
    auto fd = ::open(path.c_str(), flags, perms);
    return FileDesc(fd);
}

auto ende::sys::close(FileDesc &fd) -> bool {
    bool ret = ::close(fd._handle) == 0;
    fd._handle = -1;
    return ret;
}