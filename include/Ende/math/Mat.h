#ifndef ENDE_MAT_H
#define ENDE_MAT_H

#include <Ende/platform.h>
#include <cmath>
#include <cstddef>
#include <initializer_list>
#include <span>

namespace ende::math {

template <u32 M, u32 N>
concept IsSquare = M == N;

template <u32 M, u32 N>
concept IsVector = M == 1 || N == 1;

template <u32 M, u32 N>
concept IsColumnVector = N == 1;

template <u32 M, u32 N>
concept IsRowVector = M == 1;

template <typename T, u32 M, u32 N = M>
class Matrix {
  public:
    constexpr static u32 SIZE = M * N;

    constexpr inline Matrix() = default;

    explicit constexpr inline Matrix(const T &rhs) {
        for (u32 m = 0; m < M; m++) {
            for (u32 n = 0; n < N; n++) {
                (*this)[m][n] = rhs;
            }
        }
    }

    explicit constexpr inline Matrix(const std::array<std::array<T, N>, M> data) {
        for (u32 m = 0; m < M; m++) {
            for (u32 n = 0; n < N; n++) {
                (*this)[m][n] = data[m][n];
            }
        }
    }

    explicit constexpr inline Matrix(std::initializer_list<std::initializer_list<T>> data) {
        for (u32 mi = 0; auto &m : data) {
            for (u32 ni = 0; auto &c : m) {
                (*this)[mi][ni] = c;

                ni++;
            }
            mi++;
        }
    }

    explicit constexpr inline Matrix(const std::array<T, SIZE> data) : _data(data) {}

    constexpr inline Matrix(std::initializer_list<T> data, bool identity = true) {
        u32 m = 0;
        u32 n = 0;
        for (auto &l : data) {
            (*this)[m][n] = l;

            n++;
            if (n >= N) {
                m++;
                n = 0;
            }
        }
        if (identity) {
            for (; m < M; m++) {
                for (; n < N; n++) {
                    if (m == n)
                        (*this)[m][n] = T(1);
                    else
                        (*this)[m][n] = T(0);
                }
            }
        }
    }

    constexpr inline auto operator==(const Matrix &rhs) const -> bool {
        for (u32 m = 0; m < M; m++) {
            for (u32 n = 0; n < N; n++) {
                if ((*this)[m][n] != rhs[m][n])
                    return false;
            }
        }
        return true;
    }

    template <typename U>
    class Column {
      public:
        constexpr inline auto operator[](const u32 index) -> U & {
            return _data[index * M + _columnIndex];
        }

        constexpr inline auto operator[](const u32 index) const -> const U & {
            return _data[index * M + _columnIndex];
        }

      private:
        friend Matrix;

        Column(const u32 col, std::span<U, SIZE> data) : _columnIndex(col), _data(data) {}

        u32 _columnIndex = 0;
        std::span<U, SIZE> _data = {};
    };

    template <typename U>
    class Row {
      public:
        constexpr inline Row(const Row &rhs) {
            _rowIndex = rhs._rowIndex;
            _data = rhs._data;
            for (u32 i = 0; i < N; i++) {
                (*this)[i] = rhs[i];
            }
        }

        constexpr inline Row(Row &&rhs) noexcept : _rowIndex(rhs._rowIndex), _data(rhs._data) {
            for (u32 i = 0; i < N; i++) {
                std::swap((*this)[i], rhs[i]);
            }
        }

        constexpr auto operator=(const Row &rhs) -> Row & {
            _rowIndex = rhs._rowIndex;
            _data = rhs._data;
            for (u32 i = 0; i < N; i++) {
                (*this)[i] = rhs[i];
            }
            return *this;
        }

        constexpr auto operator=(Row &&rhs) noexcept -> Row & {
            std::swap(_rowIndex, rhs._rowIndex);
            std::swap(_data, rhs._data);
            for (u32 i = 0; i < N; i++) {
                std::swap((*this)[i], rhs[i]);
            }
            return *this;
        }

        constexpr auto operator=(const U &rhs) -> Row & {
            for (u32 i = 0; i < N; i++) {
                (*this)[i] = rhs;
            }
            return *this;
        }

        constexpr inline operator T()
            requires IsVector<M, N>
        {
            return (*this)[0];
        }

