//
// Created by cstro29 on 19/5/21.
//

#include "Ende/profile/ProfileManager.h"

void ende::profile::submit(ProfileData &&data) {
    ProfileManager::submit(std::forward<ProfileData>(data));
}

ende::profile::ProfileManager::ProfileManager() {
    _data.push({});
}

ende::profile::ProfileManager::~ProfileManager() {

    auto fd = sys::open("profile/profile.json", 0);
    dump(fd);
}

