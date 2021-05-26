//
// Created by cstro29 on 19/5/21.
//

#ifndef ENDE_FILESYSTEM_H
#define ENDE_FILESYSTEM_H

#include <Ende/platform.h>
#include <Ende/sys/time.h>
#include <Ende/Optional.h>
#include <string>

namespace ende::sys {

    bool mkdir(const std::string& path);

    bool rmdir(const std::string& path);

    bool isdir(const std::string& path);

    bool chdir(const std::string& path);

    bool remove(const std::string& path);

    bool exists(const std::string& path);

    std::string cwd();

    std::string exePath();


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

        bool read() const;

        bool write() const;

        bool execute() const;

        u32 mode() const;

        bool operator&(const Perm& rhs) const;

    private:
        friend Optional<struct Stat> stat(const std::string& path);
        u32 perm = 0;
    };

    struct FileType {

        bool dir() const;

        bool file() const;

        bool symlink() const;

    private:
        friend Optional<struct Stat> stat(const std::string& path);
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

    Optional<Stat> stat(const std::string& path);


    struct Dir {
        std::string path = "";
        void* handle = nullptr;
        void* data = nullptr;
    };

    Optional<Dir> openDir(const std::string& path);

    struct DirEntry {
        std::string name = "";
    };

    Optional<DirEntry> readDir(Dir& dir);

    bool closeDir(Dir& dir);

}

#endif //ENDE_FILESYSTEM_H
