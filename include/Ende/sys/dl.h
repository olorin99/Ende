#ifndef ENDE_DL_H
#define ENDE_DL_H

#include <Ende/platform.h>
#include <string>

namespace ende::sys::dl {

    auto open(const std::string& path, i32 flags) -> void*;

    auto symbol(void* handle, const std::string& name) -> void*;

    auto close(void* handle) -> bool;

    auto error() -> std::string;

}

#endif //ENDE_DL_H
