#ifndef ENDE_TRAITS_H
#define ENDE_TRAITS_H

#include <Ende/platform.h>
#include <type_traits>

namespace ende::util {

    template <typename Arg, typename... Args>
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



    template <typename T, typename Archive>
    struct has_member_serialize {

        template <typename TT>
        static constexpr decltype(std::declval<TT>().serialize(std::declval<Archive>())) test(int);

        template <typename>
        static constexpr u64 test(...);

        static constexpr bool value = std::is_same<decltype(test<T>(0)), void>::value;

    };


    template <typename T, typename Archive>
    struct has_free_serialize {

        template <typename TT>
        static constexpr decltype(serialize(std::declval<Archive>(), std::declval<TT&>())) test(int);

        template <typename>
        static constexpr u64 test(...);

        static constexpr bool value = std::is_same<decltype(test<T>(0)), void>::value;

    };


    template <typename F, typename V = void>
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
    struct function_traits<F&> : function_traits<F> {};

    template <typename F>
    struct function_traits<F&&> : function_traits<F> {};

    template <typename R, typename... Args>
    struct function_traits<R(*)(Args...)> : function_traits<R(Args...)> {};

    template <typename C, typename R, typename... Args>
    struct function_traits<R(C::*)(Args...)> : function_traits<R(C&, Args...)> {};

    template <typename C, typename R, typename... Args>
    struct function_traits<R(C::*)(Args...) const> : function_traits<R(C const&, Args...)> {};

    template <typename C, typename R>
    struct function_traits<R(C::*)> : function_traits<R(C&)> {};
}

#endif //ENDE_TRAITS_H
