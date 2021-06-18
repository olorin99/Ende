//
// Created by cstro29 on 3/6/21.
//

#include <Ende/sys/filesystem.h>
#include "Ende/filesystem/File.h"

ende::fs::File::File()
    : _data(FileData{nullptr, 0, none})
{}

ende::fs::File::File(FILE *handle, u8 mode)
    : _data(FileData{handle, 0, mode, Path(""), false})
{}

ende::fs::File::~File() {
    detach();
}

ende::fs::File::File(const File &file)
    : _data(file._data)
{}

ende::fs::File::File(File &&file) noexcept {
    std::swap(_data, file._data);
}

ende::fs::File & ende::fs::File::operator=(const File &file) {
    _data = file._data;
    return *this;
}

ende::fs::File & ende::fs::File::operator=(File &&file) noexcept {
    std::swap(_data, file._data);
    return *this;
}

FILE * ende::fs::File::handle() const {
    return _data->handle;
}

bool ende::fs::File::open(const Path &path, u8 mode) {
    if (!path.isFile())
        return false;

    if (_data->mode)
        close();

    std::string md;
    if (mode & in)
        md = "r";
    else if (mode & out)
        md = "w";
    else if (mode & append)
        md = "a";
    if (mode & binary)
        md += "b";
    if ((mode & (in | out)) == (in | out))
        md += "+";

    FILE* handle = fopen((*path).c_str(), md.c_str());
    if (!handle)
        return false;

    _data->handle = handle;
    _data->pos = 0;
    _data->mode = mode;
    _data->path = path;
    return true;
}

bool ende::fs::File::close() {
    if (!_data) return true;
    if (!_data->handle) return true;
    _data->mode = 0;
    bool ret = fclose(_data->handle) == 0;
    if (ret) _data->handle = nullptr;
    return ret;
}

bool ende::fs::File::detach() {
    if (_data.unique() && _data->shouldClose)
        close();
    return _data.detach();
}

bool ende::fs::File::isMode(u8 mode) const {
    return (_data->mode & mode) == mode;
}

bool ende::fs::File::isOpen() const {
    return _data && _data->handle;
}

u64 ende::fs::File::size() const {
    if (!_data->handle) return 0;
    return ende::sys::stat(*_data->path).unwrap_or({}).size;
}

u64 ende::fs::File::pos() const {
    return _data->pos;
}

bool ende::fs::File::seek(u64 position) {
    if (!_data->handle) return false;
    bool res = fseek(_data->handle, position, SEEK_SET) == 0;
    _data->pos = position;
    return res;
}

bool ende::fs::File::seekPos(i64 position) {
    if (!_data->handle) return false;
    bool res = fseek(_data->handle, position, SEEK_CUR) == 0;
    _data->pos += position;
    return res;
}

u64 ende::fs::File::read(Span<char> buffer) {
    if (!_data->handle || !isMode(in)) return 0;
    return fread(buffer.data(), sizeof(char), buffer.size(), _data->handle);
}

std::string ende::fs::File::read() {
    u64 fileSize = size();
    if (fileSize == 0) return "";
    std::string buffer(fileSize, '\0');
    read(buffer);
    return buffer;
}

std::string ende::fs::File::readLn() {
    char* line = nullptr;
    std::size_t size = 0;
    i32 len = ::getline(&line, &size, _data->handle);
    if (len == -1) {
        free(line);
        return "";
    }
    std::string buffer(line, len);
    _data->pos += len;
    free(line);
    return buffer;
}

u64 ende::fs::File::write(Span<const char> buffer) {
    if (!_data->handle || (!isMode(out) && !isMode(append))) return 0;
    return fwrite(buffer.data(), sizeof(char), buffer.size(), _data->handle);
}

ende::fs::Path ende::fs::File::path() const {
    return _data->path;
}

ende::fs::File::operator bool() const {
    return isOpen();
}