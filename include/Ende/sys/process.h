//
// Created by cstro29 on 26/5/21.
//

#ifndef ENDE_PROCESS_H
#define ENDE_PROCESS_H

#include <Ende/platform.h>
#include <Ende/sys/FileDesc.h>
#include <Ende/sys/pipe.h>
#include <Ende/Result.h>
#include <Ende/Vector.h>

namespace ende::sys {

    struct ProcessInfo {
        i32 id;
        i32 ret;
        FileDesc process;
        FileDesc thread;
    };

    struct Pipes {
        Pipe stdin;
        Pipe stdout;
        Pipe stderr;
    };

    Result<ProcessInfo> spawn(const std::string& cmd, const Vector<std::string>& args, Pipes& pipes);

    bool wait(ProcessInfo& info, u32 flags);

    bool kill(ProcessInfo& info, i32 signal);

}

#endif //ENDE_PROCESS_H
