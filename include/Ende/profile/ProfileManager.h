//
// Created by cstro29 on 19/5/21.
//

#ifndef ENDE_PROFILEMANAGER_H
#define ENDE_PROFILEMANAGER_H

#include <Ende/platform.h>
#include <Ende/Singleton.h>
#include <Ende/Vector.h>
#include <Ende/profile/profile.h>
#include <Ende/sys/FileDesc.h>

namespace ende::profile {

    class ProfileManager : public Singleton<ProfileManager> {
    public:

        ProfileManager();

        ~ProfileManager();

        static void submit(ProfileData&& data);

        static bool dump(sys::FileDesc& fd);

    private:

        Vector<ProfileData> _data;

    };

}

#endif //ENDE_PROFILEMANAGER_H
