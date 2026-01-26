#ifndef ENDE_PLATFORM_H
#define ENDE_PLATFORM_H

// define int aliases

#include <cstdint>

using i8 = std::int8_t;
using u8 = std::uint8_t;
using i16 = std::int16_t;
using u16 = std::uint16_t;
using i32 = std::int32_t;
using u32 = std::uint32_t;
using i64 = std::int64_t;
using u64 = std::uint64_t;
using f32 = float;
using f64 = double;


#define TRY(expr)\
({\
auto&& tmp = (expr);\
if(!tmp.has_value())\
return std::unexpected(tmp.error());\
std::move(tmp.value());\
})

#define TRY_MAIN(expr)\
({\
auto&& tmp = (expr);\
if(!tmp.has_value())\
return static_cast<i32>(tmp.error());\
std::move(tmp.value());\
})

#ifdef __linux__

#define ENDE_LINUX
#define ENDE_UNIX

#define PRETTY_FUNC __PRETTY_FUNCTION__
#define FORCEINLINE __attribute__((always_inline))
#define NOINLINE __attribute__((noinline))

#elif defined __APPLE__

#define ENDE_APPLE
#define ENDE_UNIX

#define PRETTY_FUNC __PRETTY_FUNCTION__
#define FORCEINLINE __attribute__((always_inline))
#define NOINLINE __attribute__((noinline))

#elif defined _WIN32

#define ENDE_WIN

#define PRETTY_FUNC __FUNCTION__
#define FORCEINLINE __forceinline
#define NOINLINE __declspec(noinline)

#else

#error "unsupported system"

#endif



#ifdef __clang__
#define ENDE_CLANG
#elif (defined __GNUC__ || defined __GNUG__) && !(defined __clang__ || defined __INTEL_COMPILER)
#define ENDE_GNU
#elif defined __ICC
#define ENDE_INTEL
#elif defined __MSC_VER
#define ENDE_MSCV
#endif



#ifdef __SSE3__
#define ENDE_SSE3
#elif defined __SSE2__
#define ENDE_SSE2
#elif defined __SSE__
#define ENDE_SSE
#elif defined __AVX2__
#define ENDE_AVX2
#elif defined __AVX__
#define ENDE_AVX
#else
#define ENDE_SCALAR
#endif


// compile time funcs to do checks

namespace ende::platform {

    enum class Os {
        Linux,
        Windows
    };

    constexpr Os os() {
#ifdef ENDE_LINUX
        return Os::Linux;
#elif defined ENDE_WIN
        return Os::Windows;
#endif
    }

    constexpr bool debug() {
#ifdef NDEBUG
        return false;
#else
        return true;
#endif
    }

    constexpr u32 version() {
#ifndef __cplusplus
        return 0;
#elif __cplusplus == 1
        return 1;
#elif __cplusplus == 199711L
        return 98;
#elif __cplusplus == 201103L
        return 11;
#elif __cplusplus == 201402L
        return 14;
#elif __cplusplus == 201703L
        return 17;
#elif __cplusplus == 202002L
#endif
        return 0;
    }

    constexpr const char* compiler() {
#ifdef __clang__
        return "clang";
#elif (defined __GNUC__ || defined __GNUG__) && !(defined __clang__ || defined __INTEL_COMPILER)
        return "gnu";
#elif defined __ICC
        return "intel";
#elif defined __MSC_VER
        return "mscv";
#endif
    }

    constexpr const char* arch() {
#if (defined __amd64__ || defined _M_AMD64)
        return "amd64";
#elif (defined __arm__ || defined _M_ARM)
        return "arm";
#elif (defined __aarch64__)
        return "arm64";
#elif (defined __i386 || defined _M_IX86)
        return "x86";
#endif
    }



    enum class Endian {
        Big,
        Little
    };

    constexpr Endian endian() {
#if (defined __BYTE_ORDER && __BYTE_ORDER == __BIG_ENDIAN) || \
        (defined __BYTE_ORDER__ && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__) || \
        defined __BIG_ENDIAN__ || \
        defined __ARMEB__ || \
        defined __THUMBEB || \
        defined __AARCH64EB__
        return Endian::Big;
#elif (defined __BYTE_ORDER && __BYTE_ORDER == __LITTLE_ENDIAN) || \
        (defined __BYTE_ORDER__ && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__) || \
        defined __LITTLE_ENDIAN__ || \
        defined __ARMEL__ || \
        defined __THUMBEL__ || \
        defined __AARCH64EL
        return Endian::Little;
#endif
    }



    enum class Simd {
        NONE,
        SSE,
        SSE2,
        SSE3,
        SSE4,

        AVX,
        AVX2
    };


    constexpr Simd simd() {
#ifdef __SSE3__
        return Simd::SSE3;
#elif defined __SSE2__
        return Simd::SSE2;
#elif defined __SSE__
        return Simd::SSE;
#elif defined __AVX2__
        return Simd::AVX2;
#elif defined __AVX__
        return Simd::AVX;
#else
        return Simd::None;
#endif
    }

}

// function to compile time comparision for platform details
template <typename T>
constexpr inline bool cmp(T lhs, T rhs) {
    return lhs == rhs;
}


// for string
template <>
constexpr inline bool cmp(const char* lhs, const char* rhs) {
    return *lhs == *rhs && (*lhs == '\0' || *rhs == '\0' || cmp(lhs + 1, rhs + 1));
}


#endif //ENDE_PLATFORM_H
