#ifndef ENDE_FILE_H
#define ENDE_FILE_H

#include <Ende/platform.h>
#include <filesystem>
#include <span>
#include <cstdio>
#include <expected>

namespace ende::fs {

    enum : u8 {
        none = 0x00,
        in = 0x01,
        out = 0x02,
        text = 0x04,
        binary = 0x08,
        append = 0x10
    };

    class File {
    public:

        File() = default;
        File(FILE* handle, u8 mode);
        ~File();
        File(File&& rhs) noexcept;
        File& operator=(File&& rhs) noexcept;

        static auto open(const std::filesystem::path& path, u8 mode = in | text) -> std::expected<File, i32>;

        auto close() -> bool;

        auto isMode(u8 mode) const -> bool;

        auto isOpen() const -> bool;

        auto size() const -> u64;

        auto pos() const -> u64;

        auto seek(u64 position) -> bool;

        auto seekPos(i64 position) -> bool;

        auto read(std::span<char> buffer) const -> u64;

        auto read(std::span<u8> buffer) const -> u64;
        auto read() const -> std::string;
        auto readLn() -> std::string;
        auto write(std::span<const char> buffer) const -> u64;

        auto handle() const -> FILE*;

        auto path() const -> std::filesystem::path;

        explicit operator bool() const;

    private:

        FILE* _handle = nullptr;
        u64 _pos = 0;
        u8 _mode = none;
        std::filesystem::path _path;

    };

}

#endif //ENDE_FILE_H
