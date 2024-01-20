#include "Ende/sys/notify.h"

#include <sys/inotify.h>

constexpr i32 EVENT_SIZE = sizeof(struct inotify_event);
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
            struct inotify_event* event = reinterpret_cast<struct inotify_event*>(&buffer[i]);
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