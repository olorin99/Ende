//
// Created by cstro29 on 18/6/21.
//

#ifndef ANINO_MAT_H
#define ANINO_MAT_H

#include <Ende/platform.h>
#include <Ende/math/Vec.h>

namespace ende::math {

    template <u8 N, typename T>
    class Mat {
    public:

        constexpr inline Mat() {
            for (u8 i = 0; i < N; i++) {
                for (u8 j = 0; j < N; j++)
                    _data[i][j] = 0;
            }
        }

        constexpr inline Mat(const std::array<std::array<T, N>, N>& array) {
            for (u8 i = 0; i < N; i++) {
                for (u8 j = 0; j < N; j++) {
                    _data[i][j] = array[i][j];
                }
            }
        }

        constexpr inline Mat(const std::array<Vec<N, T>, N>& array) {
            for (u8 i = 0; i < N; i++) {
                for (u8 j = 0; j < N; j++) {
                    _data[i][j] = array[i][j];
                }
            }
        }

        constexpr inline Mat inverse() const {
            T Coef00 = _data[2][2] * _data[3][3] - _data[3][2] * _data[2][3];
            T Coef02 = _data[1][2] * _data[3][3] - _data[3][2] * _data[1][3];
            T Coef03 = _data[1][2] * _data[2][3] - _data[2][2] * _data[1][3];

            T Coef04 = _data[2][1] * _data[3][3] - _data[3][1] * _data[2][3];
            T Coef06 = _data[1][1] * _data[3][3] - _data[3][1] * _data[1][3];
            T Coef07 = _data[1][1] * _data[2][3] - _data[2][1] * _data[1][3];

            T Coef08 = _data[2][1] * _data[3][2] - _data[3][1] * _data[2][2];
            T Coef10 = _data[1][1] * _data[3][2] - _data[3][1] * _data[1][2];
            T Coef11 = _data[1][1] * _data[2][2] - _data[2][1] * _data[1][2];

            T Coef12 = _data[2][0] * _data[3][3] - _data[3][0] * _data[2][3];
            T Coef14 = _data[1][0] * _data[3][3] - _data[3][0] * _data[1][3];
            T Coef15 = _data[1][0] * _data[2][3] - _data[2][0] * _data[1][3];

            T Coef16 = _data[2][0] * _data[3][2] - _data[3][0] * _data[2][2];
            T Coef18 = _data[1][0] * _data[3][2] - _data[3][0] * _data[1][2];
            T Coef19 = _data[1][0] * _data[2][2] - _data[2][0] * _data[1][2];

            T Coef20 = _data[2][0] * _data[3][1] - _data[3][0] * _data[2][1];
            T Coef22 = _data[1][0] * _data[3][1] - _data[3][0] * _data[1][1];
            T Coef23 = _data[1][0] * _data[2][1] - _data[2][0] * _data[1][1];

            Vec<4, T> Fac0({Coef00, Coef00, Coef02, Coef03});
            Vec<4, T> Fac1({Coef04, Coef04, Coef06, Coef07});
            Vec<4, T> Fac2({Coef08, Coef08, Coef10, Coef11});
            Vec<4, T> Fac3({Coef12, Coef12, Coef14, Coef15});
            Vec<4, T> Fac4({Coef16, Coef16, Coef18, Coef19});
            Vec<4, T> Fac5({Coef20, Coef20, Coef22, Coef23});

            Vec<4, T> Vec0({_data[1][0], _data[0][0], _data[0][0], _data[0][0]});
            Vec<4, T> Vec1({_data[1][1], _data[0][1], _data[0][1], _data[0][1]});
            Vec<4, T> Vec2({_data[1][2], _data[0][2], _data[0][2], _data[0][2]});
            Vec<4, T> Vec3({_data[1][3], _data[0][3], _data[0][3], _data[0][3]});

            Vec<4, T> Inv0(Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2);
            Vec<4, T> Inv1(Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4);
            Vec<4, T> Inv2(Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5);
            Vec<4, T> Inv3(Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5);

            Vec<4, T> SignA({+1, -1, +1, -1});
            Vec<4, T> SignB({-1, +1, -1, +1});
            Mat<4, T> Inverse({Inv0 * SignA, Inv1 * SignB, Inv2 * SignA, Inv3 * SignB});

            Vec<4, T> Row0({Inverse[0][0], Inverse[1][0], Inverse[2][0], Inverse[3][0]});

            Vec<4, T> Dot0(Row0 * Vec<4, T>({_data[0][0], _data[0][1], _data[0][2], _data[0][3]}));
            T Dot1 = (Dot0.x() + Dot0.y()) + (Dot0.z() + Dot0.w());

            T OneOverDeterminant = static_cast<T>(1) / Dot1;

            return Inverse * OneOverDeterminant;
        }

