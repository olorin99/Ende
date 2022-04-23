//
// Created by cstro29 on 5/7/21.
//

#include "Ende/math/Frustum.h"

ende::math::Frustum::Frustum(const ende::math::Mat4f &matrix) {
    update(matrix);
}

void ende::math::Frustum::update(const ende::math::Mat4f &matrix) {
    _planes[0].first[0] = matrix[0][2] + matrix[0][3];
    _planes[0].first[1] = matrix[1][2] + matrix[1][3];
    _planes[0].first[2] = matrix[2][2] + matrix[2][3];
    _planes[0].second = matrix[3][2] + matrix[3][3];

    _planes[1].first[0] = matrix[0][3] - matrix[0][2];
    _planes[1].first[1] = matrix[1][3] - matrix[1][2];
    _planes[1].first[2] = matrix[2][3] - matrix[2][2];
    _planes[1].second = matrix[3][3] - matrix[3][2];

    _planes[2].first[0] = matrix[0][1] + matrix[0][3];
    _planes[2].first[1] = matrix[1][1] + matrix[1][3];
    _planes[2].first[2] = matrix[2][1] + matrix[2][3];
    _planes[2].second = matrix[3][1] + matrix[3][3];

    _planes[3].first[0] = matrix[0][3] - matrix[0][1];
    _planes[3].first[1] = matrix[1][3] - matrix[1][1];
    _planes[3].first[2] = matrix[2][3] - matrix[2][1];
    _planes[3].second = matrix[3][3] - matrix[3][1];

    _planes[4].first[0] = matrix[0][0] + matrix[0][3];
    _planes[4].first[1] = matrix[1][0] + matrix[1][3];
    _planes[4].first[2] = matrix[2][0] + matrix[2][3];
    _planes[4].second = matrix[3][0] + matrix[3][3];

    _planes[5].first[0] = matrix[0][3] - matrix[0][0];
    _planes[5].first[1] = matrix[1][3] - matrix[1][0];
    _planes[5].first[2] = matrix[2][3] - matrix[2][0];
    _planes[5].second = matrix[3][3] - matrix[3][0];

    for (i32 i = 0; i < 6; i++) {
        f32 length = _planes[i].first.length();
        _planes[i].first = _planes[i].first / length;
        _planes[i].second /= length;
    }
}

bool ende::math::Frustum::intersect(const ende::math::Vec3f &point) const {
    for (auto& plane : _planes) {
        if (point.dot(plane.first) + plane.second <= 0)
            return false;
    }
    return true;
}

bool ende::math::Frustum::intersect(const ende::math::Vec3f &point, f32 radius) const {
    for (auto& plane : _planes) {
        if (point.dot(plane.first) + plane.second < -radius)
            return false;
    }
    return true;
}