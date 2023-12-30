
#ifndef ENDE_PROFILEMANAGER_H
#define ENDE_PROFILEMANAGER_H

#include <Ende/platform.h>
#include <Ende/Singleton.h>
#include <vector>
#include <Ende/profile/profile.h>
#include <Ende/filesystem/File.h>
#include <thread>
#include <condition_variable>

namespace ende::profile {

    class ProfileManager : public Singleton<ProfileManager> {
    public:

        ProfileManager();

        ~ProfileManager();

        static void submit(ProfileData&& data);

        static bool dump(fs::File& file);

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

#endif //ENDE_PROFILEMANAGER_H
