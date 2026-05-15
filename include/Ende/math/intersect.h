#ifndef ENDE_INTERSECT_H
#define ENDE_INTERSECT_H

#include <Ende/math/Mat.h>
#include <Ende/platform.h>

namespace ende::math {

constexpr inline auto intersect(const float3 &sphereCenter, f32 radius, const float3 &plane) -> bool {
    return sphereCenter.dot(plane) < -radius;
}

constexpr inline auto intersect(const float3 &sphereCenter, f32 radius, const float3 &sphereCenter1, f32 radius1) -> bool {
    return (sphereCenter - sphereCenter1).lengthSquared() < (radius + radius1) * (radius + radius1);
}

} // namespace ende::math

#endif // ENDE_INTERSECT_H
