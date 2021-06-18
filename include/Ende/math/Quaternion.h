//
// Created by cstro29 on 18/6/21.
//

#ifndef ANINO_QUATERNION_H
#define ANINO_QUATERNION_H

#include <Ende/platform.h>
#include <Ende/math/math.h>
#include <Ende/math/Vec.h>
#include <Ende/math/Mat.h>

namespace ende::math {

    class Quaternion {
    public:

        constexpr inline Quaternion()
                : _data{0, 0, 0, 0}
        {}

        constexpr inline Quaternion(f32 x, f32 y, f32 z, f32 w)
                : _data{x, y, z, w}
        {}

        constexpr inline Quaternion(const std::array<f32, 4>& rhs)
                : _data(rhs)
        {}

        constexpr inline Quaternion(const Vec3f& axis, f32 angle)
                : _data{0, 0, 0, 0}
        {
            const f32 sinHalfAngle = std::sin(angle / 2.f);
            const f32 cosHalfAngle = std::cos(angle / 2.f);
            _data[0] = axis.x() * sinHalfAngle;
            _data[1] = axis.y() * sinHalfAngle;
            _data[2] = axis.z() * sinHalfAngle;
            _data[3] = cosHalfAngle;
        }


        inline Quaternion(const Mat4f& rot)
                : _data{0, 0, 0, 0}
        {
            f32 _w = std::sqrt(1 + rot[0][0] + rot[1][1] + rot[2][2]) / 2;
            f32 _x = (rot[2][1] - rot[1][2]) / (4 * _w);
            f32 _y = (rot[0][2] - rot[2][0]) / (4 * _w);
            f32 _z = (rot[1][0] - rot[0][1]) / (4 * _w);
            _data[0] = _x;
            _data[1] = _y;
            _data[2] = _z;
            _data[3] = _w;
        }



        // functions

        constexpr inline f32 dot(const Quaternion& rhs) const {
            f32 result = 0;
            for (u8 i = 0; i < 4; i++)
                result += _data[i] * rhs._data[i];
            return result;
        }

        constexpr inline f32 lengthSq() const {
            return dot(*this);
        }

        constexpr inline f32 length() const {
            return std::sqrt(lengthSq());
        }

        constexpr inline Quaternion unit() const {
            return *this / length();
        }



        constexpr inline Quaternion lerp(const Quaternion& rhs, f32 factor) const {
            return (rhs - *this) * factor + *this;
        }

        constexpr inline Quaternion nLerp(const Quaternion& r, f32 factor, bool shortestPath = true) const {
            Quaternion dest;
            if (shortestPath && dot(r) < 0)
                dest = r * -1;
            else
                dest = r;
            return lerp(dest, factor).unit();
        }

        constexpr inline Quaternion sLerp(const Quaternion& r, f32 factor, bool shortestPath = true) const {
            const f32 EPSILON = 1e3;
            f32 cos = dot(r);
            Quaternion dest;
            if (shortestPath && cos < 0) {
                cos *= -1;
                dest = r * -1;
            } else
                dest = r;
            if (abs(cos) > (1 - EPSILON))
                return nLerp(dest, factor, false);
            f32 sin = std::sqrt(1.f - cos * cos);
            f32 angle = std::atan2(sin, cos);
            f32 invSin = 1.f / sin;
            f32 srcFactor = std::sin((1.f - factor) * angle) * invSin;
            f32 destFactor = std::sin(factor * angle) * invSin;
            return (*this) * srcFactor + dest * destFactor;
        }



        inline Vec3f rotate(const Vec3f& rhs) const {
            Quaternion conj = conjugate();
            Quaternion w = Quaternion(*this * rhs) * conj;
            return Vec3f({w.x(), w.y(), w.z()});
        }

        constexpr inline Quaternion conjugate() const {
            return Quaternion(-x(), -y(), -z(), w());
        }

        constexpr inline Quaternion inverse() const {
            return unit().conjugate();
        }

        inline Mat4f toMat() const {
            Mat4f result;

            result[0][0] = 1.f - (2.f * y() * y()) - (2.f * z() * z());
            result[0][1] = (2.f * x() * y()) + (2.f * z() * w());
            result[0][2] = (2.f * x() * z()) - (2.f * y() * w());
            result[0][3] = 0;

            result[1][0] = (2.f * x() * y()) - (2.f * z() * w());
            result[1][1] = 1.f - (2.f * x() * x()) - (2.f * z() * z());
            result[1][2] = (2.f * y() * z()) + (2.f * x() * w());
            result[1][3] = 0;

            result[2][0] = (2.f * x() * z()) + (2.f * y() * w());
            result[2][1] = (2.f * y() * z()) - (2.f * x() * w());
            result[2][2] = 1.f - (2.f * x() * x()) - (2.f * y() * y());
            result[2][3] = 0;

            result[3][0] = 0;
            result[3][1] = 0;
            result[3][2] = 0;
            result[3][3] = 1;

            return result;
        }




