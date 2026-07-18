module;

#include <Ende/platform.h>
#include <expected>
#include <filesystem>

export module ende.os.DynamicLib;

import ende.system.dl;
import ende.util;

namespace ende::os {

export class DynamicLib {
  public:
    DynamicLib() = default;

    ~DynamicLib();

    DynamicLib(DynamicLib &&lib) noexcept;
    DynamicLib &operator=(DynamicLib &&lib) noexcept;

    static auto open(const std::string &path, i32 flags) -> std::expected<DynamicLib, std::string>;

    auto close() -> bool;

    auto error() const -> std::string_view {
        return _error;
    }

    template <typename T>
    T symbol(const std::string &name) {
        void *sym = sys::dl::symbol(_address, name);
        if (!sym) {
            _error = sys::dl::error();
            return nullptr;
        }
        return reinterpret_cast<T>(sym);
    }

  private:
    void *_address = nullptr;
    std::string _error = {};
    std::filesystem::path _path = {};
    i32 _flags = 0;
};

}

ende::os::DynamicLib::~DynamicLib() {
    close();
}

ende::os::DynamicLib::DynamicLib(DynamicLib &&lib) noexcept {
    std::swap(_address, lib._address);
    std::swap(_error, lib._error);
    std::swap(_path, lib._path);
    std::swap(_flags, lib._flags);
}

ende::os::DynamicLib &ende::os::DynamicLib::operator=(DynamicLib &&lib) noexcept {
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
