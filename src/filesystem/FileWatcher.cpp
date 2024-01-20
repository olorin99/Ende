#include "Ende/filesystem/FileWatcher.h"
#include <Ende/sys/notify.h>
#include <sys/inotify.h>

ende::fs::FileWatcher::FileWatcher() {
    _watcher = sys::notify::init(IN_NONBLOCK);
}

void ende::fs::FileWatcher::addWatch(const std::filesystem::path &path) {
    auto watch = sys::notify::addWatch(_watcher, path.string(), IN_ACCESS | IN_MODIFY);
    _watches.push_back(std::make_pair(path, watch));
}

void ende::fs::FileWatcher::removeWatch(const std::filesystem::path &path) {
    for (u32 i = 0; i < _watches.size(); i++) {
        auto& watch = _watches[i];
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
    for (auto& event : systemEvents) {
        for (auto& watch : _watches) {
            if (event.watch == watch.second) {
                events.push_back({
                    .path = watch.first,
                    .mask = event.mask
                });
                break;
            }
        }
    }
    return events;
}