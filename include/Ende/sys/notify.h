#ifndef ENDE_NOTIFY_H
#define ENDE_NOTIFY_H

#include <Ende/platform.h>
#include <string>
#include <Ende/sys/FileDesc.h>
#include <vector>
#include <sys/inotify.h>

namespace ende::sys::notify {

    enum class Mask {
        ACCESS = IN_ACCESS,
        ATTRIB = IN_ATTRIB,
        CLOSE_WRITE = IN_CLOSE_WRITE,
        CLOSE_NOWRITE = IN_CLOSE_NOWRITE,
        CREATE = IN_CREATE,
        DELETE = IN_DELETE,
        DELETE_SELF = IN_DELETE_SELF,
        MODIFY = IN_MODIFY,
        MOVE_SELF = IN_MOVE_SELF,
        MOVED_FROM = IN_MOVED_FROM,
        MOVED_TO = IN_MOVED_TO,
        OPEN = IN_OPEN
    };

    constexpr inline auto operator|(const Mask& lhs, const Mask& rhs) -> Mask {
        return static_cast<Mask>(static_cast<std::underlying_type<Mask>::type>(lhs) | static_cast<std::underlying_type<Mask>::type>(rhs));
    }

    auto init(i32 flags) -> FileDesc;

    auto addWatch(const FileDesc& fd, const std::string& pathName, Mask mask) -> i32;

    auto removeWatch(const FileDesc& fd, i32 watch) -> i32;

    struct Event {
        i32 watch;
        Mask mask;
        std::string name;
    };

    auto read(const FileDesc& fd, u32 size) -> std::vector<Event>;


}

#endif //ENDE_NOTIFY_H
