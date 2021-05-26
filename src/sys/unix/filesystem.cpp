//
// Created by cstro29 on 19/5/21.
//

#include "Ende/sys/filesystem.h"

#include <unistd.h>
#include <sys/stat.h>
#include <cstring>
#include <climits>
#include <dirent.h>

bool ende::sys::mkdir(const std::string &path) {
    return ::mkdir(path.c_str(), S_IRWXU) == 0;
}

bool ende::sys::rmdir(const std::string &path) {
    return ::rmdir(path.c_str()) == 0;
}

bool ende::sys::isdir(const std::string &path) {
    struct stat info;
    ::stat(path.c_str(), &info);
    return S_ISDIR(info.st_mode);
}

bool ende::sys::chdir(const std::string &path) {
    return ::chdir(path.c_str()) == 0;
}

bool ende::sys::remove(const std::string &path) {
    return ::remove(path.c_str()) == 0;
}

bool ende::sys::exists(const std::string &path) {
    return ::access(path.c_str(), F_OK) != -1;
}

std::string ende::sys::cwd() {
    constexpr u32 bufsize = 4096;
    char buf[bufsize];
    std::memset(buf, 0, bufsize);
    return ::getcwd(buf, bufsize - 1);
}

std::string ende::sys::exePath() {
    char buf[PATH_MAX];
    ssize_t count = ::readlink("/proc/self/exe", buf, PATH_MAX);
    return std::string(buf, count > 0 ? count : 0);
}


bool ende::sys::Permisions::read() const {
    return *this & Perm::OWNER_READ;
}

bool ende::sys::Permisions::write() const {
    return *this & Perm::OWNER_WRITE;
}

bool ende::sys::Permisions::execute() const {
    return *this & Perm::OWNER_EXEC;
}

u32 ende::sys::Permisions::mode() const {
    return perm;
}

bool ende::sys::Permisions::operator&(const Perm &rhs) const {
    return (perm & static_cast<u32>(rhs)) == static_cast<u32>(rhs);
}


bool ende::sys::FileType::dir() const {
    return S_ISDIR(mode);
}

bool ende::sys::FileType::file() const {
    return S_ISREG(mode);
}

bool ende::sys::FileType::symlink() const {
    return S_ISLNK(mode);
}


ende::Optional<ende::sys::Stat> ende::sys::stat(const std::string &path) {
    struct stat info;
    i32 ret = ::stat(path.c_str(), &info);
    if (ret == -1)
        return None;

    Stat retInfo{};
    retInfo.name = path;
    retInfo.size = static_cast<u64>(info.st_size);
    retInfo.fileType.mode = info.st_mode;
    retInfo.permissions.perm = info.st_mode;
    retInfo.modified = {static_cast<i64>(info.st_mtim.tv_sec), static_cast<i32>(info.st_mtim.tv_nsec)};
    retInfo.accessed = {static_cast<i64>(info.st_atim.tv_sec), static_cast<i32>(info.st_atim.tv_nsec)};
    return Some(retInfo);
}



ende::Optional<ende::sys::Dir> ende::sys::openDir(const std::string &path) {
    void* handle = static_cast<void*>(::opendir(path.c_str()));
    if (!handle) return None;

    Dir dirInfo{};
    dirInfo.path = path;
    dirInfo.handle = handle;
    return Some(dirInfo);
}

ende::Optional<ende::sys::DirEntry> ende::sys::readDir(Dir &dir) {
    if (!dir.handle)
        return None;
    struct dirent* entry = ::readdir(static_cast<DIR*>(dir.handle));
    if (!entry)
        return None;

    DirEntry dirEntry{};
    dirEntry.name = entry->d_name;
    return Some(dirEntry);
}

bool ende::sys::closeDir(Dir &dir) {
    return ::closedir(static_cast<DIR*>(dir.handle)) == 0;
}