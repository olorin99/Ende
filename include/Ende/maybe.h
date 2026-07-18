#ifndef ENDE_MAYBE_H
#define ENDE_MAYBE_H

#define GET_maybe_MACRO(_1, _2, NAME, ...) NAME
#define maybe(...) GET_maybe_MACRO(__VA_ARGS__, maybe_2, maybe_1)(__VA_ARGS__)
#define maybe_conv(type, expr) maybe(expr, [](const auto &e) { return static_cast<type>(e); })

#define maybe_1(expr)                                             \
    ({                                                            \
        auto &&_result = (expr);                                  \
        if (!_result) {                                           \
            return ::_maybe_failure_proxy(::_get_error(_result)); \
        }                                                         \
        ::_deref_or_void(std::move(_result));                     \
    })

#define maybe_2(expr, fallback)                                         \
    ({                                                                  \
        auto &&_result = (expr);                                        \
        if (!_result) {                                                 \
            [[maybe_unused]] auto &&_e = ::_get_error(_result);         \
            return ::_maybe_failure_proxy(::_get_result(fallback, _e)); \
        }                                                               \
        ::_deref_or_void(std::move(_result));                           \
    })

#endif
