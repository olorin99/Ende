module;


#include <cstdint>
#include <expected>
#include <optional>
#include <array>
#define ENDE_DONT_IMPORT_PLATFORM_UTIL
#include <Ende/platform.h>

export module ende.util;

export using i8 = std::int8_t;
export using u8 = std::uint8_t;
export using i16 = std::int16_t;
export using u16 = std::uint16_t;
export using i32 = std::int32_t;
export using u32 = std::uint32_t;
export using i64 = std::int64_t;
export using u64 = std::uint64_t;
export using f32 = float;
export using f64 = double;

export template <typename T>
struct is_expected : std::false_type {};

template <typename E>
struct is_expected<std::unexpected<E>> : std::true_type {};

template <typename T, typename E>
struct is_expected<std::expected<T, E>> : std::true_type {};

export template <typename T>
auto _get_error(T &&container) -> decltype(auto) {
    if constexpr (requires { container.error(); })
        return std::forward<T>(container).error();
    else
        return nullptr;
}

export template <typename F, typename T>
auto _get_result(F &&fallback, T &&error) -> decltype(auto) {
    if constexpr (std::invocable<F, T>) {
        return std::forward<F>(fallback)(std::forward<T>(error));
    } else {
        return std::forward<F>(fallback);
    }
}

export template <typename Storage>
struct _maybe_failure_proxy {
  private:
    using Self = _maybe_failure_proxy;

  public:
    Storage value;

    template <typename T>
    operator std::optional<T>([[maybe_unused]] this const Self &_) {
        return std::nullopt;
    }

    template <typename T, typename E>
    operator std::expected<T, E>(this Self &&self) {
        using value_type = std::decay_t<Storage>;

        if constexpr (std::is_same_v<value_type, std::nullptr_t>) {
            static_assert(std::default_initializable<E>, "E must be default initializable");
            return std::unexpected(E{});
        } else if constexpr (is_expected<value_type>::value)
            return std::forward<value_type>(self.value);
        else
            return std::unexpected<E>(std::forward<value_type>(self.value));
    }

    operator Storage(this Self &&self) {
        return self.value;
    }
};

export template <typename T>
_maybe_failure_proxy(T &&) -> _maybe_failure_proxy<std::decay_t<T>>;

export template <typename T>
auto _deref_or_void(T &&container) -> decltype(auto) {
    using value_type = typename std::decay_t<T>::value_type;
    if constexpr (std::is_void_v<value_type>)
        return;
    else
        return *std::forward<T>(container);
}



namespace ende::util {

    namespace platform {

        export enum class Os {
            Linux,
            Windows
        };

        export constexpr Os os() {
        #ifdef ENDE_LINUX
            return Os::Linux;
        #elif defined ENDE_WIN
            return Os::Windows;
        #endif
        }

        export constexpr bool debug() {
        #ifdef NDEBUG
            return false;
        #else
            return true;
        #endif
        }

