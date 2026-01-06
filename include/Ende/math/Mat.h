#ifndef ENDE_MAT_H
#define ENDE_MAT_H

#include <Ende/platform.h>
#include <Ende/math/Vec.h>

namespace ende::math {

    template <u32 N, typename T, bool rowMajor = true>
    class Mat {
    public:

        constexpr inline Mat() {
            for (u32 x = 0; x < N; x++) {
                for (u32 y = 0; y < N; y++)
                    (*this)(x, y) = 0;
            }
        }

        explicit constexpr inline Mat(const std::array<std::array<T, N>, N>& array) {
            for (u32 x = 0; x < N; x++) {
                for (u32 y = 0; y < N; y++) {
                    (*this)(x, y) = array[y][x];
                }
            }
        }

        explicit constexpr inline Mat(const std::array<Vec<N, T>, N>& array) {
            for (u32 x = 0; x < N; x++) {
                for (u32 y = 0; y < N; y++) {
                    (*this)(x, y) = array[y][x];
                }
            }
        }

        explicit constexpr inline Mat(const std::array<T, N * N>& array)
            : _data(array)
        {}

        constexpr inline Mat inverse() const {
            Mat inverse;
            inverse._data[0] =  _data[5] * _data[10] * _data[15] - _data[5] * _data[14] * _data[11] - _data[6] * _data[9] * _data[15] + _data[6] * _data[13] * _data[11] + _data[7] * _data[9] * _data[14] - _data[7] * _data[13] * _data[10];
            inverse._data[ 1] = -_data[1] * _data[10] * _data[15] + _data[1] * _data[14] * _data[11] + _data[2] * _data[9] * _data[15] - _data[2] * _data[13] * _data[11] - _data[3] * _data[9] * _data[14] + _data[3] * _data[13] * _data[10];
            inverse._data[ 2] =  _data[1] * _data[ 6] * _data[15] - _data[1] * _data[14] * _data[ 7] - _data[2] * _data[5] * _data[15] + _data[2] * _data[13] * _data[ 7] + _data[3] * _data[5] * _data[14] - _data[3] * _data[13] * _data[ 6];
            inverse._data[ 3] = -_data[1] * _data[ 6] * _data[11] + _data[1] * _data[10] * _data[ 7] + _data[2] * _data[5] * _data[11] - _data[2] * _data[ 9] * _data[ 7] - _data[3] * _data[5] * _data[10] + _data[3] * _data[ 9] * _data[ 6];
            inverse._data[ 4] = -_data[4] * _data[10] * _data[15] + _data[4] * _data[14] * _data[11] + _data[6] * _data[8] * _data[15] - _data[6] * _data[12] * _data[11] - _data[7] * _data[8] * _data[14] + _data[7] * _data[12] * _data[10];
            inverse._data[ 5] =  _data[0] * _data[10] * _data[15] - _data[0] * _data[14] * _data[11] - _data[2] * _data[8] * _data[15] + _data[2] * _data[12] * _data[11] + _data[3] * _data[8] * _data[14] - _data[3] * _data[12] * _data[10];
            inverse._data[ 6] = -_data[0] * _data[ 6] * _data[15] + _data[0] * _data[14] * _data[ 7] + _data[2] * _data[4] * _data[15] - _data[2] * _data[12] * _data[ 7] - _data[3] * _data[4] * _data[14] + _data[3] * _data[12] * _data[ 6];
            inverse._data[ 7] =  _data[0] * _data[ 6] * _data[11] - _data[0] * _data[10] * _data[ 7] - _data[2] * _data[4] * _data[11] + _data[2] * _data[ 8] * _data[ 7] + _data[3] * _data[4] * _data[10] - _data[3] * _data[ 8] * _data[ 6];
            inverse._data[ 8] =  _data[4] * _data[ 9] * _data[15] - _data[4] * _data[13] * _data[11] - _data[5] * _data[8] * _data[15] + _data[5] * _data[12] * _data[11] + _data[7] * _data[8] * _data[13] - _data[7] * _data[12] * _data[ 9];
            inverse._data[ 9] = -_data[0] * _data[ 9] * _data[15] + _data[0] * _data[13] * _data[11] + _data[1] * _data[8] * _data[15] - _data[1] * _data[12] * _data[11] - _data[3] * _data[8] * _data[13] + _data[3] * _data[12] * _data[ 9];
            inverse._data[10] =  _data[0] * _data[ 5] * _data[15] - _data[0] * _data[13] * _data[ 7] - _data[1] * _data[4] * _data[15] + _data[1] * _data[12] * _data[ 7] + _data[3] * _data[4] * _data[13] - _data[3] * _data[12] * _data[ 5];
            inverse._data[11] = -_data[0] * _data[ 5] * _data[11] + _data[0] * _data[ 9] * _data[ 7] + _data[1] * _data[4] * _data[11] - _data[1] * _data[ 8] * _data[ 7] - _data[3] * _data[4] * _data[ 9] + _data[3] * _data[ 8] * _data[ 5];
            inverse._data[12] = -_data[4] * _data[ 9] * _data[14] + _data[4] * _data[13] * _data[10] + _data[5] * _data[8] * _data[14] - _data[5] * _data[12] * _data[10] - _data[6] * _data[8] * _data[13] + _data[6] * _data[12] * _data[ 9];
            inverse._data[13] =  _data[0] * _data[ 9] * _data[14] - _data[0] * _data[13] * _data[10] - _data[1] * _data[8] * _data[14] + _data[1] * _data[12] * _data[10] + _data[2] * _data[8] * _data[13] - _data[2] * _data[12] * _data[ 9];
            inverse._data[14] = -_data[0] * _data[ 5] * _data[14] + _data[0] * _data[13] * _data[ 6] + _data[1] * _data[4] * _data[14] - _data[1] * _data[12] * _data[ 6] - _data[2] * _data[4] * _data[13] + _data[2] * _data[12] * _data[ 5];
            inverse._data[15] =  _data[0] * _data[ 5] * _data[10] - _data[0] * _data[ 9] * _data[ 6] - _data[1] * _data[4] * _data[10] + _data[1] * _data[ 8] * _data[ 6] + _data[2] * _data[4] * _data[ 9] - _data[2] * _data[ 8] * _data[ 5];

            T det = _data[0] * inverse._data[0] + _data[4] * inverse._data[1] + _data[8] * inverse._data[2] + _data[12] * inverse._data[3];

            det = 1.f / det;
            return inverse * det;
        }

