module;

#include <cxxabi.h>
#include <Ende/platform.h>
#include <string>

import ende.util;

export module ende.system.system;

namespace ende::sys {

export auto demangle(const char *name) -> std::string;

}

auto ende::sys::demangle(const char *name) -> std::string {
    i32 status = 0;
    char *demangled = abi::__cxa_demangle(name, nullptr, nullptr, &status);
    std::string n = status == 0 && demangled ? demangled : name;
    free(demangled);
    return n;
}