        constexpr inline auto operator[](const u32 index) -> U & {
            return _data[_rowIndex * N + index];
        }

        constexpr inline auto operator[](const u32 index) const -> const U & {
            return _data[_rowIndex * N + index];
        }

        template <typename V>
        constexpr inline auto operator*=(const V *rhs) -> Row & {
            for (u32 i = 0; i < N; i++) {
                (*this)[i] *= rhs;
            }
            return (*this);
        }

        template <typename V>
        constexpr inline auto operator/=(const V *rhs) -> Row & {
            for (u32 i = 0; i < N; i++) {
                (*this)[i] /= rhs;
            }
            return (*this);
        }

        template <typename V>
        constexpr inline auto operator+=(const V *rhs) -> Row & {
            for (u32 i = 0; i < N; i++) {
                (*this)[i] += rhs;
            }
            return (*this);
        }

        template <typename V>
        constexpr inline auto operator-=(const V *rhs) -> Row & {
            for (u32 i = 0; i < N; i++) {
                (*this)[i] -= rhs;
            }
            return (*this);
        }

      private:
        friend Matrix;

        Row(const u32 row, std::span<U, SIZE> data) : _rowIndex(row), _data(data) {}

        u32 _rowIndex = 0;
        std::span<U, SIZE> _data = nullptr;
    };

    constexpr inline auto operator[](const u32 index) -> Row<T> {
        return Row(index, std::span<T, SIZE>(_data));
    }

    constexpr inline auto operator[](const u32 index) const -> Row<const T> {
        return Row(index, std::span<const T, SIZE>(_data));
    }

    constexpr inline auto row(const u32 index) -> Row<T> {
        return Row(index, std::span<T, SIZE>(_data));
    }

    constexpr inline auto row(const u32 index) const -> Row<const T> {
        return Row(index, std::span<const T, SIZE>(_data));
    }

    constexpr inline auto col(const u32 index) -> Column<T> {
        return Column(index, std::span<T, SIZE>(_data));
    }

    constexpr inline auto col(const u32 index) const -> Column<const T> {
        return Column(index, std::span<const T, SIZE>(_data));
    }

    // Matrix operations

    template <typename U = T, u32 P = N>
    constexpr inline auto operator*(const Matrix<U, N, P> &rhs) const -> Matrix<T, M, P> {
        Matrix<T, M, P> result;
        for (u32 m = 0; m < M; m++) {
            for (u32 p = 0; p < P; p++) {
                T tmp = T(0);
                for (u32 n = 0; n < N; n++) {
                    tmp += (*this)[m][n] * rhs[n][p];
                }
                result[m][p] = tmp;
            }
        }
        return result;
    }

    constexpr inline auto operator+(const Matrix &rhs) const -> Matrix {
        Matrix result;
        for (u32 m = 0; m < M; m++) {
            for (u32 n = 0; n < N; n++) {
                result[m][n] = (*this)[m][n] + rhs[m][n];
            }
        }
        return result;
    }

    constexpr inline auto operator-(const Matrix &rhs) const -> Matrix {
        Matrix result;
        for (u32 m = 0; m < M; m++) {
            for (u32 n = 0; n < N; n++) {
                result[m][n] = (*this)[m][n] - rhs[m][n];
            }
        }
        return result;
    }

    // Scalar operations

    template <typename U>
    constexpr inline auto operator*(const U &rhs) const -> Matrix {
        Matrix result;
        for (u32 m = 0; m < M; m++) {
            for (u32 n = 0; n < N; n++) {
                result[m][n] = (*this)[m][n] * rhs;
            }
        }
        return result;
    }

    template <typename U>
    constexpr inline auto operator/(const U &rhs) const -> Matrix {
        Matrix result;
        for (u32 m = 0; m < M; m++) {
            for (u32 n = 0; n < N; n++) {
                result[m][n] = (*this)[m][n] / rhs;
            }
        }
        return result;
    }

    template <typename U>
    constexpr inline auto operator+(const U &rhs) const -> Matrix {
        Matrix result;
        for (u32 m = 0; m < M; m++) {
            for (u32 n = 0; n < N; n++) {
                result[m][n] = (*this)[m][n] + rhs;
            }
        }
        return result;
    }

