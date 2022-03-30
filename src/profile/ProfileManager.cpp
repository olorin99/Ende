
#include "Ende/profile/ProfileManager.h"
#include <cstring>

void ende::profile::submit(ProfileData &&data) {
    ProfileManager::submit(std::forward<ProfileData>(data));
}

ende::profile::ProfileManager::ProfileManager() {}

ende::profile::ProfileManager::~ProfileManager() {
    fs::File file;
    if (file.open("profile/profile.txt"_path, fs::out | fs::binary))
        dump(file);
}

void ende::profile::ProfileManager::submit(ProfileData &&data) {
    instance()._data.push(std::forward<ProfileData>(data));
}

bool ende::profile::ProfileManager::dump(fs::File& file) {
    for (auto& data : instance()._data) {
        file.write({data.label, static_cast<u32>(strlen(data.label))});
        file.write(":");
        file.write({data.file, static_cast<u32>(strlen(data.file))});
        file.write(":");
        auto line = std::to_string(data.line);
        file.write(line);
        file.write(":");

        auto sec = std::to_string((data.time.milliseconds()));
        file.write(sec);
        file.write("\n");
    }

    return true;
}