
#include <Ende/sys/filesystem.h>
#include <Ende/filesystem/File.h>

ende::fs::File::~File() {
    close();
}

ende::fs::File::File(FILE* handle, const u8 mode)
    : _handle(handle),
    _mode(mode)
{}

ende::fs::File::File(File &&rhs) noexcept {
    std::swap(_handle, rhs._handle);
    std::swap(_pos, rhs._pos);
    std::swap(_mode, rhs._mode);
    std::swap(_path, rhs._path);
}

ende::fs::File & ende::fs::File::operator=(File &&rhs) noexcept {
    std::swap(_handle, rhs._handle);
    std::swap(_pos, rhs._pos);
    std::swap(_mode, rhs._mode);
    std::swap(_path, rhs._path);
    return *this;
}



auto ende::fs::File::open(const std::filesystem::path &path, const u8 mode) -> std::expected<File, i32> {
    if (const auto stat = sys::stat(path); stat && !stat->fileType.file())
        return std::unexpected(-1);

    File file = {};

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

    FILE* handle = fopen(path.c_str(), md.c_str());
    if (!handle)
        return std::unexpected(-1);

    file._handle = handle;
    file._pos = 0;
    file._mode = mode;
    file._path = path;
    return file;
}

auto ende::fs::File::close() -> bool {
    if (!_handle) return true;
    _mode = 0;
    const bool ret = fclose(_handle) == 0;
    if (ret) _handle = nullptr;
    return ret;
}

auto ende::fs::File::isMode(const u8 mode) const -> bool {
    return (_mode & mode) == mode;
}

auto ende::fs::File::isOpen() const -> bool {
    return _handle;
}

auto ende::fs::File::size() const -> u64 {
    if (!_handle) return 0;
    return ende::sys::stat(_path).value_or(sys::Stat{}).size;
}

auto ende::fs::File::pos() const -> u64 {
    return _pos;
}

auto ende::fs::File::seek(const u64 position) -> bool {
    if (!_handle) return false;
    const bool res = fseek(_handle, position, SEEK_SET) == 0;
    _pos = position;
    return res;
}

auto ende::fs::File::seekPos(const i64 position) -> bool {
    if (!_handle) return false;
    const bool res = fseek(_handle, position, SEEK_CUR) == 0;
    _pos += position;
    return res;
}

auto ende::fs::File::read(std::span<char> buffer) const -> u64 {
    if (!_handle || !isMode(in)) return 0;
    return fread(buffer.data(), sizeof(char), buffer.size(), _handle);
}

auto ende::fs::File::read(std::span<u8> buffer) const -> u64 {
    if (!_handle || !isMode(in)) return 0;
    return fread(buffer.data(), sizeof(u8), buffer.size(), _handle);
}

auto ende::fs::File::read() const -> std::string {
    const u64 fileSize = size();
    if (fileSize == 0) return "";
    std::string buffer(fileSize, '\0');
    read(buffer);
    return buffer;
}

auto ende::fs::File::readLn() -> std::string {
    char* line = nullptr;
    std::size_t size = 0;
    const i32 len = ::getline(&line, &size, _handle);
    if (len == -1) {
        free(line);
        return "";
    }
    std::string buffer(line, len);
    _pos += len;
    free(line);
    return buffer;
}

auto ende::fs::File::write(const std::span<const char> buffer) const -> u64 {
    if (!_handle || (!isMode(out) && !isMode(append))) return 0;
    return fwrite(buffer.data(), sizeof(char), buffer.size(), _handle);
}

auto ende::fs::File::handle() const -> FILE* {
    return _handle;
}

auto ende::fs::File::path() const -> std::filesystem::path {
    return _path;
}

ende::fs::File::operator bool() const {
    return isOpen();
}