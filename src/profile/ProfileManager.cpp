
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
    u32 line;
    const char* file;
    ende::Vector<ende::time::Duration> times;
};

bool ende::profile::ProfileManager::dump(fs::File& file) {

    bool inFrame = false;
    u32 currentFrame = 0;

    std::map<u32, std::map<const char*, DumpData>> mappedData;
    for (auto& data : instance()._data) {
        if (strcmp(data.label, "FRAME_BEGIN") == 0) {
            inFrame = true;
            continue;
        } else if (strcmp(data.label, "FRAME_END") == 0) {
            inFrame = false;
            continue;
        }

        auto frame = mappedData.find(inFrame ? currentFrame : 0);
        if (frame == mappedData.end()) {
            auto inserted = mappedData.insert(std::make_pair(inFrame ? currentFrame : 0, std::map<const char*, DumpData>()));
            frame = mappedData.find(inFrame ? currentFrame : 0);
        }

        auto it = frame->second.find(data.label);
        if (it != frame->second.end())
            it->second.times.push(data.time);
        else {
            auto inserted = frame->second.insert(std::make_pair(data.label, DumpData{data.label, data.line, data.file, Vector<time::Duration>()}));
            inserted.first->second.times.push(data.time);
        }
    }

    for (auto& [frameID, frame] : mappedData) {
        auto frameStr = std::to_string(frameID);
        file.write(frameStr);
        file.write(":\n");
        for (auto& mapped : frame) {
            file.write("\t");
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
    }

    return true;
}

void ende::profile::ProfileManager::beginFrame(u32 id) {
    instance()._data.push({"FRAME_BEGIN", id, "FRAME_BEGIN", 0});
}

void ende::profile::ProfileManager::endFrame(u32 id) {
    instance()._data.push({"FRAME_END", id, "FRAME_END", 0});
}