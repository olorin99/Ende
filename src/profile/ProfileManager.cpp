
#include "Ende/profile/ProfileManager.h"
#include <cstring>
#include <map>

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

struct DumpData {
    const char* label;
    i32 line;
    const char* file;
    ende::Vector<ende::time::Duration> times;
};

bool ende::profile::ProfileManager::dump(fs::File& file) {

    std::map<const char*, DumpData> mappedData;
    for (auto& data : instance()._data) {
        auto it = mappedData.find(data.label);
        if (it != mappedData.end())
            it->second.times.push(data.time);
        else {
            auto inserted = mappedData.insert(std::make_pair(data.label, DumpData{data.label, data.line, data.file, Vector<time::Duration>()}));
            inserted.first->second.times.push(data.time);
        }
    }




    for (auto& mapped : mappedData) {
        auto& data = mapped.second;
        file.write({data.label, static_cast<u32>(strlen(data.label))});
        file.write(":");
        file.write({data.file, static_cast<u32>(strlen(data.file))});
        file.write(":");
        auto line = std::to_string(data.line);
        file.write(line);
        file.write(":");

        for (auto& time : data.times) {
            auto sec = std::to_string((time.nanoseconds()));
            file.write(sec);
            file.write(",");
        }
        file.write("\n");
    }

    return true;
}