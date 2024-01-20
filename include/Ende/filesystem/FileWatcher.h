#ifndef ENDE_FILEWATCHER_H
#define ENDE_FILEWATCHER_H

#include <Ende/platform.h>
#include <Ende/sys/FileDesc.h>
#include <vector>
#include <filesystem>
#include <Ende/sys/notify.h>

namespace ende::fs {

    class FileWatcher {
    public:

        FileWatcher();

        void addWatch(const std::filesystem::path& path);

        void removeWatch(const std::filesystem::path& path);

        struct Event {
            std::filesystem::path path;
            sys::notify::Mask mask;
        };

        auto read() -> std::vector<Event>;

    private:

        sys::FileDesc _watcher = {};
        std::vector<std::pair<std::filesystem::path, i32>> _watches = {};

    };

}

#endif //ENDE_FILEWATCHER_H
