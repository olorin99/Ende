#include "Ende/os/DynamicLib.h"

ende::os::DynamicLib::~DynamicLib() {
    close();
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

auto ende::os::DynamicLib::open(const std::string &path, i32 flags) -> std::expected<DynamicLib, std::string> {
    DynamicLib lib = {};
    lib._address = sys::dl::open(path, flags);
    if (!lib._address) {
        lib._error = sys::dl::error();
        return std::unexpected(lib._error);
    }
    return lib;
}

auto ende::os::DynamicLib::close() -> bool {
    if (!_address)
        return false;
    return sys::dl::close(_address);
}