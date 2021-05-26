//
// Created by cstro29 on 26/5/21.
//

#include "Ende/sys/pipe.h"
#include <unistd.h>

ende::Optional<ende::sys::Pipe> ende::sys::pipe() {
    i32 pfd[2];
    i32 ret = ::pipe2(pfd, 0);
    if (ret == -1)
        return None;
    return Some(Pipe{FileDesc(pfd[0]), FileDesc(pfd[1])});
}