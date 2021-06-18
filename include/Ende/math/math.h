//
// Created by cstro29 on 18/6/21.
//

#ifndef ANINO_MATH_H
#define ANINO_MATH_H


#include <Ende/platform.h>

namespace ende::math {

    constexpr f64 PI = 3.14159;
    constexpr f64 EXP = 2.718;


    // converts degrees to radians
    constexpr inline f64 rad(f64 degrees) {
        return (degrees / 180.f) * PI;
    }

    // converts radians to degrees
    constexpr inline f64 deg(f64 radians) {
        return (radians / PI) * 180.f;
    }

    constexpr inline f64 pow(f64 x, i32 exponent) {
        f64 result = 1;
        while (exponent > 0) {
            if (exponent % 2 == 0) {
                x *= x;
                exponent /= 2;
            } else {
                exponent -= 1;
                result *= x;
            }
        }
        return result;
    }

    constexpr inline u64 factorial(u32 x) {
        u64 res = 1;
        for (u32 i = 2; i <= x; i++)
            res *= i;
        return res;
    }

    template <typename T>
    constexpr inline T abs(T x) {
        return T{} > x ? -x : x;
    }

}

#endif //ANINO_MATH_H
