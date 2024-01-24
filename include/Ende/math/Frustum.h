//
// Created by cstro29 on 5/7/21.
//

#ifndef ENDE_FRUSTUM_H
#define ENDE_FRUSTUM_H

#include <Ende/math/Vec.h>
#include <Ende/math/Mat.h>

namespace ende::math {

    class Frustum {
    public:

        Frustum() = default;

        Frustum(const ende::math::Mat4f& matrix);

        void update(const ende::math::Mat4f& matrix);


        bool intersect(const ende::math::Vec3f& point) const;

        bool intersect(const ende::math::Vec3f& point, f32 radius) const;

        const std::array<Vec4f, 6>& planes() const { return _planes; }

        //TODO: add AABB checking

    private:

        std::array<Vec4f, 6> _planes = {};

    };

}

#endif //ENDE_FRUSTUM_H
