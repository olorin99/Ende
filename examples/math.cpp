
#include "Ende/math/math.h"
#include <Ende/platform.h>
#include <random>

#include <Ende/math/Mat.h>
#include <Ende/math/Quaternion.h>

void genMatrix(const u32 N, f32* data) {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<f32> dis(0, 1000);


    for (int x = 0; x < N; x++) {
        for (int y = 0; y < N; y++) {
            data[x * N + y] = dis(rng);
        }
    }
}

void printMatrix(const u32 N, const f32* data) {
    for (int x = 0; x < N; x++) {
        for (int y = 0; y < N; y++) {
            printf("%f, ", data[x * N + y]);
        }
        printf("\n");
    }
}

void multiplyMatrix(const u32 N, const f32* lhs, const f32* rhs, f32* output) {
    for (int x = 0; x < N; x++) {
        for (int y = 0; y < N; y++) {
            f32 tmp = 0.0;
            for (int i = 0; i < N; i++) {
                tmp += lhs[y * N + i] * rhs[x + N * i];
            }
            output[y * N + x] = tmp;
        }
    }
}

constexpr std::array<f32, 16> numpyResultMult = {
    441782.6748529, 224734.07919367, 1235459.35434445, 884863.15678932,
    306692.00964685, 329900.91575359, 691674.09555236, 543506.13725086,
    259004.32970611, 403227.55789998, 626837.7746413, 474550.71571464,
    518006.27036842, 390429.56743794, 943193.10424146, 749638.27179617,
};
constexpr std::array<f32, 16> numpyResultInv = {
    0.00043522, 0.00202231, -0.00116297, -0.00048245,
    -0.00039492, 0.00492885, 0.00070352, -0.0027566,
    0.00098571, -0.00090281, 0.00085782, -0.00060891,
    -0.00064117, -0.00660133, 0.00186079, 0.004704
};
constexpr std::array<f32, 16> numpyResultTranspose = {
    953.786682, 544.787109, 87.711945, 859.831055,
    50.874264, 283.525879, 411.690125, 241.963593,
    760.835754, 19.079586, 315.564392, 5.649533,
    226.123016, 224.493805, 291.099396, 443.296082
};
constexpr std::array<f32, 4> numpyResultTransformVec = {
    15598.840853, 29751.9117065, 46164.0387605, 26467.2535515
};

void compareWithNumpy(const u32 N, const f32* result, const f32* comparison) {

    f32 totalDiff = 0;
    for (u32 i = 0; i < N; i++) {
        const auto diff = std::abs(result[i] - comparison[i]);
        if (diff > 1e-6) {
            printf("Error at (%d) of (%f)\n", i, diff);
        }
        totalDiff += diff;
    }
    printf("Total diff between numpy result is (%f)\nAveraged across %i values is (%f)\n", totalDiff, N, totalDiff / N);
}

