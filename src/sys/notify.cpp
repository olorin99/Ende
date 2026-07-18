module;

#include <Ende/platform.h>
#include <string>
#include <sys/inotify.h>
#include <vector>

export module ende.system.notify;

import ende.system.FileDesc;
import ende.util;

namespace ende::sys::notify {

export enum class Mask {
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

export constexpr inline auto operator|(const Mask &lhs, const Mask &rhs) -> Mask {
    return static_cast<Mask>(static_cast<std::underlying_type<Mask>::type>(lhs) | static_cast<std::underlying_type<Mask>::type>(rhs));
}

export auto init(i32 flags) -> FileDesc;

export auto addWatch(const FileDesc &fd, const std::string &pathName, Mask mask) -> i32;

export auto removeWatch(const FileDesc &fd, i32 watch) -> i32;

export struct Event {
    i32 watch;
    Mask mask;
    std::string name;
};

export auto read(const FileDesc &fd, u32 size) -> std::vector<Event>;

}

constexpr i32 EVENT_SIZE = sizeof(inotify_event);
constexpr i32 BUFF_SIZE = 1024 * (EVENT_SIZE + 16);

auto ende::sys::notify::init(i32 flags) -> FileDesc {
    return FileDesc(inotify_init1(flags));
}

auto ende::sys::notify::addWatch(const FileDesc &fd, const std::string &pathName, Mask mask) -> i32 {
    return inotify_add_watch(fd.handle(), pathName.c_str(), static_cast<u32>(mask));
}

auto ende::sys::notify::removeWatch(const FileDesc &fd, i32 watch) -> i32 {
    return inotify_rm_watch(fd.handle(), watch);
}

auto ende::sys::notify::read(const FileDesc &fd, u32 size) -> std::vector<Event> {
    std::vector<Event> events;
    std::string buffer(size * EVENT_SIZE, '\0');

    i32 count = fd.read(buffer);
    while (count > 0) {
        i32 i = 0;
        while (i < count) {
            struct inotify_event *event = reinterpret_cast<struct inotify_event *>(&buffer[i]);
            Event e{};
            e.watch = event->wd;
            e.mask = static_cast<Mask>(event->mask);
            if (event->len)
                e.name = event->name;
            events.push_back(e);
            i += EVENT_SIZE + event->len;
        }
        count = fd.read(buffer);
    }
    return events;
}
