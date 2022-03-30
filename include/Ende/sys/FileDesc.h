//
// Created by cstro29 on 19/5/21.
//

#ifndef ENDE_FILEDESC_H
#define ENDE_FILEDESC_H

#include <Ende/platform.h>
#include <Ende/Span.h>
#include <string>

namespace ende::sys {

    class FileDesc {
    public:

#ifdef ENDE_UNIX
        typedef i32 Handle;
#elif defined ENDE_WIN
        typedef void* Handle;
#endif

        FileDesc();

        explicit FileDesc(Handle handle);

        FileDesc(const FileDesc& fd);

        FileDesc(FileDesc&& fd) noexcept;

        FileDesc& operator=(const FileDesc& fd);

        FileDesc& operator=(FileDesc&& fd) noexcept;


        const Handle& handle() const;

        bool valid() const;


        i32 read(Span<char> buffer) const;

        std::string read() const;

        i32 write(Span<const char> buffer) const;


        bool dup(const FileDesc& fd) const;

        bool flush() const;

        u64 size() const;


        static const FileDesc& stdin();

        static const FileDesc& stdout();

        static const FileDesc& stderr();

    private:
        friend bool close(FileDesc& fd);

        Handle _handle;
        u32 _flags;


    };

    FileDesc open(const std::string& path, u32 flags, u32 perms = 0666);

    bool close(FileDesc& fd);

}

#endif //ENDE_FILEDESC_H
