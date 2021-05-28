//
// Created by cstro29 on 28/5/21.
//

#ifndef ENDE_NOTIFY_H
#define ENDE_NOTIFY_H

#include <Ende/platform.h>
#include <string>
#include <Ende/sys/FileDesc.h>
#include <Ende/Vector.h>

namespace ende::sys::notify {

    FileDesc init(i32 flags);

    i32 addWatch(const FileDesc& fd, const std::string& pathName, u32 mask);

    i32 removeWatch(const FileDesc& fd, i32 watch);

    struct Event {
        i32 watch;
        u32 mask;
        std::string name;
    };

    Vector<Event> read(const FileDesc& fd, u32 size);


}

#endif //ENDE_NOTIFY_H
