
#ifndef ENDE_PROFILEMANAGER_H
#define ENDE_PROFILEMANAGER_H

#include <Ende/platform.h>
#include <Ende/Singleton.h>
#include <Ende/Vector.h>
#include <Ende/profile/profile.h>
#include <Ende/filesystem/File.h>

namespace ende::profile {

    class ProfileManager : public Singleton<ProfileManager> {
    public:

        ProfileManager();

        ~ProfileManager();

        static void submit(ProfileData&& data);

        static bool dump(fs::File& file);

    private:

        Vector<ProfileData> _data;

    };

}

#endif //ENDE_PROFILEMANAGER_H
