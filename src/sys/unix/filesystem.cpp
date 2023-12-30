#include "Ende/sys/filesystem.h"

#include <unistd.h>
#include <sys/stat.h>
#include <cstring>
#include <climits>
#include <dirent.h>

auto ende::sys::mkdir(const std::string &path) -> bool {
    return ::mkdir(path.c_str(), S_IRWXU) == 0;
}

auto ende::sys::rmdir(const std::string &path) -> bool {
    return ::rmdir(path.c_str()) == 0;
}

auto ende::sys::isdir(const std::string &path) -> bool {
    struct stat info;
    ::stat(path.c_str(), &info);
    return S_ISDIR(info.st_mode);
}

auto ende::sys::chdir(const std::string &path) -> bool {
    return ::chdir(path.c_str()) == 0;
}

auto ende::sys::remove(const std::string &path) -> bool {
    return ::remove(path.c_str()) == 0;
}

auto ende::sys::exists(const std::string &path) -> bool {
    return ::access(path.c_str(), F_OK) != -1;
}

auto ende::sys::cwd() -> std::string {
    constexpr u32 bufsize = 4096;
    char buf[bufsize];
    std::memset(buf, 0, bufsize);
    return ::getcwd(buf, bufsize - 1);
}

auto ende::sys::exePath() -> std::string {
    char buf[PATH_MAX];
    ssize_t count = ::readlink("/proc/self/exe", buf, PATH_MAX);
    return std::string(buf, count > 0 ? count : 0);
}


auto ende::sys::Permisions::read() const -> bool {
    return *this & Perm::OWNER_READ;
}

auto ende::sys::Permisions::write() const -> bool {
    return *this & Perm::OWNER_WRITE;
}

auto ende::sys::Permisions::execute() const -> bool {
    return *this & Perm::OWNER_EXEC;
}

auto ende::sys::Permisions::mode() const -> u32 {
    return perm;
}

auto ende::sys::Permisions::operator&(const Perm &rhs) const -> bool {
    return (perm & static_cast<u32>(rhs)) == static_cast<u32>(rhs);
}


auto ende::sys::FileType::dir() const -> bool {
    return S_ISDIR(mode);
}

auto ende::sys::FileType::file() const -> bool {
    return S_ISREG(mode);
}

auto ende::sys::FileType::symlink() const -> bool {
    return S_ISLNK(mode);
}


auto ende::sys::stat(const std::string &path) -> std::optional<ende::sys::Stat> {
    struct stat info;
    i32 ret = ::stat(path.c_str(), &info);
    if (ret == -1)
        return {};

    Stat retInfo{};
    retInfo.name = path;
    retInfo.size = static_cast<u64>(info.st_size);
    retInfo.fileType.mode = info.st_mode;
    retInfo.permissions.perm = info.st_mode;
    retInfo.modified = {static_cast<i64>(info.st_mtim.tv_sec), static_cast<i32>(info.st_mtim.tv_nsec)};
    retInfo.accessed = {static_cast<i64>(info.st_atim.tv_sec), static_cast<i32>(info.st_atim.tv_nsec)};
    return retInfo;
}



auto ende::sys::openDir(const std::string &path) -> std::optional<ende::sys::Dir> {
    void* handle = static_cast<void*>(::opendir(path.c_str()));
    if (!handle) return {};

    Dir dirInfo{};
    dirInfo.path = path;
    dirInfo.handle = handle;
    return dirInfo;
}

auto ende::sys::readDir(Dir &dir) -> std::optional<ende::sys::DirEntry> {
    if (!dir.handle)
        return {};
    struct dirent* entry = ::readdir(static_cast<DIR*>(dir.handle));
    if (!entry)
        return {};

    DirEntry dirEntry{};
    dirEntry.name = entry->d_name;
    return dirEntry;
}

auto ende::sys::closeDir(Dir &dir) -> bool {
    return ::closedir(static_cast<DIR*>(dir.handle)) == 0;
}