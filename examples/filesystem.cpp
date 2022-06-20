
#include <Ende/log/log.h>
#include <Ende/filesystem/Path.h>

#include <iostream>
#include <Ende/profile/profile.h>

#include <Ende/sys/system.h>

#include <Ende/io.h>

#include <Ende/time/time.h>

int main() {

    ende::fs::Path path("/home/cstro29/Documents/Projects/Ende/include/Ende/filesystem/Path.h");

    {
        PROFILE_NAMED("test");
        ende::sys::sleep({5});
    }

    ende::stdout().write(*path);

    ende::log::info("this is some info");

//    char buffer[50];
//    u32 length = ende::stdin().read(buffer);
//    buffer[length-1];
    //auto buffer = ende::stdin().readLn();

    //ende::stdout().write(buffer);

    std::cout << *path << '\n';

    for (auto str : path)
        std::cout << str << ' ';
    std::cout << '\n';

    for (u32 i = 0; i < 1e6; i++) {
        printf("%f\n", static_cast<f64>(ende::time::SystemTime::now().microseconds()) / 1000.f);
        //printf("%f\n", static_cast<f64>(static_cast<f64>(ende::time::SystemTime::now().microseconds())));
    }


    return 0;
}