    template <typename U>
    constexpr inline auto operator-(const U &rhs) const -> Matrix {
        Matrix result;
        for (u32 m = 0; m < M; m++) {
            for (u32 n = 0; n < N; n++) {
                result[m][n] = (*this)[m][n] - rhs;
            }
        }
        return result;
    }

    constexpr inline auto transpose() const -> Matrix<T, N, M> {
        Matrix<T, N, M> result;
        for (u32 m = 0; m < M; m++) {
            for (u32 n = 0; n < N; n++) {
                result[m][n] = (*this)[n][m];
            }
        }
        return result;
    }

    constexpr inline auto determinant() const -> T
        requires(M == N && M == 2)
    {
        const auto a = (*this)[0][0];
        const auto b = (*this)[0][1];
        const auto c = (*this)[1][0];
        const auto d = (*this)[1][1];
        return (a * d) - (b * c);
    }

    constexpr inline auto determinant() const -> T
        requires(M == N && M == 3)
    {
        const auto a = (*this)[0][0];
        const auto b = (*this)[0][1];
        const auto c = (*this)[0][2];

        const auto d = (*this)[1][0];
        const auto e = (*this)[1][1];
        const auto f = (*this)[1][2];

        const auto g = (*this)[2][0];
        const auto h = (*this)[2][1];
        const auto i = (*this)[2][2];
        return (a * e * i) + (b * f * g) + (c * d * h) - (c * e * g) - (b * d * i) - (a * f * h);
    }

    constexpr inline auto determinant() const -> T
        requires IsSquare<M, N>
    {
        auto d = (*this);
        T det = 1.0;
        for (u32 n = 0; n < N; n++) {
            i32 pivot = n;
            for (u32 m = n + 1; m < M; m++) {
                if (std::abs(d[m][n]) > std::abs(d[pivot][n]))
                    pivot = m;
            }
            if (pivot != n) {
                auto r0 = d[n];
                auto r1 = d[pivot];
                std::swap(r0, r1);
                d[n] = r0;
                d[pivot] = r1;
                det *= -1;
            }
            if (d[n][n] == 0)
                return 0;
            det *= d[n][n];
            for (u32 m = n + 1; m < M; m++) {
                f32 factor = d[m][n] / d[n][n];
                for (u32 k = n + 1; k < M; k++) {
                    d[m][k] -= factor * d[n][k];
                }
            }
        }
        return det;
    }

