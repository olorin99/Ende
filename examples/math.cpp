
#include <Ende/platform.h>
#include <random>

#include <Ende/math/Mat.h>

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

    const auto lhs = ende::math::Mat<4, f32, false>(d1);
    const auto rhs = ende::math::Mat<4, f32, false>(d2);

    const auto vec = ende::math::Vec<4, f32>{ 0.5, 100, 12, 4 };

    // const auto mult = lhs * rhs;
    const auto mult = rhs * lhs;

    printf("\nMultiplication\n");
    compareWithNumpy(16, &mult[0][0], numpyResultMult.data());

    printf("\nInverse\n");
    const auto inverse = lhs.inverse();
    // printMatrix(N, inverse[0]);
    compareWithNumpy(16, &inverse[0][0], numpyResultInv.data());

    printf("\nTranspose\n");
    const auto transpose = lhs.transpose();
    printMatrix(N, transpose[0]);
    compareWithNumpy(16, &transpose[0][0], numpyResultTranspose.data());

    printf("\nTransform\n");
    const auto transform = lhs.transform(vec);
    compareWithNumpy(4, &transform[0], numpyResultTransformVec.data());


    return 0;
}
