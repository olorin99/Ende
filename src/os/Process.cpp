module;

#include <csignal>
#include <iostream>

#include <Ende/platform.h>
#include <span>
#include <vector>

export module ende.os.Process;

import ende.system.pipe;
import ende.system.process;
import ende.system.FileDesc;
import ende.util;

namespace ende::os {

export class Process {
  public:
    Process(const std::string &cmd);

    ~Process();

    Process(const Process &process) = delete;

    Process(Process &&process) noexcept;

    Process &operator=(const Process &process) = delete;

    Process &operator=(Process &&process) noexcept;

    auto arg(const std::string &arg) -> Process &;

    auto args(std::span<std::string> args) -> Process &;

    auto id() const -> i32;

    auto fork() -> Process &;

    auto wait() -> i32;

    auto kill() -> bool;

    auto stdin() const -> sys::Pipe;

    auto stdout() const -> sys::Pipe;

    auto stderr() const -> sys::Pipe;

    void printArgs() const;

  private:
    sys::Pipes _pipes;
    sys::ProcessInfo _info;
    bool _forked;
    std::string _cmd;
    std::vector<std::string> _args;
};

}

ende::os::Process::Process(const std::string &cmd)
    : _cmd(cmd) {}

ende::os::Process::~Process() {
    wait();
}

ende::os::Process::Process(Process &&process) noexcept {
    std::swap(_pipes, process._pipes);
    std::swap(_info, process._info);
    std::swap(_forked, process._forked);
    std::swap(_cmd, process._cmd);
    std::swap(_args, process._args);
}

ende::os::Process &ende::os::Process::operator=(Process &&process) noexcept {
    std::swap(_pipes, process._pipes);
    std::swap(_info, process._info);
    std::swap(_forked, process._forked);
    std::swap(_cmd, process._cmd);
    std::swap(_args, process._args);
    return *this;
}

auto ende::os::Process::arg(const std::string &arg) -> Process & {
    _args.push_back(arg);
    return *this;
}

auto ende::os::Process::args(std::span<std::string> args) -> Process & {
    for (auto &arg : args)
        _args.push_back(arg);
    return *this;
}

auto ende::os::Process::id() const -> i32 {
    return _info.id;
}

auto ende::os::Process::fork() -> Process & {
    _pipes.stdin = sys::pipe().value();
    _pipes.stdout = sys::pipe().value();
    _pipes.stderr = sys::pipe().value();

    auto res = sys::spawn(_cmd, _args, _pipes);

    if (res) {
        _info = res.value();
        _forked = true;
    } else
        _forked = false;

    return *this;
}

auto ende::os::Process::wait() -> i32 {
    if (!_forked)
        return -1;
    sys::close(_pipes.stdin.input);
    sys::close(_pipes.stdin.output);
    sys::wait(_info, 0);
    return _info.ret;
}

auto ende::os::Process::kill() -> bool {
    return sys::kill(_info, SIGKILL);
}

auto ende::os::Process::stdin() const -> ende::sys::Pipe {
    return _pipes.stdin;
}

auto ende::os::Process::stdout() const -> ende::sys::Pipe {
    return _pipes.stdout;
}

auto ende::os::Process::stderr() const -> ende::sys::Pipe {
    return _pipes.stderr;
}

void ende::os::Process::printArgs() const {
    for (auto &arg : _args)
        std::cout << arg << ' ';
}
