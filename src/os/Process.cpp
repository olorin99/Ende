#include <csignal>
#include "Ende/os/Process.h"
#include <iostream>

ende::os::Process::Process(const std::string &cmd)
    : _cmd(cmd)
{}

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

ende::os::Process& ende::os::Process::operator=(Process &&process) noexcept {
    std::swap(_pipes, process._pipes);
    std::swap(_info, process._info);
    std::swap(_forked, process._forked);
    std::swap(_cmd, process._cmd);
    std::swap(_args, process._args);
    return *this;
}


auto ende::os::Process::arg(const std::string &arg) -> Process& {
    _args.push_back(arg);
    return *this;
}

auto ende::os::Process::args(std::span<std::string> args) -> Process& {
    for (auto& arg : args)
        _args.push_back(arg);
    return *this;
}

auto ende::os::Process::id() const -> i32 {
    return _info.id;
}

auto ende::os::Process::fork() -> Process& {
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

auto ende::os::Process::kill() -> bool{
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
    for (auto& arg : _args)
        std::cout << arg << ' ';
}