int main() {

    constexpr i32 N = 4;

    f32 lhsData[N * N] = {};
    genMatrix(N, lhsData);
    printMatrix(N, lhsData);
    f32 rhsData[N * N] = {};
    genMatrix(N, rhsData);
    printMatrix(N, rhsData);

    f32 outputData[N * N] = {};
    multiplyMatrix(N, lhsData, rhsData, outputData);
    printMatrix(N, outputData);


    constexpr std::array<f32, 16> d1 = {
        953.786682, 50.874264, 760.835754, 226.123016,
        544.787109, 283.525879, 19.079586, 224.493805,
        87.711945, 411.690125, 315.564392, 291.099396,
        859.831055, 241.963593, 5.649533, 443.296082,
    };

    constexpr std::array<f32, 16> d2 = {
        261.370972, 107.665634, 752.436890, 570.694946,
        91.448067, 744.699890, 762.233582, 596.810791,
        65.345505, 31.845793, 556.752319, 332.154968,
        610.822266, 265.026337, 245.087509, 254.128189,
    };


    multiplyMatrix(N, d1.data(), d2.data(), outputData);
    printMatrix(N, outputData);

    printf("Raw mult\n");
    compareWithNumpy(16, outputData, numpyResultMult.data());

    const auto lhs = ende::math::Matrix<f32, 4>(d1);
    const auto rhs = ende::math::Matrix<f32, 4>(d2);

    const auto vec = ende::math::float4{ 0.5, 100, 12, 4 };
    const auto vec1 = ende::math::float4{ 0.7, 90, 42, -1 };

    const auto minVec = ende::math::min(vec, vec1);
    const auto maxVec = ende::math::max(vec, vec1);


    const auto a = 2.0 - vec;
    const auto b = 2.0 * vec;
    const auto c = 2.0 + vec;
    const auto d = 2.0 / vec;

    const auto e = ende::math::float4(0.f, 1.f, 2.f, 3.f);

    const auto f = ende::math::identity<f32, 4>();

    const auto h = f * vec;

    const auto mult = lhs * rhs;
    // const auto mult = rhs * lhs;

    printf("\nMultiplication\n");
    compareWithNumpy(16, &mult[0][0], numpyResultMult.data());

    printf("\nInverse\n");
    const auto inverse = lhs.inverse();
    // printMatrix(N, inverse[0]);
    compareWithNumpy(16, &inverse[0][0], numpyResultInv.data());

    printf("\nTranspose\n");
    const auto transpose = lhs.transpose();
    printMatrix(N, &transpose[0][0]);
    compareWithNumpy(16, &transpose[0][0], numpyResultTranspose.data());

    printf("\nTransform\n");
    const auto transform = lhs * vec;
    compareWithNumpy(4, &transform[0][0], numpyResultTransformVec.data());


    auto q1 = ende::math::Quaternion(0.14, 0.56, 0.692, 0.4);
    auto q2 = ende::math::Quaternion(0.24, 0.166, 0.156, 1);

    auto productA = q1 * q2;
    auto productB = q2 * q1;

    printf("q1 * q2: %fi + %fj + %fk + %f\n", productA.x(), productA.y(), productA.z(), productA.w());
    printf("q2 * q1: %fi + %fj + %fk + %f\n", productB.x(), productB.y(), productB.z(), productB.w());


    auto v0 = ende::math::float4x4(std::to_array({
        std::to_array({1.f, 0.f, 0.f, 0.f}),
        std::to_array({1.f, 0.f, 0.f, 0.f}),
        std::to_array({1.f, 0.f, 0.f, 0.f}),
        std::to_array({1.f, 0.f, 0.f, 0.f})
    }));
    // auto v1 = ende::math::v2::identity<f32, 4, 4>();
    auto v1 = ende::math::float4x4({
        2.f, 4.f, 5.f, 5.f,
        66.f, 2.f, 2.f, 4.f,
        4.f, 12.f, 23.f, 56.f,
        6.f, 9.f, 1.f, 4.f
    });
    auto v2 = ende::math::float4(1.f, 0.f, 54.f, 1.f);

    auto v3 = ende::math::scale<f32, 4>({1.f, 2.f, 3.f, 1.f});
    auto v4 = ende::math::translation<f32, 4, 3>({4.f, 56.f, -2.f});

    auto v5 = ende::math::perspective<f32>(ende::math::rad(45.f), 1920.f / 1080.f, 0.f, 1000.f);
    auto v6 = ende::math::orthographic<f32>(-1, 1, -1, 1, 0, 1);

    auto v7 = ende::math::Matrix<f32, 4, 2>({
        1.f, 303.f,
        5.f, -2.f,
        0.54f, 4000.f,
        10.f, 3.f
    });

    auto v8 = ende::math::float4(3.f, 100.f, 34.f, 1.f);

    auto v9 = ende::math::float3(1.f, 0.f, 0.f);
    auto v10 = ende::math::float3(0.f, 1.f, 0.f);

    auto v11 = ende::math::float4(v9, 4.f);
    auto v12 = ende::math::float4(40.f, v10);

    auto v13 = ende::math::float4({10.f, 20.f}, {30.f, 50.f});

    auto m2 = v0 * v1;
    auto m3 = v1 * v2;

    auto m4 = v1 * 30;
    auto m5 = v1 / 4;
    auto m6 = v1 + 90;
    auto m7 = v1 - 34.f;

    auto d0 = v1.determinant();
    auto i0 = v1.inverse();

    // auto d3 = v7.determinant();

    auto l0 = v2.dot(v8);
    auto l1 = v2.lengthSquared();
    auto l2 = v2.length();
    auto l3 = v2.unit();
    auto l4 = v2.reflect(v8);
    auto l5 = v2.project(v8);
    auto l6 = v2.lerp(v8, 0.7);

    auto l7 = v9.cross(v10);


    auto p0 = v2.x();
    auto p1 = v2.xy();
    auto p2 = v2.xyz();
    auto p3 = v2.ywxz();

    auto r0 = v0[0];
    auto r1 = v0[1];

    auto c0 = v0.col(0);
    auto c1 = v0.col(1);

    auto f03 = r0[3];
    auto f33 = v0[3][3];

    printf("%f", f03);

    return 0;
}