        export constexpr u32 version() {
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

        export constexpr const char *compiler() {
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

        export constexpr const char *arch() {
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

        export enum class Endian {
            Big,
            Little
        };

        export constexpr Endian endian() {
        #if (defined __BYTE_ORDER && __BYTE_ORDER == __BIG_ENDIAN) ||             \
            (defined __BYTE_ORDER__ && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__) || \
            defined __BIG_ENDIAN__ ||                                             \
            defined __ARMEB__ ||                                                  \
            defined __THUMBEB ||                                                  \
            defined __AARCH64EB__
            return Endian::Big;
        #elif (defined __BYTE_ORDER && __BYTE_ORDER == __LITTLE_ENDIAN) ||           \
            (defined __BYTE_ORDER__ && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__) || \
            defined __LITTLE_ENDIAN__ ||                                             \
            defined __ARMEL__ ||                                                     \
            defined __THUMBEL__ ||                                                   \
            defined __AARCH64EL
            return Endian::Little;
        #endif
        }

        export enum class Simd {
            NONE,
            SSE,
            SSE2,
            SSE3,
            SSE4,

            AVX,
            AVX2
        };

        export constexpr Simd simd() {
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

        } // namespace ende::platform

    // function to compile time comparision for platform details
    export template <typename T>
    constexpr inline bool cmp(T lhs, T rhs) {
        return lhs == rhs;
    }

    // for string
    template <>
    constexpr inline bool cmp(const char *lhs, const char *rhs) {
        return *lhs == *rhs && (*lhs == '\0' || *rhs == '\0' || cmp(lhs + 1, rhs + 1));
    }


export constexpr auto rgb(f32 r, f32 g, f32 b, f32 a = 256) -> std::array<f32, 4> {
    return {r / 256.f, g / 256.f, b / 256.f, a / 256.f};
}




export constexpr inline auto combineHash(u32 first, u32 second) -> u32 {
    return first ^= second + 0x9e3779b9 + (first << 6) + (first >> 2);
}

export constexpr inline auto combineHash(u64 first, u64 second) -> u64 {
    return first ^= second + 0x9e3779b97f4a7c15 + (first << 12) + (first >> 4);
}

export inline auto murmur3(const u32 *key, u32 count, u32 seed) noexcept -> u32 {
    u32 h = seed;
    u32 i = count;
    do {
        u32 k = *key++;
        k *= 0xcc9e2d51u;
        k = (k << 15u) | (k >> 17u);
        k *= 0x1b873593u;
        h ^= k;
        h = (h << 13u) | (k >> 19u);
        h = (h * 5u) + 0xe6546b64u;
    } while (--i);
    h ^= count;
    h ^= h >> 16u;
    h *= 0x86ebca6bu;
    h ^= h >> 13u;
    h *= 0xc2b2ae35u;
    h ^= h >> 16u;
    return h;
}

export template <typename T>
struct MurmurHash {
    auto operator()(const T &key) const noexcept -> u32 {
        static_assert((sizeof(key) & 3u) == 0);
        return murmur3((const u32 *)&key, sizeof(key) / 4, 0);
    }
};

export inline constexpr auto hash32(const char *const str, const u32 seed = 0x811c9dc5) noexcept -> u32 {
    return (str[0] == '\0') ? seed : hash32(&str[1], (seed ^ static_cast<u32>(str[1])) * 0x1000193);
}

export inline constexpr auto hash64(const char *const str, const u64 seed = 0xcbf29ce484222325) noexcept -> u32 {
    return (str[0] == '\0') ? seed : hash64(&str[1], (seed ^ static_cast<u32>(str[1])) * 0x100000001b3);
}



export template <typename Arg, typename... Args>
struct any;

template <typename Arg>
struct any<Arg> {
    static constexpr bool value = Arg::value;
};

template <typename Arg1, typename Arg2, typename... Args>
struct any<Arg1, Arg2, Args...> {

    static constexpr bool value = (Arg1::value || Arg2::value) ? true : any<Args...>::value;
};

//    template <bool Args, bool... Args>
//    struct any;
//
//    template <bool Arg>
//    struct any<Arg> {
//        static constexpr bool value = Arg;
//    };
//
//    template <bool Arg1, bool Arg2, bool... Args>
//    struct any<Arg1, Arg2, Args...> {
//        static constexpr bool value = (Arg1 || Arg2) ? true : any<Args...>::value;
//    };

export template <typename T, typename Archive>
struct has_member_serialize {

    template <typename TT>
    static constexpr decltype(std::declval<TT>().serialize(std::declval<Archive>())) test(int);

    template <typename>
    static constexpr u64 test(...);

    static constexpr bool value = std::is_same<decltype(test<T>(0)), void>::value;
};

export template <typename T, typename Archive>
struct has_free_serialize {

    template <typename TT>
    static constexpr decltype(serialize(std::declval<Archive>(), std::declval<TT &>())) test(int);

    template <typename>
    static constexpr u64 test(...);

    static constexpr bool value = std::is_same<decltype(test<T>(0)), void>::value;
};

export template <typename F, typename V = void>
struct function_traits {};

template <typename F>
struct function_traits<F, std::void_t<decltype(&F::operator())>> {
  private:
    typedef function_traits<decltype(&F::operator())> tr;

  public:
    typedef typename tr::return_type return_type;

    constexpr static i32 arity = tr::arity - 1;

    //        template <i32 Index>
    //        struct arg : tr::template arg<Index + 1> {};
};

template <typename R, typename... Args>
struct function_traits<R(Args...)> {

    typedef R return_type;

    constexpr static i32 arity = sizeof...(Args);

    //        template <i32 Index>
    //        struct arg {
    //            static_assert(Index < arity, "argument index is out of range");
    //            typedef typename std::tuple_element<Index, std::tuple<Args...>>::type type;
    //        };
};

template <typename F>
struct function_traits<F &> : function_traits<F> {};

template <typename F>
struct function_traits<F &&> : function_traits<F> {};

template <typename R, typename... Args>
struct function_traits<R (*)(Args...)> : function_traits<R(Args...)> {};

template <typename C, typename R, typename... Args>
struct function_traits<R (C::*)(Args...)> : function_traits<R(C &, Args...)> {};

template <typename C, typename R, typename... Args>
struct function_traits<R (C::*)(Args...) const> : function_traits<R(C const &, Args...)> {};

template <typename C, typename R>
struct function_traits<R(C::*)> : function_traits<R(C &)> {};


} // namespace ende::util