        constexpr inline Mat transpose() const {
            Mat result;
            for (u32 x = 0; x < N; x++) {
                for (u32 y = 0; y < N; y++)
                    result(x, y) = (*this)(y, x);
            }
            return result;
        }

        template <u8 O>
        constexpr inline Vec<O, T> transform(const Vec<O, T>& rhs) const {
            Vec<O, T> result;
            for (u32 row = 0; row < O; row++) {
                result[row] = T(0);
                for (u8 col = 0; col < O; col++) {
                    if constexpr (rowMajor) result[row] += (*this)(col, row) * rhs[col];
                    else result[row] += (*this)(row, col) * rhs[col];
                }
            }
            return result;
        }


        constexpr inline Mat operator*(const Mat& rhs) const {
            Mat result;
            for (u32 x = 0; x < N; x++) {
                for (u32 y = 0; y < N; y++) {
                    T tmp = T(0);
                    for (u32 i = 0; i < N; i++) {
                        tmp += (*this)(i, y) * rhs(x, i);
                    }
                    result(x, y) = tmp;
                }
            }
            return result;
        }

        constexpr inline Mat operator+(const Mat& rhs) const {
            Mat result;
            for (u32 x = 0; x < N; x++) {
                for (u32 y = 0; y < N; y++)
                    result(x, y) = (*this)(x, y) + rhs(x, y);
            }
            return result;
        }

        constexpr inline Mat operator-(const Mat& rhs) const {
            Mat result;
            for (u32 x = 0; x < N; x++) {
                for (u32 y = 0; y < N; y++)
                    result(x, y) = (*this)(x, y) - rhs(x, y);
            }
            return result;
        }

        constexpr inline Mat operator+(const T& rhs) const {
            Mat result;
            for (u32 x = 0; x < N; x++) {
                for (u32 y = 0; y < N; y++)
                    result(x, y) = (*this)(x, y) + rhs;
            }
            return result;
        }

        constexpr inline Mat operator-(const T& rhs) const {
            Mat result;
            for (u32 x = 0; x < N; x++) {
                for (u32 y = 0; y < N; y++)
                    result(x, y) = (*this)(x, y) - rhs;
            }
            return result;
        }

        constexpr inline Mat operator*(const T& rhs) const {
            Mat result;
            for (u32 x = 0; x < N; x++) {
                for (u32 y = 0; y < N; y++)
                    result(x, y) = (*this)(x, y) * rhs;
            }
            return result;
        }

        constexpr inline Mat operator/(const T& rhs) const {
            Mat result;
            for (u32 x = 0; x < N; x++) {
                for (u32 y = 0; y < N; y++) {
                    result(x, y) = (*this)(x, y) / rhs;
                }
            }
            return result;
        }

        constexpr inline T* operator[](u8 index) {
            assert(N >= index);
            return &_data[index];
        }

        constexpr inline const T* operator[](u8 index) const {
            assert(N >= index);
            return &_data[index];
        }

        constexpr inline T& operator()(const u32 x, const u32 y) {
            if constexpr (rowMajor) return _data[x + y * N];
            else return _data[x * N + y];
        }

        constexpr inline const T& operator()(const u32 x, const u32 y) const {
            if constexpr (rowMajor) return _data[x + y * N];
            else return _data[x * N + y];
        }

        constexpr inline bool operator==(const Mat& rhs) const {
            for (u32 x = 0; x < N; x++) {
                for (u32 y = 0; y < N; y++)
                    if ((*this)(x, y) != rhs(x, y)) return false;
            }
            return true;
        }

        constexpr inline bool operator!=(const Mat& rhs) const {
            return !(*this == rhs);
        }

