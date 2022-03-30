
#include <Ende/filesystem/Path.h>
#include <iostream>

#include <Ende/profile/profile.h>
#include <Ende/sys/system.h>

int main() {

    ende::fs::Path path("/home/cstro29/Documents/Projects/Ende/include/Ende/filesystem/Path.h");

    {
        PROFILE_NAMED("test");
        ende::sys::sleep({5});
    }

    std::cout << *path << '\n';

    for (auto str : path)
        std::cout << str << ' ';
    std::cout << '\n';


    return 0;
}