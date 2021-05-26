//
// Created by cstro29 on 20/5/21.
//

#ifndef ENDE_RESULT_H
#define ENDE_RESULT_H


#include <Ende/platform.h>
#include <Ende/Optional.h>
//#include <format>
//#include <Ende/util/macros.h>
#include <Ende/util/traits.h>
#include <Ende/sys/system.h>

namespace ende {

    namespace detail {

        template<typename T>
        struct ok {
            constexpr ok(T &&t) : value(std::forward<T>(t)) {}
            T value;
        };

        template<typename T>
        struct ok<T &> {
            constexpr ok(T &t) : value(std::addressof(t)) {}
            T *value;
        };


        template<typename E>
        struct err {
            constexpr err(E &&e) : error(std::forward<E>(e)) {}
            E error;
        };

        template<typename E>
        struct err<E &> {
            constexpr err(E &e) : error(std::addressof(e)) {}
            E *error;
        };

    }

}

template <typename T>
constexpr ende::detail::ok<T> Ok(T&& t) {
    return ende::detail::ok<T>(std::forward<T>(t));
}

template <typename E = bool>
constexpr ende::detail::err<E> Err(E&& e = E()) {
    return ende::detail::err<E>(std::forward<E>(e));
}



namespace ende {

    namespace detail {

        template <typename T, typename E, bool = std::is_trivially_destructible_v<T> && std::is_trivially_destructible_v<E>>
        struct ResultStorage {

            constexpr ResultStorage() noexcept
                    : _valid(false),
                      _error{}
            {}

            constexpr ResultStorage(T&& t)
                    : _valid(true),
                      _value(std::forward<T>(t))
            {}

            constexpr ResultStorage(E&& e, i32)
                    : _valid(false),
                      _error(std::forward<E>(e))
            {}

            ~ResultStorage() {
                if (_valid)
                    _value.~T();
            }


            bool _valid;
            union {
                T _value;
                E _error;
            };

        };

        template <typename T, typename E>
        struct ResultStorage<T, E, true> {

            constexpr ResultStorage() noexcept
                    : _valid(false),
                      _error{}
            {}

            constexpr ResultStorage(T&& t)
                    : _valid(true),
                      _value(std::forward<T>(t))
            {}

            constexpr ResultStorage(E&& e, i32)
                    : _valid(false),
                      _error(std::forward<E>(e))
            {}

            bool _valid;
            union {
                T _value;
                E _error;
            };

        };

    }



    template <typename T, typename E = bool>
    class [[nodiscard]] Result {
    public:

        typedef T value_type;
        typedef E error_type;

        constexpr inline Result(detail::ok<T>&& ok)
                : _storage(std::move(ok.value))
        {}

        constexpr inline Result(detail::ok<T&>&& ok)
                : _storage(std::move(*ok.value))
        {}

        constexpr inline Result(detail::ok<const T>&& ok)
                : _storage(ok.value)
        {}

        constexpr inline Result(detail::ok<const T&>&& ok)
                : _storage(T(*ok.value))
        {}



        constexpr inline Result(detail::err<E>&& err)
                : _storage(std::forward<detail::err<E>>(err).error, 0)
        {}

        constexpr inline Result(detail::err<E&>&& err)
                : _storage(*std::forward<detail::err<E&>>(err).error, 0)
        {}


        constexpr inline Result(const Result& res) = delete;

        constexpr inline Result(Result&& res) noexcept
                : _storage()
        {
            std::swap(_storage, res._storage);
        }


        constexpr inline bool isOk() const {
            return _storage._valid;
        }

        constexpr inline bool isErr() const {
            return !isOk();
        }


        constexpr inline explicit operator bool() const {
            return isOk();
        }



        constexpr inline Optional<T> ok() const {
            if (isOk())
                return Some(_storage._value);
            return None;
        }

        constexpr inline Optional<E> err() const {
            if (isErr())
                return Some(_storage._error);
            return None;
        }



        constexpr inline T unwrap() {
            return isOk() ? std::move(_storage._value) : throw std::runtime_error("Invalid result: {}");
//            return isOk() ? std::move(_storage._value) : throw std::runtime_error(std::format("Invalid result: {}", _storage._error));
        }

        constexpr inline T unwrap() const {
            return isOk() ? _storage._value : throw std::runtime_error("Invalid result: {}");
//            return isOk() ? _storage._value : throw std::runtime_error(std::format("Invalid result: {}", _storage._error));
        }


        constexpr inline T unwrapOr(T&& t) {
            if (isOk())
                return std::move(_storage._value);
            return std::forward<T>(t);
        }

        constexpr inline T unwrapOr(T&& t) const {
            if (isOk())
                return _storage._value;
            return std::forward<T>(t);
        }

        constexpr inline E unwrapErr() const {
            return isErr() ? _storage._error : throw std::runtime_error("Valid result");
        }