    constexpr inline auto inverse() const -> Matrix
        requires(M == N && M == 4)
    {
        Matrix inverse;
        inverse._data[0] = _data[5] * _data[10] * _data[15] - _data[5] * _data[14] * _data[11] - _data[6] * _data[9] * _data[15] + _data[6] * _data[13] * _data[11] + _data[7] * _data[9] * _data[14] - _data[7] * _data[13] * _data[10];
        inverse._data[1] = -_data[1] * _data[10] * _data[15] + _data[1] * _data[14] * _data[11] + _data[2] * _data[9] * _data[15] - _data[2] * _data[13] * _data[11] - _data[3] * _data[9] * _data[14] + _data[3] * _data[13] * _data[10];
        inverse._data[2] = _data[1] * _data[6] * _data[15] - _data[1] * _data[14] * _data[7] - _data[2] * _data[5] * _data[15] + _data[2] * _data[13] * _data[7] + _data[3] * _data[5] * _data[14] - _data[3] * _data[13] * _data[6];
        inverse._data[3] = -_data[1] * _data[6] * _data[11] + _data[1] * _data[10] * _data[7] + _data[2] * _data[5] * _data[11] - _data[2] * _data[9] * _data[7] - _data[3] * _data[5] * _data[10] + _data[3] * _data[9] * _data[6];
        inverse._data[4] = -_data[4] * _data[10] * _data[15] + _data[4] * _data[14] * _data[11] + _data[6] * _data[8] * _data[15] - _data[6] * _data[12] * _data[11] - _data[7] * _data[8] * _data[14] + _data[7] * _data[12] * _data[10];
        inverse._data[5] = _data[0] * _data[10] * _data[15] - _data[0] * _data[14] * _data[11] - _data[2] * _data[8] * _data[15] + _data[2] * _data[12] * _data[11] + _data[3] * _data[8] * _data[14] - _data[3] * _data[12] * _data[10];
        inverse._data[6] = -_data[0] * _data[6] * _data[15] + _data[0] * _data[14] * _data[7] + _data[2] * _data[4] * _data[15] - _data[2] * _data[12] * _data[7] - _data[3] * _data[4] * _data[14] + _data[3] * _data[12] * _data[6];
        inverse._data[7] = _data[0] * _data[6] * _data[11] - _data[0] * _data[10] * _data[7] - _data[2] * _data[4] * _data[11] + _data[2] * _data[8] * _data[7] + _data[3] * _data[4] * _data[10] - _data[3] * _data[8] * _data[6];
        inverse._data[8] = _data[4] * _data[9] * _data[15] - _data[4] * _data[13] * _data[11] - _data[5] * _data[8] * _data[15] + _data[5] * _data[12] * _data[11] + _data[7] * _data[8] * _data[13] - _data[7] * _data[12] * _data[9];
        inverse._data[9] = -_data[0] * _data[9] * _data[15] + _data[0] * _data[13] * _data[11] + _data[1] * _data[8] * _data[15] - _data[1] * _data[12] * _data[11] - _data[3] * _data[8] * _data[13] + _data[3] * _data[12] * _data[9];
        inverse._data[10] = _data[0] * _data[5] * _data[15] - _data[0] * _data[13] * _data[7] - _data[1] * _data[4] * _data[15] + _data[1] * _data[12] * _data[7] + _data[3] * _data[4] * _data[13] - _data[3] * _data[12] * _data[5];
        inverse._data[11] = -_data[0] * _data[5] * _data[11] + _data[0] * _data[9] * _data[7] + _data[1] * _data[4] * _data[11] - _data[1] * _data[8] * _data[7] - _data[3] * _data[4] * _data[9] + _data[3] * _data[8] * _data[5];
        inverse._data[12] = -_data[4] * _data[9] * _data[14] + _data[4] * _data[13] * _data[10] + _data[5] * _data[8] * _data[14] - _data[5] * _data[12] * _data[10] - _data[6] * _data[8] * _data[13] + _data[6] * _data[12] * _data[9];
        inverse._data[13] = _data[0] * _data[9] * _data[14] - _data[0] * _data[13] * _data[10] - _data[1] * _data[8] * _data[14] + _data[1] * _data[12] * _data[10] + _data[2] * _data[8] * _data[13] - _data[2] * _data[12] * _data[9];
        inverse._data[14] = -_data[0] * _data[5] * _data[14] + _data[0] * _data[13] * _data[6] + _data[1] * _data[4] * _data[14] - _data[1] * _data[12] * _data[6] - _data[2] * _data[4] * _data[13] + _data[2] * _data[12] * _data[5];
        inverse._data[15] = _data[0] * _data[5] * _data[10] - _data[0] * _data[9] * _data[6] - _data[1] * _data[4] * _data[10] + _data[1] * _data[8] * _data[6] + _data[2] * _data[4] * _data[9] - _data[2] * _data[8] * _data[5];

        T det = _data[0] * inverse._data[0] + _data[4] * inverse._data[1] + _data[8] * inverse._data[2] + _data[12] * inverse._data[3];

        det = 1.f / det;
        return inverse * det;
    }

    // Vector operations

    constexpr inline explicit Matrix(const T &x, const T &y)
        requires IsVector<M, N> && (M == 2 || N == 2)
        : Matrix({x, y}) {}
    constexpr inline explicit Matrix(const T &x, const T &y, const T &z)
        requires IsVector<M, N> && (M == 3 || N == 3)
        : Matrix({x, y, z}) {}
    constexpr inline explicit Matrix(const T &x, const T &y, const T &z, const T &w)
        requires IsVector<M, N> && (M == 4 || N == 4)
        : Matrix({x, y, z, w}) {}

    constexpr inline explicit Matrix(const Matrix<T, 2, 1> &xy, const T &z)
        requires IsVector<M, N> && (M == 3 || N == 3)
        : Matrix(xy[0], xy[1], z) {}
    constexpr inline explicit Matrix(const T &x, const Matrix<T, 2, 1> &yz)
        requires IsVector<M, N> && (M == 3 || N == 3)
        : Matrix(x, yz[0], yz[1]) {}

