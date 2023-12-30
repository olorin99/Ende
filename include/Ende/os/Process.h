#ifndef ENDE_PROCESS_H
#define ENDE_PROCESS_H

#include <Ende/platform.h>
#include <Ende/sys/process.h>
#include <Ende/sys/pipe.h>
#include <vector>
#include <span>

namespace ende::os {

    class Process {
    public:

        Process(const std::string& cmd);

        ~Process();

        Process(const Process& process) = delete;

        Process(Process&& process) noexcept;

        Process& operator=(const Process& process) = delete;

        Process& operator=(Process&& process) noexcept;


        auto arg(const std::string& arg) -> Process&;

        auto args(std::span<std::string> args) -> Process&;

        auto id() const -> i32;

        auto fork() -> Process&;

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

#endif //ENDE_PROCESS_H
