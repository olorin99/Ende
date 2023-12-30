#ifndef ENDE_PIPE_H
#define ENDE_PIPE_H

#include <Ende/platform.h>
#include <Ende/sys/FileDesc.h>
#include <optional>

namespace ende::sys {

    struct Pipe {
        FileDesc input;
        FileDesc output;
    };

    auto pipe() -> std::optional<Pipe>;

}

#endif //ENDE_PIPE_H
