//
// Created by cstro29 on 26/5/21.
//

#ifndef ENDE_DYNAMICLIB_H
#define ENDE_DYNAMICLIB_H

#include <Ende/platform.h>
#include <Ende/sys/dl.h>

namespace ende::os {

    class DynamicLib {
    public:

        DynamicLib();

        DynamicLib(const std::string& path, i32 flags);

        DynamicLib(const DynamicLib& lib);

        DynamicLib(DynamicLib&& lib) noexcept;

        DynamicLib& operator=(DynamicLib&& lib) noexcept;


        bool open(const std::string& path, i32 flags);

        bool close();

        std::string error() const;

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

        void* _address;
        std::string _error;
        std::string _path;
        i32 _flags;

    };

}

#endif //ENDE_DYNAMICLIB_H
