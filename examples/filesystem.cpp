#include <Ende/platform.h>
#include <Ende/profile.h>
#include <iostream>


#include <filesystem>
#include <thread>

// import ende;
import ende.io;
// import ende.filesystem.File;
import ende.filesystem.FileWatcher;
import ende.util;
import ende.system.system;

using namespace std::chrono_literals;

int main() {

    std::filesystem::path path("/home/cstro29/Documents/Projects/Ende/include/Ende/filesystem/Path.h");

    {
        PROFILE_NAMED("test");
        std::this_thread::sleep_for(400ms);
    }

    ende::stdout().write(path.string());
    const auto colour = ende::util::rgb(200, 10, 5);

    //    char buffer[50];
    //    u32 length = ende::stdin().read(buffer);
    //    buffer[length-1];
    // auto buffer = ende::stdin().readLn();

    // ende::stdout().write(buffer);

    std::cout << path << '\n';

    for (auto str : path)
        std::cout << str << ' ';
    std::cout << '\n';

    //    for (u32 i = 0; i < 1e6; i++) {
    //        printf("%f\n", static_cast<f64>(ende::time::SystemTime::now().microseconds()) / 1000.f);
    //        //printf("%f\n", static_cast<f64>(static_cast<f64>(ende::time::SystemTime::now().microseconds())));
    //    }

    ende::fs::FileWatcher watcher;
    watcher.addWatch("/home/christian/test_watch.txt");

    std::vector<ende::fs::FileWatcher::Event> events = {};
    while (events.empty())
        events = watcher.read();

    for (auto &event : events) {
        std::cout << "path: " << event.path << ", mask: " << (u32)event.mask << '\n';
    }

    return 0;
}
