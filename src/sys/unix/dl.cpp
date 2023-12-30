#include "Ende/sys/dl.h"
#include <dlfcn.h>

auto ende::sys::dl::open(const std::string &path, i32 flags) -> void* {
    return ::dlopen(path.c_str(), flags);
}

auto ende::sys::dl::symbol(void *handle, const std::string &name) -> void* {
    if (!handle) return nullptr;
    return ::dlsym(handle, name.c_str());
}

auto ende::sys::dl::close(void *handle) -> bool {
    return ::dlclose(handle) == 0;
}

auto ende::sys::dl::error() -> std::string {
    return std::string(::dlerror());
}