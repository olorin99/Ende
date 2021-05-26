//
// Created by cstro29 on 26/5/21.
//

#include "Ende/os/DynamicLib.h"

ende::os::DynamicLib::DynamicLib()
    : _address(nullptr),
    _error(""),
    _path(""),
    _flags(0)
{}

ende::os::DynamicLib::DynamicLib(const std::string &path, i32 flags) {
    open(path, flags);
}

ende::os::DynamicLib::DynamicLib(const DynamicLib &lib) {
    open(lib._path, lib._flags);
}

ende::os::DynamicLib::DynamicLib(DynamicLib &&lib) noexcept {
    std::swap(_address, lib._address);
    std::swap(_error, lib._error);
    std::swap(_path, lib._path);
    std::swap(_flags, lib._flags);
}

ende::os::DynamicLib & ende::os::DynamicLib::operator=(DynamicLib &&lib) noexcept {
    std::swap(_address, lib._address);
    std::swap(_error, lib._error);
    std::swap(_path, lib._path);
    std::swap(_flags, lib._flags);
    return *this;
}


bool ende::os::DynamicLib::open(const std::string &path, i32 flags) {
    if (_address)
        close();
    _address = sys::dl::open(path, flags);
    if (!_address) {
        _error = sys::dl::error();
        return false;
    }
    return true;
}

bool ende::os::DynamicLib::close() {
    if (!_address)
        return false;
    return sys::dl::close(_address);
}

std::string ende::os::DynamicLib::error() const {
    return _error;
}