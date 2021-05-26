//
// Created by cstro29 on 26/5/21.
//

#ifndef ENDE_PROCESS_H
#define ENDE_PROCESS_H

#include <Ende/platform.h>
#include <Ende/sys/process.h>
#include <Ende/sys/pipe.h>
#include <Ende/Vector.h>
#include <Ende/Span.h>

namespace ende::os {

    class Process {
    public:

        Process(const std::string& cmd);

        ~Process();

        Process(const Process& process) = delete;

        Process(Process&& process) noexcept;

        Process& operator=(const Process& process) = delete;

        Process& operator=(Process&& process) noexcept;


        Process& arg(const std::string& arg);

        Process& args(Span<std::string> args);

        i32 id() const;

        Process& fork();

        i32 wait();

        bool kill();


        sys::Pipe stdin() const;

        sys::Pipe stdout() const;

        sys::Pipe stderr() const;

        void printArgs() const;

    private:

        sys::Pipes _pipes;
        sys::ProcessInfo _info;
        bool _forked;
        std::string _cmd;
        Vector<std::string> _args;

    };

}

#endif //ENDE_PROCESS_H
