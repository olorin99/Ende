#ifndef ENDE_SYSTEM_H
#define ENDE_SYSTEM_H

#include <Ende/platform.h>
#include <string>

namespace ende::sys {

    auto demangle(const char* name) -> std::string;

}

#endif //ENDE_SYSTEM_H
