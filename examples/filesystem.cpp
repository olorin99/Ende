//
// Created by cstro29 on 3/6/21.
//

#include <Ende/filesystem/Path.h>
#include <iostream>

int main() {

    ende::fs::Path path("/home/cstro29/Documents/Projects/Ende/include/Ende/filesystem/Path.h");



    std::cout << *path << '\n';

    for (auto str : path)
        std::cout << str << ' ';
    std::cout << '\n';


    return 0;
}