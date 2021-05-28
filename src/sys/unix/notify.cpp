//
// Created by cstro29 on 28/5/21.
//

#include "Ende/sys/notify.h"

#include <sys/inotify.h>

constexpr i32 EVENT_SIZE = sizeof(struct inotify_event);
constexpr i32 BUFF_SIZE = 1024 * (EVENT_SIZE + 16);

ende::sys::FileDesc ende::sys::notify::init(i32 flags) {
    return FileDesc(inotify_init1(flags));
}

i32 ende::sys::notify::addWatch(const FileDesc &fd, const std::string &pathName, u32 mask) {
    return inotify_add_watch(fd.handle(), pathName.c_str(), mask);
}

i32 ende::sys::notify::removeWatch(const FileDesc &fd, i32 watch) {
    return inotify_rm_watch(fd.handle(), watch);
}

ende::Vector<ende::sys::notify::Event> ende::sys::notify::read(const FileDesc &fd, u32 size) {
    std::string buffer = fd.read();

    Vector<Event> events;
    i32 length = buffer.size();
    i32 i = 0;
    while (i < length) {
        struct inotify_event* event = reinterpret_cast<struct inotify_event*>(&buffer[i]);
        Event e{};
        e.watch = event->wd;
        e.mask = event->mask;
        if (event->len)
            e.name = event->name;
        events.push(e);
        i += EVENT_SIZE + event->len;
    }
    return events;
}