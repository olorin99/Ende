//
// Created by cstro29 on 26/5/21.
//

#ifndef ENDE_SYS_PROCESS_H
#define ENDE_SYS_PROCESS_H

#include <Ende/platform.h>
#include <Ende/sys/FileDesc.h>
#include <Ende/sys/pipe.h>
#include <vector>
#include <expected>

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

    auto spawn(const std::string& cmd, const std::vector<std::string>& args, Pipes& pipes) -> std::expected<ProcessInfo, int>;

    auto wait(ProcessInfo& info, u32 flags) -> bool;

    auto kill(ProcessInfo& info, i32 signal) -> bool;

}

#endif //ENDE_SYS_PROCESS_H
