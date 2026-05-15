//
// Created by cstro29 on 5/7/21.
//

#ifndef ENDE_FRUSTUM_H
#define ENDE_FRUSTUM_H

#include <Ende/math/Mat.h>

namespace ende::math {

    class Frustum {
    public:

        Frustum() = default;

        Frustum(const ende::math::float4x4& matrix);

        void update(const ende::math::float4x4& matrix);


        bool intersect(const ende::math::float3& point) const;

        bool intersect(const ende::math::float3& point, f32 radius) const;

        const std::array<float4, 6>& planes() const { return _planes; }

        //TODO: add AABB checking

    private:

        std::array<float4, 6> _planes = {};

    };

}

#endif //ENDE_FRUSTUM_H
