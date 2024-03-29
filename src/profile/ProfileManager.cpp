
#include "Ende/profile/ProfileManager.h"
#include <cstring>
#include <map>
#include <Ende/thread/thread.h>

void ende::profile::submit(ProfileData &&data) {
    ProfileManager::submit(std::forward<ProfileData>(data));
}

ende::profile::ProfileManager::ProfileManager()
    : //_activeQueue(0),
//    _stop(false),
    _currentFrame(0),
    _usedFrames(60)
{
//    _queue[0].reserve(1000);
//    _queue[1].reserve(1000);
    _data.reserve(1000);
//    _offloadThread = std::thread([&]() {
//        while (true) {
//            std::unique_lock<std::mutex> lock(_queueMutex);
//            _switch.wait(lock, [&]() { return _stop || true; });
//
//            u32 queue = _activeQueue;
//            _activeQueue = (_activeQueue + 1) % 2; // switch queue
//
//            _data.insert(_data.end(), _queue[queue].begin(), _queue[queue].end());
//            _queue[queue].clear();
//
//            if (_stop)
//                break;
//        }
//    });
}

ende::profile::ProfileManager::~ProfileManager() {
//    _stop = true;
//    _switch.notify_all();

    frame();

    fs::File file;
    if (file.open("profile/profile.txt", fs::out | fs::binary))
        dump(file);

//    _offloadThread.join();
}

void ende::profile::ProfileManager::submit(ProfileData &&data) {
    instance()._data.push_back(std::move(data));
//    Vector<ProfileData>& queue = instance()._queue[instance()._activeQueue];
//    queue.push(std::forward<ProfileData>(data));
//    if (queue.size() > 900)
//        instance()._switch.notify_all();
}

struct DumpData {
    const char* label;
    u32 line;
    const char* file;
    std::vector<std::pair<std::chrono::high_resolution_clock::time_point, std::chrono::high_resolution_clock::time_point>> times;
};

bool ende::profile::ProfileManager::dump(fs::File& file) {

    for (auto& frame : instance()._frames) {
        for (auto& point : frame) {
            file.write({ point.label, static_cast<u32>(strlen(point.label)) });
            file.write("@");
            file.write(std::to_string(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::time_point_cast<std::chrono::nanoseconds>(point.start).time_since_epoch()).count()));
            file.write("@");
            file.write(std::to_string(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::time_point_cast<std::chrono::nanoseconds>(point.end).time_since_epoch()).count()));
            file.write("\n");
        }
    }

    return true;
}

void ende::profile::ProfileManager::frame() {
    instance()._frames[instance()._currentFrame++] = instance()._data;
    instance()._data.clear();
//    if (instance()._usedFrames < 60)
//        instance()._usedFrames = instance()._currentFrame;
    instance()._currentFrame = instance()._currentFrame % 60;
}