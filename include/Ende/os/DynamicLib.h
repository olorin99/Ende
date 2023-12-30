#ifndef ENDE_DYNAMICLIB_H
#define ENDE_DYNAMICLIB_H

#include <Ende/platform.h>
#include <Ende/sys/dl.h>
#include <filesystem>
#include <expected>

namespace ende::os {

    class DynamicLib {
    public:

        DynamicLib() = default;

        ~DynamicLib();

        DynamicLib(DynamicLib&& lib) noexcept;
        DynamicLib& operator=(DynamicLib&& lib) noexcept;

        static auto open(const std::string& path, i32 flags) -> std::expected<DynamicLib, std::string>;

        auto close() -> bool;

        auto error() const -> std::string_view {
            return _error;
        }

        template <typename T>
        T symbol(const std::string& name) {
            void* sym = sys::dl::symbol(_address, name);
            if (!sym) {
                _error = sys::dl::error();
                return nullptr;
            }
            return reinterpret_cast<T>(sym);
        }

    private:

        void* _address = nullptr;
        std::string _error = {};
        std::filesystem::path _path = {};
        i32 _flags = 0;

    };

}

#endif //ENDE_DYNAMICLIB_H
