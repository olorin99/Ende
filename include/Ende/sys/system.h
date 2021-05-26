//
// Created by cstro29 on 19/5/21.
//

#ifndef ENDE_SYSTEM_H
#define ENDE_SYSTEM_H

#include <Ende/platform.h>
#include <Ende/sys/time.h>
#include <string>

namespace ende::sys {

    void sleep(TimeSpec time);

    std::string demangle(const char* name);

}

#endif //ENDE_SYSTEM_H