    constexpr inline explicit Matrix(const Matrix<T, 2, 1> &xy, const T &z, const T &w)
        requires IsVector<M, N> && (M == 4 || N == 4)
        : Matrix(xy[0], xy[1], z, w) {}
    constexpr inline explicit Matrix(const T &x, const Matrix<T, 2, 1> &yz, const T &w)
        requires IsVector<M, N> && (M == 4 || N == 4)
        : Matrix(x, yz[0], yz[1], w) {}
    constexpr inline explicit Matrix(const T &x, const T &y, const Matrix<T, 2, 1> &zw)
        requires IsVector<M, N> && (M == 4 || N == 4)
        : Matrix(x, y, zw[0], zw[1]) {}
    constexpr inline explicit Matrix(const Matrix<T, 2, 1> &xy, const Matrix<T, 2, 1> &zw)
        requires IsVector<M, N> && (M == 4 || N == 4)
        : Matrix(xy[0], xy[1], zw[0], zw[1]) {}
    constexpr inline explicit Matrix(const Matrix<T, 3, 1> &xyz, const T &w)
        requires IsVector<M, N> && (M == 4 || N == 4)
        : Matrix(xyz[0], xyz[1], xyz[2], w) {}
    constexpr inline explicit Matrix(const T &x, const Matrix<T, 3, 1> &yzw)
        requires IsVector<M, N> && (M == 4 || N == 4)
        : Matrix(x, yzw[0], yzw[1], yzw[2]) {}

    constexpr inline auto dot(const Matrix &rhs) const -> T
        requires IsVector<M, N>
    {
        T result = T(0);
        for (u32 m = 0; m < M; m++) {
            result += (*this)[m] * rhs[m];
        }
        return result;
    }

    constexpr inline auto lengthSquared() const -> T
        requires IsVector<M, N>
    {
        return dot(*this);
    }

    constexpr inline auto length() const -> T
        requires IsVector<M, N>
    {
        return std::sqrt(lengthSquared());
    }

    constexpr inline auto unit() const -> Matrix
        requires IsVector<M, N>
    {
        return *this / length();
    }

    constexpr inline auto reflect(const Matrix &rhs) const -> Matrix
        requires IsVector<M, N>
    {
        return *this - (rhs * (dot(rhs) * 2));
    }

    constexpr inline auto project(const Matrix &rhs) const -> Matrix
        requires IsVector<M, N>
    {
        return rhs * (dot(rhs) / rhs.lengthSquared());
    }

    constexpr inline auto lerp(const Matrix &rhs, f32 factor) const -> Matrix
        requires IsVector<M, N>
    {
        return (rhs - *this) * factor + *this;
    }

    constexpr inline auto cross(const Matrix &rhs) const -> Matrix
        requires IsVector<M, N> && (M == 3 || N == 3)
    {
        const auto x = (*this)[1] * rhs[2] - (*this)[2] * rhs[1];
        const auto y = (*this)[2] * rhs[0] - (*this)[0] * rhs[2];
        const auto z = (*this)[0] * rhs[1] - (*this)[1] * rhs[0];
        return Matrix({x, y, z});
    }

    constexpr inline auto x() const -> T
        requires IsVector<M, N>
    {
        return (*this)[0];
    }

    constexpr inline auto y() const -> T
        requires IsVector<M, N> && (M >= 2 || N >= 2)
    {
        return (*this)[1];
    }

    constexpr inline auto z() const -> T
        requires IsVector<M, N> && (M >= 3 || N >= 3)
    {
        return (*this)[2];
    }

    constexpr inline auto w() const -> T
        requires IsVector<M, N> && (M >= 4 || N >= 4)
    {
        return (*this)[3];
    }

#define SWIZZLE2(a, b)                                    \
    constexpr inline auto a##b() const -> Matrix<T, 2, 1> \
        requires IsVector<M, N> && (requires {a(); b(); })         \
    {                                                     \
        return {a(), b()};                                \
    }
#define SWIZZLE3(a, b, c)                                    \
    constexpr inline auto a##b##c() const -> Matrix<T, 3, 1> \
        requires IsVector<M, N> && (requires {a(); b(); c(); })            \
    {                                                        \
        return {a(), b(), c()};                              \
    }
#define SWIZZLE4(a, b, c, d)                                    \
    constexpr inline auto a##b##c##d() const -> Matrix<T, 4, 1> \
        requires IsVector<M, N> && (requires {a(); b(); c(); d(); })               \
    {                                                           \
        return {a(), b(), c(), d()};                            \
    }

