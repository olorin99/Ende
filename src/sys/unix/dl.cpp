//
// Created by cstro29 on 26/5/21.
//

#include "Ende/sys/dl.h"
#include <dlfcn.h>

void* ende::sys::dl::open(const std::string &path, i32 flags) {
    return ::dlopen(path.c_str(), flags);
}

void* ende::sys::dl::symbol(void *handle, const std::string &name) {
    if (!handle) return nullptr;
    return ::dlsym(handle, name.c_str());
}

bool ende::sys::dl::close(void *handle) {
    return ::dlclose(handle) == 0;
}

std::string ende::sys::dl::error() {
    return std::string(::dlerror());
}