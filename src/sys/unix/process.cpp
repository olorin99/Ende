//
// Created by cstro29 on 26/5/21.
//

#include "Ende/sys/process.h"
#include <unistd.h>
#include <wait.h>
#include <cstring>

auto ende::sys::spawn(const std::string &cmd, const std::vector<std::string> &args, Pipes &pipes) -> std::expected<ende::sys::ProcessInfo, int> {
    i32 pid = ::fork();
    switch (pid) {
        case 0:
            break;
        default:
            close(pipes.stdin.output);
            close(pipes.stdout.output);
            close(pipes.stderr.output);
            return ProcessInfo{pid};
    }
    pipes.stdin.output.dup(FileDesc::stdin());
    pipes.stdout.output.dup(FileDesc::stdout());
    pipes.stderr.output.dup(FileDesc::stderr());

    char* execArgs[1024];
    int argCount = 0;
    execArgs[argCount++] = strdup(cmd.c_str());
    for (auto& arg : args)
        execArgs[argCount++] = strdup(arg.c_str());
    execvp(cmd.c_str(), execArgs);
    return std::unexpected(-1);
}

auto ende::sys::wait(ProcessInfo &info, u32 flags) -> bool {
    i32 procStat;
    pid_t ret = ::waitpid(info.id, &procStat, flags);
    bool finished = (ret && (WIFEXITED(procStat) || WIFSIGNALED(procStat)));
    if (finished)
        info.ret = WEXITSTATUS(procStat);
    return finished;
}

auto ende::sys::kill(ProcessInfo &info, i32 signal) -> bool {
    return ::kill(info.id, signal) == 0;
}