        constexpr inline T expect(const std::string& msg) const {
            return isOk() ? _storage._value : throw std::runtime_error("{}, {}");
//            return isOk() ? _storage._value : throw std::runtime_error(ende::format("{}, {}", msg, _storage._error));
        }



        template <typename U>
        constexpr inline bool valType() const {
            return std::is_same_v<T, U>;
        }

        template <typename F>
        constexpr inline bool errType() const {
            return std::is_same_v<E, F>;
        }



        template <typename F>
        constexpr inline auto map(const F& f) -> Result<std::invoke_result_t<F, value_type>, E> {
            static_assert(util::function_traits<F>::arity == 1, "map function needs single parameter");
            static_assert(std::is_same<typename util::function_traits<F>::template arg<0>::type, T>::value, "map function parameter needs to be same as T");
            if (isOk())
                return Ok(f(_storage._value));
            return Err(_storage._error);
        }

        template <typename F>
        constexpr inline auto mapErr(const F& f) -> Result<T, std::invoke_result_t<F, error_type>> {
            if (isOk())
                return Ok(_storage._value);
            return Err(f(_storage._error));
        }

        template <typename F>
        constexpr inline auto andThen(const F& f) -> std::invoke_result_t<F, value_type> {
            if (isOk())
                return f(_storage._value);
            return Err(_storage._error);
        }

    private:

        detail::ResultStorage<T, E> _storage;

    };




    // specialization for references
    template <typename T, typename E>
    class [[nodiscard]] Result<T&, E> {
    public:

        typedef T& value_type;
        typedef E error_type;

        constexpr inline Result(detail::ok<T&>&& ok)
                : _valid(true),
                  _value(std::forward<detail::ok<T&>>(ok).value)
        {}

        constexpr inline Result(detail::err<E>&& err)
                : _valid(false),
                  _error(std::forward<detail::err<E>>(err).error)
        {}

        constexpr inline Result(detail::err<E&>&& err)
                : _valid(false),
                  _error(*std::forward<detail::err<E&>>(err).error)
        {}


        ~Result() {
            if (!_valid)
                _error.~E();
        }


        constexpr inline Result(const Result& res) = delete;

        constexpr inline Result(Result&& res) noexcept
                : _valid(false)
        {
            std::swap(_valid, res._valid);
            if (_valid)
                std::swap(_value, res._value);
            else
                std::swap(_error, res._error);
        }



        constexpr inline bool isOk() const {
            return _valid;
        }

        constexpr inline bool isErr() const {
            return !_valid;
        }


        constexpr inline explicit operator bool() const {
            return isOk();
        }



        constexpr inline Optional<T> ok() const {
            if (isOk())
                return Some(*_value);
            return None;
        }

        constexpr inline Optional<E> err() const {
            if (isErr())
                return Some(_error);
            return None;
        }



        constexpr inline T& unwrap() const {
            if (isOk())
                return *_value;
            throw std::runtime_error("invalid result");
        }

        constexpr inline T& unwrapOr(T&& t) const {
            if (isOk())
                return *_value;
            return std::forward<T>(t);
        }

        constexpr inline E unwrapErr() const {
            if (isErr())
                return _error;
            throw std::runtime_error("valid result");
        }

        constexpr inline T& expect(const std::string& msg) const {
            if (isOk())
                return *_value;
            throw std::runtime_error("{}, {}");
//            throw std::runtime_error(ende::format("{}, {}", msg, _error));
        }




        template <typename F>
        constexpr inline auto map(const F& f) -> Result<std::invoke_result_t<F, value_type>, E> {
            if (isOk())
                return Ok(f(*_value));
            return Err(_error);
        }

        template <typename F>
        constexpr inline auto mapErr(const F& f) -> Result<T, std::invoke_result_t<F, error_type>> {
            if (isOk())
                return Ok(*_value);
            return Err(f(_error));
        }

        template <typename F>
        constexpr inline auto andThen(const F& f) -> std::invoke_result_t<F, value_type> {
//            static_assert(std::is_same<std::invoke_result_t<F, value_type>, T&>::value);
            if (isOk())
                return f(*_value);
            return Err(_error);
        }


    private:

        bool _valid;
        union {
            T* _value;
            E _error;
        };

    };

}


#define TRY(expr) \
    ({ \
        auto res = (expr); \
        if (!res) \
            return Err(res.unwrapErr()); \
        res.unwrap(); \
    })


//template <typename T, typename E>
//struct std::formatter<ende::Result<T, E>> {
//
//    constexpr auto parse(std::format_parse_context& ctx) {
//        return ctx.begin();
//    }
//
//    template <typename FormatContext>
//    auto format(const ende::Result<T, E>& result, FormatContext& ctx) {
//        return format_to(ctx.out(), "Result<{}, {}> is {}", ende::sys::demangle(typeid(T).name()), ende::sys::demangle(typeid(E).name()), (result.isOk() ? "ok" : "err"));
//    }
//};

#endif //ENDE_RESULT_H
