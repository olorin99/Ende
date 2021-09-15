#ifndef ENDE_INTERSECT_H
#define ENDE_INTERSECT_H

#include <Ende/platform.h>
#include <Ende/math/Vec.h>

namespace ende::math {

    bool intersect(const Vec3f& sphereCenter, f32 radius, const Vec3f& plane) {
        return sphereCenter.dot(plane) < -radius;
    }

    bool intersect(const Vec3f& sphereCenter, f32 radius, const Vec3f& sphereCenter1, f32 radius1) {
        return (sphereCenter - sphereCenter1).lengthSq() < (radius + radius1) * (radius + radius1);
    }

}

#endif //ENDE_INTERSECT_H