    SWIZZLE2(x, y);
    SWIZZLE2(x, z);
    SWIZZLE2(x, w);

    SWIZZLE2(y, x);
    SWIZZLE2(y, z);
    SWIZZLE2(y, w);

    SWIZZLE2(z, y);
    SWIZZLE2(z, x);
    SWIZZLE2(z, w);

    SWIZZLE3(x, y, z);
    SWIZZLE3(x, z, y);
    SWIZZLE3(x, y, w);
    SWIZZLE3(x, z, w);
    SWIZZLE3(x, w, z);
    SWIZZLE3(x, w, y);

    SWIZZLE3(y, x, z);
    SWIZZLE3(y, z, x);
    SWIZZLE3(y, x, w);
    SWIZZLE3(y, z, w);
    SWIZZLE3(y, w, z);
    SWIZZLE3(y, w, x);

    SWIZZLE3(z, y, x);
    SWIZZLE3(z, x, y);
    SWIZZLE3(z, y, w);
    SWIZZLE3(z, x, w);
    SWIZZLE3(z, w, x);
    SWIZZLE3(z, w, y);

    SWIZZLE3(w, y, z);
    SWIZZLE3(w, z, y);
    SWIZZLE3(w, y, x);
    SWIZZLE3(w, z, x);
    SWIZZLE3(w, x, z);
    SWIZZLE3(w, x, y);

    SWIZZLE4(x, y, z, w);
    SWIZZLE4(x, y, w, z);
    SWIZZLE4(x, z, y, w);
    SWIZZLE4(x, z, w, y);
    SWIZZLE4(x, w, z, y);
    SWIZZLE4(x, w, y, z);

    SWIZZLE4(y, x, z, w);
    SWIZZLE4(y, x, w, z);
    SWIZZLE4(y, z, x, w);
    SWIZZLE4(y, z, w, x);
    SWIZZLE4(y, w, z, x);
    SWIZZLE4(y, w, x, z);

    SWIZZLE4(z, y, x, w);
    SWIZZLE4(z, y, w, x);
    SWIZZLE4(z, x, y, w);
    SWIZZLE4(z, x, w, y);
    SWIZZLE4(z, w, x, y);
    SWIZZLE4(z, w, y, x);

    SWIZZLE4(w, y, z, x);
    SWIZZLE4(w, y, x, z);
    SWIZZLE4(w, z, y, x);
    SWIZZLE4(w, z, x, y);
    SWIZZLE4(w, x, z, y);
    SWIZZLE4(w, x, y, z);

    auto data() -> std::span<T, SIZE> { return _data; }
    auto data() const -> std::span<const T, SIZE> { return _data; }

