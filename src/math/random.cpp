//
// Created by cstro29 on 30/6/21.
//

#include "Ende/math/random.h"
#include <limits>
#include <random>

ende::math::xorshift::xorshift()
    : _state{123456789, 362436069, 52188629, 88675123}
{}

ende::math::xorshift::xorshift(result_type r)
    : _state{123456789, 362436069, 52188629, r}
{}

ende::math::xorshift::xorshift(const state_type &state)
    : _state(state)
{}

ende::math::xorshift::result_type ende::math::xorshift::operator()() {
    result_type t = _state.x ^ (_state.x << 15);
    _state.x = _state.y;
    _state.y = _state.z;
    _state.z = _state.w;
    _state.w = _state.w ^ (_state.w >> 21) ^ (t ^ (t >> 4));
    return _state.w;
}

void ende::math::xorshift::seed() {
    _state = {123456789, 362436069, 52188629, 88675123};
}

void ende::math::xorshift::seed(result_type r) {
    _state = {123456789, 362436069, 52188629, r};
}

void ende::math::xorshift::seed(const state_type &state) {
    _state = state;
}

void ende::math::xorshift::discard(u32 z) {
    while (z--)
        (*this)();
}

const ende::math::xorshift::state_type & ende::math::xorshift::state() const {
    return _state;
}

void ende::math::xorshift::state(const state_type &state) {
    _state = state;
}

ende::math::xorshift::result_type ende::math::xorshift::min() {
    return std::numeric_limits<result_type>::min();
}

ende::math::xorshift::result_type ende::math::xorshift::max() {
    return std::numeric_limits<result_type>::max();
}

thread_local ende::math::xorshift seed = ende::math::xorshift(std::random_device()());


template <>
f32 ende::math::rand(f32 min, f32 max) {
    return std::uniform_real_distribution<f32>(min, max)(seed);
}

template <>
f64 ende::math::rand(f64 min, f64 max) {
    return std::uniform_real_distribution<f64>(min, max)(seed);
}

template <>
i32 ende::math::rand(i32 min, i32 max) {
    return std::uniform_int_distribution<i32>(min, max)(seed);
}

template <>
u32 ende::math::rand(u32 min, u32 max) {
    return std::uniform_int_distribution<u32>(min, max)(seed);
}

template <>
i64 ende::math::rand(i64 min, i64 max) {
    return std::uniform_int_distribution<i64>(min, max)(seed);
}

template <>
u64 ende::math::rand(u64 min, u64 max) {
    return std::uniform_int_distribution<u64>(min, max)(seed);
}