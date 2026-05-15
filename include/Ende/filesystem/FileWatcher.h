#ifndef ENDE_FILEWATCHER_H
#define ENDE_FILEWATCHER_H

#include <Ende/platform.h>
#include <Ende/sys/FileDesc.h>
#include <Ende/sys/notify.h>
#include <filesystem>
#include <vector>

namespace ende::fs {

class FileWatcher {
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

#endif // ENDE_FILEWATCHER_H
