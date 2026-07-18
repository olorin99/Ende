module;

#include <cstring>
#include <unistd.h>
#include <wait.h>

#include <Ende/platform.h>
#include <expected>
#include <string>
#include <vector>

export module ende.system.process;

import ende.system.FileDesc;
import ende.system.pipe;
import ende.util;

namespace ende::sys {

export struct ProcessInfo {
    i32 id;
    i32 ret;
    FileDesc process;
    FileDesc thread;
};

export struct Pipes {
    Pipe stdin;
    Pipe stdout;
    Pipe stderr;
};

export auto spawn(const std::string &cmd, const std::vector<std::string> &args, Pipes &pipes) -> std::expected<ProcessInfo, int>;

export auto wait(ProcessInfo &info, u32 flags) -> bool;

export auto kill(ProcessInfo &info, i32 signal) -> bool;

}

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

    char *execArgs[1024];
    int argCount = 0;
    execArgs[argCount++] = strdup(cmd.c_str());
    for (auto &arg : args)
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
