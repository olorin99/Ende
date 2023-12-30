#include "Ende/sys/pipe.h"
#include <unistd.h>

auto ende::sys::pipe() -> std::optional<ende::sys::Pipe> {
    i32 pfd[2];
    i32 ret = ::pipe2(pfd, 0);
    if (ret == -1)
        return {};
    return Pipe{FileDesc(pfd[0]), FileDesc(pfd[1])};
}