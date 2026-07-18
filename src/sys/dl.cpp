module;

#include <dlfcn.h>
#include <Ende/platform.h>
#include <string>

export module ende.system.dl;

import ende.util;

namespace ende::sys::dl {

export auto open(const std::string &path, i32 flags) -> void *;

export auto symbol(void *handle, const std::string &name) -> void *;

export auto close(void *handle) -> bool;

export auto error() -> std::string;

}

auto ende::sys::dl::open(const std::string &path, i32 flags) -> void * {
    return ::dlopen(path.c_str(), flags);
}

auto ende::sys::dl::symbol(void *handle, const std::string &name) -> void * {
    if (!handle)
        return nullptr;
    return ::dlsym(handle, name.c_str());
}

auto ende::sys::dl::close(void *handle) -> bool {
    return ::dlclose(handle) == 0;
}

auto ende::sys::dl::error() -> std::string {
    return std::string(::dlerror());
}
