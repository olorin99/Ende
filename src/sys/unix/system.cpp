//
// Created by cstro29 on 19/5/21.
//

#include "Ende/sys/system.h"
#include <unistd.h>
#include <cxxabi.h>

void ende::sys::sleep(TimeSpec time) {
    i64 micro = time.sec * 1000000 + time.nano / 1000;
    usleep(micro);
}

std::string ende::sys::demangle(const char *name) {
    i32 status = 0;
    char* demangled = abi::__cxa_demangle(name, nullptr, nullptr, &status);
    std::string n = status == 0 && demangled ? demangled : name;
    free(demangled);
    return n;
}