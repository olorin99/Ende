#ifndef ENDE_FILEDESC_H
#define ENDE_FILEDESC_H

#include <Ende/platform.h>
#include <span>
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


        auto handle() const -> const Handle&;

        auto valid() const -> bool;


        auto read(std::span<char> buffer) const -> i32;

        auto read() const -> std::string;

        auto write(std::span<const char> buffer) const -> i32;


        auto dup(const FileDesc& fd) const -> bool;

        auto flush() const -> bool;

        auto size() const -> u64;


        static auto stdin() -> const FileDesc&;

        static auto stdout() -> const FileDesc&;

        static auto stderr() -> const FileDesc&;

    private:
        friend bool close(FileDesc& fd);

        Handle _handle;
        u32 _flags;


    };

    FileDesc open(const std::string& path, u32 flags, u32 perms = 0666);

    bool close(FileDesc& fd);

}

#endif //ENDE_FILEDESC_H
