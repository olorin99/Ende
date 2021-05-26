//
// Created by cstro29 on 26/5/21.
//

#include "Ende/sys/process.h"
#include <unistd.h>
#include <wait.h>
#include <cstring>

ende::Result<ende::sys::ProcessInfo> ende::sys::spawn(const std::string &cmd, const Vector<std::string> &args, Pipes &pipes) {
    i32 pid = ::fork();
    switch (pid) {
        case 0:
            break;
        default:
            close(pipes.stdin.output);
            close(pipes.stdout.output);
            close(pipes.stderr.output);
            return Ok(ProcessInfo{pid});
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
    return Err();
}

bool ende::sys::wait(ProcessInfo &info, u32 flags) {
    i32 procStat;
    pid_t ret = ::waitpid(info.id, &procStat, flags);
    bool finished = (ret && (WIFEXITED(procStat) || WIFSIGNALED(procStat)));
    if (finished)
        info.ret = WEXITSTATUS(procStat);
    return finished;
}

bool ende::sys::kill(ProcessInfo &info, i32 signal) {
    return ::kill(info.id, signal) == 0;
}