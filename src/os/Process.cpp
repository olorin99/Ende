//
// Created by cstro29 on 26/5/21.
//

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


ende::os::Process & ende::os::Process::arg(const std::string &arg) {
    _args.push(arg);
}

ende::os::Process & ende::os::Process::args(Span<std::string> args) {
    _args.insert(_args.begin(), args);
}

i32 ende::os::Process::id() const {
    return _info.id;
}

ende::os::Process & ende::os::Process::fork() {
    _pipes.stdin = sys::pipe().unwrap();
    _pipes.stdout = sys::pipe().unwrap();
    _pipes.stderr = sys::pipe().unwrap();

    auto res = sys::spawn(_cmd, _args, _pipes);

    if (res) {
        _info = res.unwrap();
        _forked = true;
    } else
        _forked = false;

    return *this;
}

i32 ende::os::Process::wait() {
    if (!_forked)
        return -1;
    sys::close(_pipes.stdin.input);
    sys::close(_pipes.stdin.output);
    sys::wait(_info, 0);
    return _info.ret;
}

bool ende::os::Process::kill() {
    return sys::kill(_info, SIGKILL);
}

ende::sys::Pipe ende::os::Process::stdin() const {
    return _pipes.stdin;
}

ende::sys::Pipe ende::os::Process::stdout() const {
    return _pipes.stdout;
}

ende::sys::Pipe ende::os::Process::stderr() const {
    return _pipes.stderr;
}

void ende::os::Process::printArgs() const {
    for (auto& arg : args)
        std::cout << arg << ' ';
}