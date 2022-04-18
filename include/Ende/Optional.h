//
// Created by cstro29 on 19/5/21.
//

#ifndef ENDE_OPTIONAL_H
#define ENDE_OPTIONAL_H

#include <Ende/platform.h>
#include <type_traits>
#include <utility>
#include <cassert>
#include <stdexcept>

namespace ende {

    namespace detail {

        struct Dummy {
        };

        template<typename T, bool = std::is_trivially_destructible<T>::value>
        struct OptionalStorage {

            constexpr OptionalStorage() noexcept
                    : _dummy{},
                      _valid(false) {}

            constexpr OptionalStorage(T &&t)
                    : _value(std::forward<T>(t)),
                      _valid(true) {}

            template<typename... Args>
            constexpr OptionalStorage(i32, Args &&... args)
                    : _value(std::forward<Args>(args)...),
                      _valid(true) {}

            ~OptionalStorage() {
                if (_valid) {
                    _value.~T();
                    _valid = false;
                }
            }

            union {
                Dummy _dummy;
                T _value;
            };
            bool _valid;

        };


        template<typename T>
        struct OptionalStorage<T, true> {

            constexpr OptionalStorage() noexcept
                    : _dummy{},
                      _valid(false) {}

            constexpr OptionalStorage(T &&t)
                    : _value(std::forward<T>(t)),
                      _valid(true) {}

            template<typename... Args>
            constexpr OptionalStorage(i32, Args &&... args)
                    : _value(std::forward<Args>(args)...),
                      _valid(true) {}

            union {
                Dummy _dummy;
                T _value;
            };
            bool _valid;

        };

    }

}

struct none {
    struct init_tag {
    };

    constexpr explicit none(init_tag) {}
};

constexpr none None(none::init_tag{});

namespace ende {

    template<typename T>
    class Optional {
    public:

        typedef T value_type;

        constexpr Optional() noexcept {};

        constexpr Optional(none n) noexcept {};

        constexpr explicit Optional(T &&t)
                : _storage(std::forward<T>(t)) {}

        template<typename... Args>
        constexpr Optional(Args &&... args)
                : _storage(0, std::forward<Args>(args)...) {}

//        constexpr Optional(const Optional& rhs)
//            : _storage(rhs._storage)
//        {}
//
        constexpr Optional(Optional&& rhs) noexcept {
            std::swap(_storage._value, rhs._storage._value);
            std::swap(_storage._valid, rhs._storage._valid);
        }


        constexpr Optional &operator=(Optional &&rhs) noexcept {
            std::swap(_storage._value, rhs._storage._value);
            std::swap(_storage._valid, rhs._storage._valid);
        }


        constexpr void reset() {
            _storage = detail::OptionalStorage<T>();
        }

        constexpr bool valid() const {
            return _storage._valid;
        }

        T& operator*() {
            assert(valid());
            return _storage._value;
        }

        constexpr T &operator*() const {
            assert(valid());
            return _storage._value;
        }

        T* operator->() {
            static_assert(!std::is_reference<T>::value);
            assert(valid());
            return &_storage._valid;
        }

        constexpr T *operator->() const {
            static_assert(!std::is_reference<T>::value);
            assert(valid());
            return &_storage._value;
        }

        constexpr explicit operator bool() const {
            return valid();
        }


        constexpr T unwrap() {
            return valid() ? std::move(_storage._value) : throw std::runtime_error("invalid optional");
        }

        constexpr T unwrap() const {
            if (valid())
                return _storage._value;
            // TODO: replace runtime_error with optional_error instead
            throw std::runtime_error("invalid optional");
        }

        constexpr T unwrap_or(const T &t) const {
            if (valid())
                return _storage._value;
            return t;
        }


        template<typename F>
        constexpr Optional<std::invoke_result_t<F, value_type>> map(const F &f) const {
            if (valid())
                return Optional<std::invoke_result_t<F, value_type>>(f(_storage._value));
            return None;
        }

        template<typename F>
        constexpr Optional<std::invoke_result_t<F, value_type>>
        map_or(const std::invoke_result_t<F, value_type> &u, const F &f) const {
            if (valid())
                return Optional<std::invoke_result_t<F, value_type>>(f(_storage._value));
            return Optional<std::invoke_result_t<F, value_type>>(u);
        }


    private:

        detail::OptionalStorage<T> _storage;

    };

}

template <typename T>
constexpr ende::Optional<typename std::decay<T>::type> Some(T&& t) {
    return ende::Optional<typename std::decay<T>::type>(std::forward<T>(t));
}

#define TRY_OPT(expr) \
    ({ \
        auto res = (expr); \
        if (!res) \
            return NONE; \
        res.unwrap(); \
    })

//#define MATCH(opt) (opt ? opt.unwrap() : throw "")

//    template <typename T>
//    constexpr Optional<T> Some(T&& t) {
//        return Optional<T>(std::forward<T>(t));
//    }

//}

#endif //ENDE_OPTIONAL_H
