#ifndef ENDE_SYSTEM_H
#define ENDE_SYSTEM_H

#include <Ende/platform.h>
#include <Ende/sys/time.h>
#include <string>

namespace ende::sys {

    void sleep(TimeSpec time);

    auto demangle(const char* name) -> std::string;

}

#endif //ENDE_SYSTEM_H
