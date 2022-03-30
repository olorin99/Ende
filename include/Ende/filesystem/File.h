
#ifndef ENDE_FILE_H
#define ENDE_FILE_H

#include <Ende/platform.h>
#include <Ende/filesystem/Path.h>
#include <Ende/Shared.h>
#include <cstdio>

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

        File();

        File(FILE* handle, u8 mode);

        ~File();

        File(const File& file);

        File(File&& file) noexcept;

        File& operator=(const File& file);

        File& operator=(File&& file) noexcept;


        FILE* handle() const;

        bool open(const Path& path, u8 mode = in | text);

        bool close();

        bool detach();


        bool isMode(u8 mode) const;

        bool isOpen() const;

        u64 size() const;

        u64 pos() const;

        bool seek(u64 position);

        bool seekPos(i64 position);


        u64 read(Span<char> buffer);

        std::string read();

        std::string readLn();

        u64 write(Span<const char> buffer);

        Path path() const;

        explicit operator bool() const;

    private:

        struct FileData {
            FILE* handle;
            u64 pos;
            u8 mode;
            Path path;
            bool shouldClose = true;
        };
        Shared<FileData> _data;

    };

}

#endif //ENDE_FILE_H
