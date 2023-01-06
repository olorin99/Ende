
#include "Ende/profile/ProfileManager.h"
#include <cstring>
#include <map>
#include <Ende/thread/thread.h>

void ende::profile::submit(ProfileData &&data) {
    ProfileManager::submit(std::forward<ProfileData>(data));
}

ende::profile::ProfileManager::ProfileManager()
    : _activeQueue(0),
    _stop(false)
{
    _queue[0].reserve(1000);
    _queue[1].reserve(1000);
    _data.reserve(1000);
    _offloadThread = std::thread([&]() {
        while (true) {
            std::unique_lock<std::mutex> lock(_queueMutex);
            _switch.wait(lock, [&]() { return _stop || true; });

            u32 queue = _activeQueue;
            _activeQueue = (_activeQueue + 1) % 2; // switch queue

            _data.insert(_data.end(), _queue[queue].begin(), _queue[queue].end());
            _queue[queue].clear();

            if (_stop)
                break;
        }
    });
}

ende::profile::ProfileManager::~ProfileManager() {
    _stop = true;
    _switch.notify_all();

    fs::File file;
    if (file.open("profile/profile.txt"_path, fs::out | fs::binary))
        dump(file);

    _offloadThread.join();
}

void ende::profile::ProfileManager::submit(ProfileData &&data) {
    Vector<ProfileData>& queue = instance()._queue[instance()._activeQueue];
    queue.push(std::forward<ProfileData>(data));
    if (queue.size() > 900)
        instance()._switch.notify_all();
}

struct DumpData {
    const char* label;
    u32 line;
    const char* file;
    ende::Vector<std::pair<ende::time::Instant, ende::time::Instant>> times;
};

bool ende::profile::ProfileManager::dump(fs::File& file) {

    for (auto& point : instance()._data) {
        file.write({ point.label, static_cast<u32>(strlen(point.label)) });
        file.write("@");
        file.write(std::to_string(point.start.nanoseconds()));
        file.write("@");
        file.write(std::to_string(point.end.nanoseconds()));
        file.write("\n");
    }

    return true;
}