module;

#include <unistd.h>
#include <Ende/platform.h>
#include <optional>

export module ende.system.pipe;

import ende.system.FileDesc;
import ende.util;

namespace ende::sys {

export struct Pipe {
    FileDesc input;
    FileDesc output;
};

export auto pipe() -> std::optional<Pipe>;

}

auto ende::sys::pipe() -> std::optional<ende::sys::Pipe> {
    i32 pfd[2];
    i32 ret = ::pipe2(pfd, 0);
    if (ret == -1)
        return {};
    return Pipe{FileDesc(pfd[0]), FileDesc(pfd[1])};
}
