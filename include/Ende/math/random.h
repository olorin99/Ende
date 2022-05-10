



//
// Created by cstro29 on 30/6/21.
//

#ifndef ENDE_RANDOM_H
#define ENDE_RANDOM_H

#include <Ende/platform.h>

namespace ende::math {

    class xorshift {
    public:

        typedef u32 result_type;

        struct state_type {
            result_type x, y, z, w;
        };


        xorshift();

        explicit xorshift(result_type r);

        explicit  xorshift(const state_type& state);


        result_type operator()();


        void seed();

        void seed(result_type r);

        void seed(const state_type& state);

        void discard(u32 z);

        const state_type& state() const;

        void state(const state_type& state);

        constexpr static result_type min();

        constexpr static result_type max();

    private:

        state_type _state;

    };


    template <typename T>
    T rand(T min, T max);

}

#endif //ENDE_RANDOM_H