        // accessors

        constexpr inline f32 x() const {
            return _data[0];
        }

        constexpr inline f32 y() const {
            return _data[1];
        }

        constexpr inline f32 z() const {
            return _data[2];
        }

        constexpr inline f32 w() const {
            return _data[3];
        }


        inline Vec3f front() const {
            return rotate(Vec3f({0, 0, 1}));
        }

        inline Vec3f back() const {
            return rotate(Vec3f({0, 0, -1}));
        }

        inline Vec3f up() const {
            return rotate(Vec3f({0, 1, 0}));
        }

        inline Vec3f down() const {
            return rotate(Vec3f({0, -1, 0}));
        }

        inline Vec3f left() const {
            return rotate(Vec3f({-1, 0, 0}));
        }

        inline Vec3f right() const {
            return rotate(Vec3f({1, 0, 0}));
        }




        // operators

        constexpr inline Quaternion operator+(const Quaternion& rhs) const {
            Quaternion result;
            for (u8 i = 0; i < 4; i++)
                result._data[i] = _data[i] + rhs._data[i];
            return result;
        }

        constexpr inline Quaternion operator-(const Quaternion& rhs) const {
            Quaternion result;
            for (u8 i = 0; i < 4; i++)
                result._data[i] = _data[i] - rhs._data[i];
            return result;
        }

        constexpr inline Quaternion operator*(const Quaternion& rhs) const {
            const f32 _w = (w() * rhs.w()) - (x() * rhs.x()) - (y() * rhs.y()) - (z() * rhs.z());
            const f32 _x = (x() * rhs.w()) + (w() * rhs.x()) + (y() * rhs.z()) - (z() * rhs.y());
            const f32 _y = (y() * rhs.w()) + (w() * rhs.y()) + (z() * rhs.x()) - (x() * rhs.z());
            const f32 _z = (z() * rhs.w()) + (w() * rhs.z()) + (x() * rhs.y()) - (y() * rhs.x());
            return Quaternion(_x, _y, _z, _w);
        }



        constexpr inline Quaternion operator*(const Vec3f& rhs) const {
            const f32 _w = -(x() * rhs.x()) - (y() * rhs.y()) - (z() * rhs.z());
            const f32 _x = (w() * rhs.x()) + (y() * rhs.z()) - (z() * rhs.y());
            const f32 _y = (w() * rhs.y()) + (z() * rhs.x()) - (x() * rhs.z());
            const f32 _z = (w() * rhs.z()) + (x() * rhs.y()) - (y() * rhs.x());
            return Quaternion(_x, _y, _z, _w);
        }


        constexpr inline Quaternion operator+(f32 rhs) const {
            Quaternion result;
            for (u8 i = 0; i < 4; i++)
                result._data[i] = _data[i] + rhs;
            return result;
        }

        constexpr inline Quaternion operator-(f32 rhs) const {
            Quaternion result;
            for (u8 i = 0; i < 4; i++)
                result._data[i] = _data[i] - rhs;
            return result;
        }

        constexpr inline Quaternion operator*(f32 rhs) const {
            Quaternion result;
            for (u8 i = 0; i < 4; i++)
                result._data[i] = _data[i] * rhs;
            return result;
        }

        constexpr inline Quaternion operator/(f32 rhs) const {
            Quaternion result;
            for (u8 i = 0; i < 4; i++)
                result._data[i] = _data[i] / rhs;
            return result;
        }




        constexpr inline f32& operator[](u8 index) {
            assert(4 >= index);
            return _data[index];
        }

        constexpr inline const f32& operator[](u8 index) const {
            assert(4 >= index);
            return _data[index];
        }


        constexpr inline bool operator==(const Quaternion& rhs) const {
            for (u8 i = 0; i < 4; i++) {
                if (_data[i] != rhs._data[i])
                    return false;
            }
            return true;
        }

        constexpr inline bool operator!=(const Quaternion& rhs) const {
            return !(*this == rhs);
        }



    private:

        std::array<f32, 4> _data;

    };

}

#endif //ANINO_QUATERNION_H
