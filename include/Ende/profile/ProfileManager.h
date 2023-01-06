
#ifndef ENDE_PROFILEMANAGER_H
#define ENDE_PROFILEMANAGER_H

#include <Ende/platform.h>
#include <Ende/Singleton.h>
#include <Ende/Vector.h>
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

    private:

        std::atomic<bool> _stop;
        std::condition_variable _switch;
        std::mutex _queueMutex;
        std::thread _offloadThread;

        u32 _activeQueue;
        Vector<ProfileData> _queue[2];
        Vector<ProfileData> _data;

    };

}

#endif //ENDE_PROFILEMANAGER_H
