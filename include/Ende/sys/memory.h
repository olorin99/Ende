//
// Created by cstro29 on 19/5/21.
//

#ifndef ENDE_MEMORY_H
#define ENDE_MEMORY_H

#include <Ende/platform.h>
#include <new>
#include <cstdlib>

namespace ende::sys {

    template <typename T>
    T* malloc(u64 count) {
        return static_cast<T*>(std::malloc(sizeof(T) * count));
    }

    template <typename T>
    T* realloc(T* src, u64 count) {
        return static_cast<T*>(std::realloc(src, sizeof(T) * count));
    }

}

#endif //ENDE_MEMORY_H
