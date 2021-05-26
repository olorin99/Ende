//
// Created by cstro29 on 26/5/21.
//

#ifndef ENDE_DL_H
#define ENDE_DL_H

#include <Ende/platform.h>
#include <string>

namespace ende::sys::dl {

    void* open(const std::string& path, i32 flags);

    void* symbol(void* handle, const std::string& name);

    bool close(void* handle);

    std::string error();

}

#endif //ENDE_DL_H
