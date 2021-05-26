//
// Created by cstro29 on 20/5/21.
//

#include <Ende/Result.h>
#include <string>

ende::Result<u32, std::string> func(u32 a, u32 b) {
    return Ok(a * b);
}

int main() {

    auto a = func(100, 30);

    return 0;
}