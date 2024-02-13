#ifndef ENDE_COLOUR_H
#define ENDE_COLOUR_H

#include <Ende/platform.h>
#include <array>

namespace ende::util {

    constexpr auto rgb(f32 r, f32 g, f32 b, f32 a = 256) -> std::array<f32, 4> {
        return { r / 256.f, g / 256.f, b / 256.f, a / 256.f};
    }

}

#endif //ENDE_COLOUR_H
