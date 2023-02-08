//
// Created by cstro29 on 5/7/21.
//

#include "Ende/math/Frustum.h"

ende::math::Frustum::Frustum(const ende::math::Mat4f &matrix) {
    update(matrix);
}

void ende::math::Frustum::update(const ende::math::Mat4f &matrix) {
    // left
    _planes[0][0] = matrix[0][3] + matrix[0][0];
    _planes[0][1] = matrix[1][3] + matrix[1][0];
    _planes[0][2] = matrix[2][3] + matrix[2][0];
    _planes[0][3] = matrix[3][3] + matrix[3][0];

    // right
    _planes[1][0] = matrix[0][3] - matrix[0][0];
    _planes[1][1] = matrix[1][3] - matrix[1][0];
    _planes[1][2] = matrix[2][3] - matrix[2][0];
    _planes[1][3] = matrix[3][3] - matrix[3][0];

    // top
    _planes[2][0] = matrix[0][3] - matrix[0][1];
    _planes[2][1] = matrix[1][3] - matrix[1][1];
    _planes[2][2] = matrix[2][3] - matrix[2][1];
    _planes[2][3] = matrix[3][3] - matrix[3][1];

    // bottom
    _planes[3][0] = matrix[0][3] + matrix[0][1];
    _planes[3][1] = matrix[1][3] + matrix[1][1];
    _planes[3][2] = matrix[2][3] + matrix[2][1];
    _planes[3][3] = matrix[3][3] + matrix[3][1];

    // back
    _planes[4][0] = matrix[0][3] + matrix[0][2];
    _planes[4][1] = matrix[1][3] + matrix[1][2];
    _planes[4][2] = matrix[2][3] + matrix[2][2];
    _planes[4][3] = matrix[3][3] + matrix[3][2];

    // front
    _planes[5][0] = matrix[0][3] - matrix[0][2];
    _planes[5][1] = matrix[1][3] - matrix[1][2];
    _planes[5][2] = matrix[2][3] - matrix[2][2];
    _planes[5][3] = matrix[3][3] - matrix[3][2];

    for (i32 i = 0; i < 6; i++) {
        f32 length = _planes[i].xyz().length();
        _planes[i] = _planes[i] / length;
    }
}

bool ende::math::Frustum::intersect(const ende::math::Vec3f &point) const {
    for (auto& plane : _planes) {
        if (point.dot(plane.xyz()) + plane.w() <= 0)
            return false;
    }
    return true;
}

bool ende::math::Frustum::intersect(const ende::math::Vec3f &point, f32 radius) const {
    for (auto& plane : _planes) {
        if (point.dot(plane.xyz()) + plane.w() < -radius)
            return false;
    }
    return true;
}