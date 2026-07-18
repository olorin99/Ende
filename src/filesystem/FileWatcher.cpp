module;

#include <Ende/platform.h>
#include <filesystem>
#include <vector>
#include <string>

#include <sys/inotify.h>

export module ende.filesystem.FileWatcher;

import ende.system.notify;
import ende.system.FileDesc;
import ende.util;

namespace ende::fs {

export class FileWatcher {
  public:
    FileWatcher();

    void addWatch(const std::filesystem::path &path, sys::notify::Mask mask = sys::notify::Mask::MODIFY);

    void removeWatch(const std::filesystem::path &path);

    struct Event {
        std::filesystem::path path;
        sys::notify::Mask mask;
    };

    auto read() -> std::vector<Event>;

  private:
    sys::FileDesc _watcher = {};
    std::vector<std::pair<std::filesystem::path, i32>> _watches = {};
};

} // namespace ende::fs

ende::fs::FileWatcher::FileWatcher() {
    _watcher = sys::notify::init(IN_NONBLOCK);
}

void ende::fs::FileWatcher::addWatch(const std::filesystem::path &path, sys::notify::Mask mask) {
    auto watch = sys::notify::addWatch(_watcher, path.string(), mask);
    _watches.push_back(std::make_pair(path, watch));
}

void ende::fs::FileWatcher::removeWatch(const std::filesystem::path &path) {
    for (u32 i = 0; i < _watches.size(); i++) {
        auto &watch = _watches[i];
        if (watch.first == path) {
            sys::notify::removeWatch(_watcher, watch.second);
            _watches.erase(_watches.begin() + i);
            return;
        }
    }
}

auto ende::fs::FileWatcher::read() -> std::vector<Event> {
    auto systemEvents = sys::notify::read(_watcher, 10);

    std::vector<Event> events = {};
    for (auto &event : systemEvents) {
        for (auto &watch : _watches) {
            if (event.watch == watch.second) {
                events.push_back({.path = watch.first,
                                  .mask = event.mask});
                break;
            }
        }
    }
    return events;
}
