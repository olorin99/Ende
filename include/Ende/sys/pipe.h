//
// Created by cstro29 on 26/5/21.
//

#ifndef ENDE_PIPE_H
#define ENDE_PIPE_H

#include <Ende/platform.h>
#include <Ende/sys/FileDesc.h>
#include <Ende/Optional.h>

namespace ende::sys {

    struct Pipe {
        FileDesc input;
        FileDesc output;
    };

    Optional<Pipe> pipe();

}

#endif //ENDE_PIPE_H