        constexpr inline Mat transpose() const {
            Mat result;
            for (u8 i = 0; i < N; i++) {
                for (u8 j = 0; j < N; j++)
                    result._data[i][j] = _data[j][i];
            }
            return result;
        }

        template <u8 O>
        constexpr inline Vec<O, T> transform(const Vec<O, T>& rhs) const {
            Vec<O, T> result;
            for (u8 row = 0; row < O; row++) {
                result[row] = T(0);
                for (u8 col = 0; col < O; col++) {
                    result[row] += _data[col][row] * rhs[col];
                }
            }
            return result;
        }


        constexpr inline Mat operator*(const Mat& rhs) const {
            Mat result;
            for (u8 i = 0; i < N; i++) { //for each column
                for (u8 j = 0; j < N; j++) { //for each row
                    result[i][j] = T(0);
                    for (u8 k = 0; k < N; k++)
                        result[i][j] += _data[k][j] * rhs[i][k]; //sum of j row and i column
                }
            }
            return result;
        }

        constexpr inline Mat operator+(const Mat& rhs) const {
            Mat result;
            for (u8 i = 0; i < N; i++) {
                for (u8 j = 0; j < N; j++)
                    result[i][j] = _data[i][j] + rhs._data[i][j];
            }
            return result;
        }

        constexpr inline Mat operator-(const Mat& rhs) const {
            Mat result;
            for (u8 i = 0; i < N; i++) {
                for (u8 j = 0; j < N; j++)
                    result[i][j] = _data[i][j] - rhs._data[i][j];
            }
            return result;
        }

        constexpr inline Mat operator+(const T& rhs) const {
            Mat result;
            for (u8 i = 0; i < N; i++) {
                for (u8 j = 0; j < N; j++)
                    result[i][j] = _data[i][j] + rhs;
            }
            return result;
        }

        constexpr inline Mat operator-(const T& rhs) const {
            Mat result;
            for (u8 i = 0; i < N; i++) {
                for (u8 j = 0; j < N; j++)
                    result[i][j] = _data[i][j] - rhs;
            }
            return result;
        }

        constexpr inline Mat operator*(const T& rhs) const {
            Mat result;
            for (u8 i = 0; i < N; i++) {
                for (u8 j = 0; j < N; j++)
                    result[i][j] = _data[i][j] * rhs;
            }
            return result;
        }

        constexpr inline Mat operator/(const T& rhs) const {
            Mat result;
            for (u8 i = 0; i < N; i++) {
                for (u8 j = 0; j < N; j++)
                    result[i][j] = _data[i][j] / rhs;
            }
            return result;
        }

        constexpr inline T* operator[](u8 index) {
            assert(N >= index);
            return _data[index];
        }

        constexpr inline const T* operator[](u8 index) const {
            assert(N >= index);
            return _data[index];
        }

        constexpr inline bool operator==(const Mat& rhs) const {
            for (u8 i = 0; i < N; i++) {
                for (u8 j = 0; j < N; j++)
                    if (_data[i][j] != rhs._data[i][j]) return false;
            }
            return true;
        }

        constexpr inline bool operator!=(const Mat& rhs) const {
            return !(*this == rhs);
        }

    private:

        T _data[N][N];

    };

    typedef Mat<4, f32> Mat4f;
    typedef Mat<4, i32> Mat4i;

    template <u8 N, typename T>
    constexpr inline Mat<N, T> identity() {
        Mat<N, T> result;
        for (u8 i = 0; i < N; i++) {
            for (u8 j = 0; j < N; j++) {
                if (i == j)
                    result[i][j] = T(1);
                else
                    result[i][j] = T(0);
            }
        }
        return result;
    }

