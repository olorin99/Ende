module;

#include <Ende/platform.h>
#include <cstring>
#include <chrono>

export module ende.profile;

import ende.filesystem.File;
import ende.util;
import ende.Singleton;

namespace ende::profile {

    export struct ProfileData {
        const char *label;
        u32 line;
        const char *file;
        std::chrono::high_resolution_clock::time_point start;
        std::chrono::high_resolution_clock::time_point end;
    };

    export void submit(ProfileData &&data);

    export class Profile {
      public:
        explicit Profile(const char *label, u32 line, const char *file)
            : _label(label),
              _line(line),
              _file(file),
              _start(std::chrono::high_resolution_clock::now()) {}

        ~Profile() {
            submit({_label, _line, _file, _start, std::chrono::high_resolution_clock::now()});
        }

      private:
        const char *_label;
        u32 _line;
        const char *_file;
        std::chrono::high_resolution_clock::time_point _start;
    };

export class ProfileManager : public Singleton<ProfileManager> {
  public:
    ProfileManager();

    ~ProfileManager();

    static void submit(ProfileData &&data);

    static bool dump(fs::File &file);

    static void frame();

    static u32 getMaxFrames() { return 60; }

    static u32 getCurrentFrame() { return instance()._currentFrame; }

    static std::span<ProfileData> getFrameData(u32 frame) { return instance()._frames[frame]; }

  private:
    //        std::atomic<bool> _stop;
    //        std::condition_variable _switch;
    //        std::mutex _queueMutex;
    //        std::thread _offloadThread;
    //
    //        u32 _activeQueue;
    //        Vector<ProfileData> _queue[2];
    std::vector<ProfileData> _data;

    u32 _currentFrame;
    u32 _usedFrames;
    std::vector<ProfileData> _frames[60];
};

}

void ende::profile::submit(ProfileData &&data) {
    ProfileManager::submit(std::forward<ProfileData>(data));
}

ende::profile::ProfileManager::ProfileManager()
    : //_activeQueue(0),
      //    _stop(false),
      _currentFrame(0),
      _usedFrames(60) {
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
    const char *label;
    u32 line;
    const char *file;
    std::vector<std::pair<std::chrono::high_resolution_clock::time_point, std::chrono::high_resolution_clock::time_point>> times;
};

bool ende::profile::ProfileManager::dump(fs::File &file) {

    for (auto &frame : instance()._frames) {
        for (auto &point : frame) {
            file.write({point.label, static_cast<u32>(strlen(point.label))});
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
