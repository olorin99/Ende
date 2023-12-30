#ifndef ENDE_FILESYSTEM_H
#define ENDE_FILESYSTEM_H

#include <Ende/platform.h>
#include <Ende/sys/time.h>
#include <optional>
#include <string>

namespace ende::sys {

    auto mkdir(const std::string& path) -> bool;

    auto rmdir(const std::string& path) -> bool;

    auto isdir(const std::string& path) -> bool;

    auto chdir(const std::string& path) -> bool;

    auto remove(const std::string& path) -> bool;

    auto exists(const std::string& path) -> bool;

    auto cwd() -> std::string;

    auto exePath() -> std::string;

    enum class Perm {
        none = 0,
        OWNER_READ = 0400,
        OWNER_WRITE = 0200,
        OWNER_EXEC = 0100,
        OWNER_ALL = 0700,

        GROUP_READ = 040,
        GROUP_WRITE = 020,
        GROUP_EXEC = 010,
        GROUP_ALL = 070,

        OTHERS_READ = 04,
        OTHERS_WRITE = 02,
        OTHERS_EXEC = 01,
        OTHERS_ALL = 07,

        ALL = 0777
    };

    struct Permisions {

        auto read() const -> bool;

        auto write() const -> bool;

        auto execute() const -> bool;

        auto mode() const -> u32;

        bool operator&(const Perm& rhs) const;

    private:
        friend auto stat(const std::string& path) -> std::optional<struct Stat>;
        u32 perm = 0;
    };

    struct FileType {

        auto dir() const -> bool;

        auto file() const -> bool;

        auto symlink() const -> bool;

    private:
        friend auto stat(const std::string& path) -> std::optional<struct Stat>;
        u32 mode = 0;
    };

    struct Stat {
        std::string name = "";
        u64 size = 0;
        FileType fileType;
        Permisions permissions;
        TimeSpec modified;
        TimeSpec accessed;
    };

    auto stat(const std::string& path) -> std::optional<struct Stat>;


    struct Dir {
        std::string path = "";
        void* handle = nullptr;
        void* data = nullptr;
    };

     auto openDir(const std::string& path) -> std::optional<Dir>;

    struct DirEntry {
        std::string name = "";
    };

    auto readDir(Dir& dir) -> std::optional<DirEntry>;

    auto closeDir(Dir& dir) -> bool;

}

#endif //ENDE_FILESYSTEM_H
