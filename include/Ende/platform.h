#ifndef ENDE_PLATFORM_H
#define ENDE_PLATFORM_H

// define int aliases

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <expected>
#include <optional>
#include <type_traits>
#include <utility>

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


template <typename T>
struct is_expected : std::false_type {};

template <typename E>
struct is_expected<std::unexpected<E>> : std::true_type {};

template <typename T, typename E>
struct is_expected<std::expected<T, E>> : std::true_type {};

template <typename T>
auto _get_error(T&& container) -> decltype(auto) {
    if constexpr (requires { container.error(); })
        return std::forward<T>(container).error();
    else
        return nullptr;
}

template <typename F, typename T>
auto _get_result(F &&fallback, T &&error) -> decltype(auto) {
    if constexpr (std::invocable<F, T>) {
        return std::forward<F>(fallback)(std::forward<T>(error));
    } else {
        return std::forward<F>(fallback);
    }
}

template <typename Storage>
struct _maybe_failure_proxy {
    private:
        using Self = _maybe_failure_proxy;

    public:
        Storage value;

    template <typename T>
    operator std::optional<T>([[maybe_unused]] this const Self& _) {
        return std::nullopt;
    }

    template <typename T, typename E>
    operator std::expected<T, E>(this Self&& self) {
        using value_type = std::decay_t<Storage>;

        if constexpr (std::is_same_v<value_type, std::nullptr_t>) {
            static_assert(std::default_initializable<E>, "E must be default initializable");
            return std::unexpected(E{});
        } else if constexpr (is_expected<value_type>::value)
            return std::forward<value_type>(self.value);
        else
            return std::unexpected<E>(std::forward<value_type>(self.value));
    }

    operator Storage(this Self&& self) {
        return self.value;
    }
};

template <typename T>
_maybe_failure_proxy(T&&) -> _maybe_failure_proxy<std::decay_t<T>>;

template <typename T>
auto _deref_or_void(T&& container) -> decltype(auto) {
    using value_type = typename std::decay_t<T>::value_type;
    if constexpr (std::is_void_v<value_type>)
        return;
    else
        return *std::forward<T>(container);
}

#define GET_maybe_MACRO(_1, _2, NAME, ...) NAME
#define maybe(...) GET_maybe_MACRO(__VA_ARGS__, maybe_2, maybe_1)(__VA_ARGS__)
#define maybe_conv(type, expr) maybe(expr, [] (const auto& e) { return static_cast<type>(e); })

#define maybe_1(expr) \
    ({ \
        auto&& _result = (expr); \
        if (!_result) { \
            return ::_maybe_failure_proxy(::_get_error(_result)); \
        } \
        ::_deref_or_void(std::move(_result)); \
    })


#define maybe_2(expr, fallback) \
    ({ \
        auto&& _result = (expr); \
        if (!_result) { \
            [[maybe_unused]] auto&& _e = ::_get_error(_result); \
            return ::_maybe_failure_proxy(::_get_result(fallback, _e)); \
        } \
        ::_deref_or_void(std::move(_result)); \
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
