//
// Created by cstro29 on 18/6/21.
//

#ifndef ANINO_VEC_H
#define ANINO_VEC_H

#include <Ende/platform.h>
#include <cassert>
#include <cmath>
#include <array>

namespace ende::math {

    template <u8 N, typename T>
    class Vec {
    public:

        constexpr inline Vec() = default;

        constexpr inline Vec(std::initializer_list<T>&& list)
//            : _data(std::forward<std::initializer_list<T>>(list))
        {
            u8 i = 0;
            for (auto& l : list)
                _data[i++] = l;
        }

        constexpr inline Vec(const std::array<T, N>& array)
            : _data(array)
        {}

        constexpr inline Vec(Vec&& vec) noexcept {
            std::swap(_data, vec._data);
        }

        constexpr inline Vec(const Vec& vec)
            : _data(vec._data)
        {}

        constexpr inline Vec& operator=(Vec&& rhs) {
            std::swap(_data, rhs._data);
            return *this;
        }

        constexpr inline Vec& operator=(const Vec& rhs) {
            _data = rhs._data;
            return *this;
        }


        constexpr inline T dot(const Vec& rhs) const {
            T result = T(0);
            for (u8 i = 0; i < N; i++)
                result += _data[i] * rhs._data[i];
            return result;
        }

        constexpr inline T lengthSq() const {
            return dot(*this);
        }

        constexpr inline T length() const {
            return std::sqrt(lengthSq());
        }

        constexpr inline Vec unit() const {
            return *this / length();
        }

        constexpr inline Vec reflect(const Vec& normal) const {
            return *this - (normal * (dot(normal) * 2));
        }

        constexpr inline Vec project(const Vec& normal) const {
            return normal * (dot(normal) / normal.lengthSq());
        }

        constexpr inline Vec lerp(const Vec& rhs, f32 factor) const {
            return (rhs - *this) * factor + *this;
        }

        constexpr inline Vec cross(const Vec& rhs) const {
            static_assert(N == 3, "cross requires 3d space");
            T _x = _data[1] * rhs._data[2] - _data[2] * rhs._data[1];
            T _y = _data[2] * rhs._data[0] - _data[0] * rhs._data[2];
            T _z = _data[0] * rhs._data[1] - _data[1] * rhs._data[0];
            return Vec({_x, _y, _z});
        }


        constexpr inline T x() const {
            static_assert(N >= 1, "x requires 1 element in vec");
            return _data[0];
        }

        constexpr inline T y() const {
            static_assert(N >= 2, "y requires 2 elements in vec");
            return _data[1];
        }

        constexpr inline T z() const {
            static_assert(N >= 3, "z requires 3 elements in vec");
            return _data[2];
        }

        constexpr inline T w() const {
            static_assert(N >= 4, "w requires 4 elements in vec");
            return _data[3];
        }


        constexpr inline Vec<2, T> xy() const {
            static_assert(N > 2, "xy requires 2 elements in vec");
            return {_data[0], _data[1]};
        }

        constexpr inline Vec<2, T> yz() const {
            static_assert(N > 3, "yz requires 3 elements in vec");
            return {_data[1], _data[2]};
        }

        constexpr inline Vec<2, T> xz() const {
            static_assert(N > 3, "xz requires 2 elements in vec");
            return {_data[0], _data[2]};
        }

        constexpr inline Vec<3, T> xyz() const {
            static_assert(N > 3, "xyz requires 2 elements in vec");
            return {_data[0], _data[1], _data[2]};
        }

        constexpr inline Vec<4, T> xyzw() const {
            static_assert(N > 2, "xyzw requires 2 elements in vec");
            return {_data[0], _data[1], _data[2], _data[3]};
        }




        constexpr inline Vec operator+(const Vec& rhs) const {
            Vec result;
            for (u8 i = 0; i < N; i++)
                result._data[i] = _data[i] + rhs._data[i];
            return result;
        }

        constexpr inline Vec operator-(const Vec& rhs) const {
            Vec result;
            for (u8 i = 0; i < N; i++)
                result._data[i] = _data[i] - rhs._data[i];
            return result;
        }

        constexpr inline Vec operator+(const T& rhs) const {
            Vec result;
            for (u8 i = 0; i < N; i++)
                result._data[i] = _data[i] + rhs;
            return result;
        }

        constexpr inline Vec operator-(const T& rhs) const {
            Vec result;
            for (u8 i = 0; i < N; i++)
                result._data[i] = _data[i] - rhs;
            return result;
        }

        constexpr inline Vec operator*(const T& rhs) const {
            Vec result;
            for (u8 i = 0; i < N; i++)
                result._data[i] = _data[i] * rhs;
            return result;
        }

        constexpr inline Vec operator/(const T& rhs) const {
            Vec result;
            for (u8 i = 0; i < N; i++)
                result._data[i] = _data[i] / rhs;
            return result;
        }

        constexpr inline T& operator[](u8 index) {
            assert(N >= index);
            return _data[index];
        }

        constexpr inline const T& operator[](u8 index) const {
            assert(N >= index);
            return _data[index];
        }

        constexpr inline bool operator==(const Vec& rhs) const {
            return _data == rhs._data;
        }

        constexpr inline bool operator!=(const Vec& rhs) const {
            return !(*this == rhs);
        }

    private:

        std::array<T, N> _data;

    };

    typedef Vec<3, f32> Vec3f;
    typedef Vec<4, f32> Vec4f;
    typedef Vec<3, i32> Vec3i;
    typedef Vec<4, i32> Vec4i;

}

#endif //ANINO_VEC_H