    template <u8 N, typename T>
    constexpr inline Mat<N, T> scale(const Vec<N - 1, T>& scale) {
        Mat<N, T> result;
        for (u8 i = 0; i < N; i++) {
            for (u8 j = 0; j < N; j++) {
                if (i == j && i != N - 1)
                    result[i][j] = scale[i];
                else
                    result[i][j] = T(0);
            }
        }
        result[N - 1][N - 1] = T(1);
        return result;
    }

    template <u8 N, typename T>
    constexpr inline Mat<N, T> translation(const Vec<N - 1, T>& pos) {
        Mat<N, T> result;
        for (u8 i = 0; i < N; i++) {
            for (u8 j = 0; j < N; j++) {
                if (i == N - 1 && j != N - 1)
                    result[i][j] = pos[j];
                else if (i == j)
                    result[i][j] = T(1);
                else
                    result[i][j] = T(0);
            }
        }
        result[N - 1][N - 1] = T(1);
        return result;
    }

    template <typename T>
    constexpr inline Mat<4, T> fromVec(const Vec<3, T>& n, const Vec<3, T>& v, const Vec<3, T>& u) {
        Mat<4, T> result;
        result[0][0] = u[0]; result[1][0] = u[1]; result[2][0] = u[2]; result[3][0] = T(0);
        result[0][1] = v[0]; result[1][1] = v[1]; result[2][1] = v[2]; result[3][1] = T(0);
        result[0][2] = n[0]; result[1][2] = n[1]; result[2][2] = n[2]; result[3][2] = T(0);
        result[0][3] = T(0); result[1][3] = T(0); result[2][3] = T(0); result[3][3] = T(1);
        return result;
    }

    template <typename T>
    constexpr inline Mat<4, T> perspective(T fov, T aspect, T near, T far, bool vulkan = true) {
        const f32 range = near - far;
        const f32 tanHalfFov = std::tan(fov / 2.f);

        Mat<4, T> result;
        result[0][0] = 1.f / (tanHalfFov * aspect);
        result[0][1] = 0.f;
        result[0][2] = 0.f;
        result[0][3] = 0.f;

        result[1][0] = 0.f;
        result[1][1] = 1.f / tanHalfFov;
        result[1][2] = 0.f;
        result[1][3] = 0.f;

        result[2][0] = 0.f;
        result[2][1] = 0.f;
        result[2][2] = (-near - far) / range;
        result[2][3] = T(1);

        result[3][0] = 0.f;
        result[3][1] = 0.f;
        result[3][2] = T(2) * far * near / range;
        result[3][3] = 0.f;

        if (vulkan)
            result[1][1] *= -1;

        return result;
    }

    template <typename T>
    constexpr inline Mat<4, T> orthographic(T left, T right, T bottom, T top, T near, T far) {
        const T width = right - left;
        const T height = top - bottom;
        const T depth = far - near;

        Mat<4, T> result;
        result[0][0] = T(2) / width;
        result[0][1] = T(0);
        result[0][2] = T(0);
        result[0][3] = T(0);

        result[1][0] = T(0);
        result[1][1] = T(2) / height;
        result[1][2] = T(0);
        result[1][3] = T(0);

        result[2][0] = T(0);
        result[2][1] = T(0);
//        result[2][2] = -T(1);
        result[2][2] = T(1) / (far - near);
        result[2][3] = T(0);

        result[3][0] = -(right + left) / width;
        result[3][1] = -(top + bottom) / height;
//        result[3][2] = T(0);
        result[3][2] = near / (near - far);
        result[3][3] = T(1);

        return result;
    }

    template <typename T>
    inline Mat<4, T> lookAt(const Vec3f& pos, const Vec3f& target, const Vec3f& origUp) {

        Vec3f front = (target - pos).unit();
        Vec3f right = origUp.cross(front).unit();
        Vec3f up = front.cross(right);

        Mat<4, T> result;
        result[0][0] = right.x();
        result[0][1] = right.y();
        result[0][2] = right.z();
        result[0][3] = T(0);

        result[1][0] = up.x();
        result[1][1] = up.y();
        result[1][2] = up.z();
        result[1][3] = T(0);

        result[2][0] = front.x();
        result[2][1] = front.y();
        result[2][2] = front.z();
        result[2][3] = T(0);

        result[3][0] = pos.x();
        result[3][1] = pos.y();
        result[3][2] = pos.z();
        result[3][3] = T(1);

        return result;
    }

}

#endif //ANINO_MAT_H
