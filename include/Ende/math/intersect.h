#ifndef ENDE_INTERSECT_H
#define ENDE_INTERSECT_H

#include <Ende/platform.h>
#include <Ende/math/Mat.h>

namespace ende::math {

    constexpr inline auto intersect(const float3& sphereCenter, f32 radius, const float3& plane) -> bool {
        return sphereCenter.dot(plane) < -radius;
    }

    constexpr inline auto intersect(const float3& sphereCenter, f32 radius, const float3& sphereCenter1, f32 radius1) -> bool {
        return (sphereCenter - sphereCenter1).lengthSquared() < (radius + radius1) * (radius + radius1);
    }

}

#endif //ENDE_INTERSECT_H