  private:
    std::array<T, SIZE> _data = {};
};

template <typename T, u32 M, u32 N = M>
constexpr inline auto identity() -> Matrix<T, M, N> {
    Matrix<T, M, N> result;
    for (u32 m = 0; m < M; m++) {
        for (u32 n = 0; n < N; n++) {
            if (m == n)
                result[m][n] = T(1);
            else
                result[m][n] = T(0);
        }
    }
    return result;
}

template <typename T, u32 M, u32 N = M>
constexpr inline auto scale(const Matrix<T, N, 1> &rhs) -> Matrix<T, M, M> {
    Matrix<T, M, M> result = identity<T, M>();
    for (u32 m = 0; m < M; m++) {
        for (u32 n = 0; n < M; n++) {
            if (m < N && m == n)
                result[m][n] = rhs[m];
        }
    }
    return result;
}

template <typename T, u32 M, u32 N = M>
constexpr inline auto translation(const Matrix<T, N, 1> &rhs) -> Matrix<T, M, M> {
    Matrix<T, M, M> result = identity<T, M>();
    for (u32 m = 0; m < M; m++) {
        for (u32 n = 0; n < M; n++) {
            if (m < N && n == M - 1)
                result[m][n] = rhs[m];
        }
    }
    return result;
}

template <typename T>
constexpr inline auto perspective(T fov, T aspect, T near, T far) -> Matrix<T, 4, 4> {
    const f32 tanHalfFov = std::tan(fov / 2.f);

    Matrix<T, 4, 4> result;

    result[0][0] = 1.f / (tanHalfFov * aspect);
    result[1][1] = 1.f / tanHalfFov;
    result[2][2] = far / (far - near);
    result[2][3] = (-near * far) / (far - near);
    result[3][2] = T(1);

    return result;
}

template <typename T>
constexpr inline auto orthographic(T left, T right, T bottom, T top, T near, T far) -> Matrix<T, 4, 4> {
    const auto width = right - left;
    const auto height = top - bottom;
    const auto depth = far - near;

    Matrix<T, 4, 4> result;

    result[0][0] = T(2) / width;
    result[1][1] = -T(2) / height;
    result[2][2] = T(1) / depth;
    result[0][3] = -(right + left) / width;
    result[1][3] = -(top + bottom) / height;
    result[2][3] = near / (near - far);
    result[3][3] = T(1);

    return result;
}

template <typename T>
constexpr inline auto lookAt(const Matrix<T, 3, 1> &position, const Matrix<T, 3, 1> &target, const Matrix<T, 3, 1> &origUp) -> Matrix<T, 4, 4> {
    const auto front = (target - position).unit();
    const auto right = origUp.cross(front).unit();
    const auto up = front.cross(right);

    Matrix<T, 4, 4> result;
    result[0][0] = right.x();
    result[1][0] = right.y();
    result[2][0] = right.z();

    result[0][1] = up.x();
    result[1][1] = up.y();
    result[2][1] = up.z();

    result[0][2] = front.x();
    result[1][2] = front.y();
    result[2][2] = front.z();

    result[0][3] = position.x();
    result[1][3] = position.y();
    result[2][3] = position.z();
    result[3][3] = T(1);

    return result;
}

template <typename T, typename U, u32 M, u32 N = M>
constexpr inline auto operator*(const U &lhs, const Matrix<T, M, N> &rhs) -> Matrix<T, M, N> {
    return rhs * lhs;
}

template <typename T, typename U, u32 M, u32 N = M>
constexpr inline auto operator/(const U &lhs, const Matrix<T, M, N> &rhs) -> Matrix<T, M, N> {
    Matrix<T, M, N> result = {};
    for (u32 m = 0; m < M; m++) {
        for (u32 n = 0; n < N; n++) {
            result[m][n] = lhs / rhs[m][n];
        }
    }
    return result;
}

template <typename T, typename U, u32 M, u32 N = M>
constexpr inline auto operator+(const U &lhs, const Matrix<T, M, N> &rhs) -> Matrix<T, M, N> {
    return Matrix<T, M, N>(lhs) + rhs;
}

template <typename T, typename U, u32 M, u32 N = M>
constexpr inline auto operator-(const U &lhs, const Matrix<T, M, N> &rhs) -> Matrix<T, M, N> {
    return Matrix<T, M, N>(lhs) - rhs;
}

template <typename T, u32 M, u32 N = M>
constexpr inline auto min(const Matrix<T, M, N> &lhs, const Matrix<T, M, N> &rhs) -> Matrix<T, M, N> {
    Matrix<T, M, N> result = {};
    for (u32 m = 0; m < M; m++) {
        for (u32 n = 0; n < N; n++) {
            result[m][n] = std::min(lhs[m][n], rhs[m][n]);
        }
    }
    return result;
};

template <typename T, u32 M, u32 N = M>
constexpr inline auto max(const Matrix<T, M, N> &lhs, const Matrix<T, M, N> &rhs) -> Matrix<T, M, N> {
    Matrix<T, M, N> result = {};
    for (u32 m = 0; m < M; m++) {
        for (u32 n = 0; n < N; n++) {
            result[m][n] = std::max(lhs[m][n], rhs[m][n]);
        }
    }
    return result;
};

using float2x2 = Matrix<f32, 2, 2>;
using float3x3 = Matrix<f32, 3, 3>;
using float4x4 = Matrix<f32, 4, 4>;

using float2 = Matrix<f32, 2, 1>;
using float3 = Matrix<f32, 3, 1>;
using float4 = Matrix<f32, 4, 1>;

using int2 = Matrix<i32, 2, 1>;
using int3 = Matrix<i32, 3, 1>;
using int4 = Matrix<i32, 4, 1>;

using uint2 = Matrix<u32, 2, 1>;
using uint3 = Matrix<u32, 3, 1>;
using uint4 = Matrix<u32, 4, 1>;

} // namespace ende::math

#endif // ENDE_MAT_H
