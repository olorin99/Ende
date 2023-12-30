#ifndef ENDE_NOTIFY_H
#define ENDE_NOTIFY_H

#include <Ende/platform.h>
#include <string>
#include <Ende/sys/FileDesc.h>
#include <vector>

namespace ende::sys::notify {

    auto init(i32 flags) -> FileDesc;

    auto addWatch(const FileDesc& fd, const std::string& pathName, u32 mask) -> i32;

    auto removeWatch(const FileDesc& fd, i32 watch) -> i32;

    struct Event {
        i32 watch;
        u32 mask;
        std::string name;
    };

    auto read(const FileDesc& fd, u32 size) -> std::vector<Event>;


}

#endif //ENDE_NOTIFY_H