    private:

        std::array<T, N * N> _data;

    };

    typedef Mat<4, f32> Mat4f;
    typedef Mat<4, i32> Mat4i;

    template <u8 N, typename T>
    constexpr inline Mat<N, T> identity() {
        Mat<N, T> result;
        for (u32 x = 0; x < N; x++) {
            for (u32 y = 0; y < N; y++) {
                if (x == y)
                    result(x, y) = T(1);
                else
                    result(x, y) = T(0);
            }
        }
        return result;
    }

    template <u8 N, typename T>
    constexpr inline Mat<N, T> scale(const Vec<N - 1, T>& scale) {
        Mat<N, T> result;
        for (u32 x = 0; x < N; x++) {
            for (u32 y = 0; y < N; y++) {
                if (x == y && x != N - 1)
                    result(x, y) = scale[x];
                else
                    result(x, y) = T(0);
            }
        }
        result(N - 1, N - 1) = T(1);
        return result;
    }

    template <u8 N, typename T>
    constexpr inline Mat<N, T> translation(const Vec<N - 1, T>& pos) {
        Mat<N, T> result;
        for (u32 x = 0; x < N; x++) {
            for (u32 y = 0; y < N; y++) {
                if (x == N - 1 && y != N - 1)
                    result(x, y) = pos[y];
                else if (x == y)
                    result(x, y) = T(1);
                else
                    result(x, y) = T(0);
            }
        }
        result(N - 1, N - 1) = T(1);
        return result;
    }

    template <typename T>
    constexpr inline Mat<4, T> fromVec(const Vec<3, T>& n, const Vec<3, T>& v, const Vec<3, T>& u) {
        Mat<4, T> result;
        result(0, 0) = u[0]; result(1, 0) = u[1]; result(2, 0) = u[2]; result(3, 0) = T(0);
        result(0, 1) = v[0]; result(1, 1) = v[1]; result(2, 1) = v[2]; result(3, 1) = T(0);
        result(0, 2) = n[0]; result(1, 2) = n[1]; result(2, 2) = n[2]; result(3, 2) = T(0);
        result(0, 3) = T(0); result(1, 3) = T(0); result(2, 3) = T(0); result(3, 3) = T(1);
        return result;
    }

    template <typename T>
    constexpr inline Mat<4, T> perspective(T fov, T aspect, T near, T far) {
        const f32 tanHalfFov = std::tan(fov / 2.f);

        Mat<4, T> result;
        result(0, 0) = 1.f / (tanHalfFov * aspect);
        result(0, 1) = 0.f;
        result(0, 2) = 0.f;
        result(0, 3) = 0.f;

        result(1, 0) = 0.f;
        result(1, 1) = 1.f / tanHalfFov;
        result(1, 2) = 0.f;
        result(1, 3) = 0.f;

        result(2, 0) = 0.f;
        result(2, 1) = 0.f;
        result(2, 2) = far / (far - near);
        result(2, 3) = T(1);

        result(3, 0) = 0.f;
        result(3, 1) = 0.f;
        result(3, 2) = -(near * far) / (far - near);
        result(3, 3) = 0.f;

        return result;
    }

    template <typename T>
    constexpr inline Mat<4, T> orthographic(T left, T right, T bottom, T top, T near, T far) {
        const T width = right - left;
        const T height = top - bottom;
        const T depth = far - near;

        Mat<4, T> result;
        result(0, 0) = T(2) / width;
        result(0, 1) = T(0);
        result(0, 2) = T(0);
        result(0, 3) = T(0);

        result(1, 0) = T(0);
        result(1, 1) = -T(2) / height;
        result(1, 2) = T(0);
        result(1, 3) = T(0);

        result(2, 0) = T(0);
        result(2, 1) = T(0);
//        result[2][2] = -T(1);
        result(2, 2) = T(1) / (far - near);
        result(2, 3) = T(0);

        result(3, 0) = -(right + left) / width;
        result(3, 1) = -(top + bottom) / height;
//        result[3][2] = T(0);
        result(3, 2) = near / (near - far);
        result(3, 3) = T(1);

        return result;
    }

    template <typename T>
    inline Mat<4, T> lookAt(const Vec3f& pos, const Vec3f& target, const Vec3f& origUp) {

        const Vec3f front = (target - pos).unit();
        const Vec3f right = origUp.cross(front).unit();
        const Vec3f up = front.cross(right);

        Mat<4, T> result;
        result(0, 0) = right.x();
        result(0, 1) = right.y();
        result(0, 2) = right.z();
        result(0, 3) = T(0);

        result(1, 0) = up.x();
        result(1, 1) = up.y();
        result(1, 2) = up.z();
        result(1, 3) = T(0);

        result(2, 0) = front.x();
        result(2, 1) = front.y();
        result(2, 2) = front.z();
        result(2, 3) = T(0);

        result(3, 0) = pos.x();
        result(3, 1) = pos.y();
        result(3, 2) = pos.z();
        result(3, 3) = T(1);

        return result;
    }

}

#endif //ENDE_MAT_H
