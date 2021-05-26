//
// Created by cstro29 on 20/5/21.
//

#ifndef ENDE_POINTER_H
#define ENDE_POINTER_H

#include <Ende/platform.h>
#include <Ende/util/hash.h>
#include <cassert>

namespace ende {

    template<typename T>
    class Pointer {
    public:

        Pointer(T *ptr = nullptr)
                : _data(ptr) {}

        ~Pointer() {
            delete _data;
        }


        Pointer(const Pointer &ptr) = delete;

        Pointer &operator=(const Pointer &ptr) = delete;

        Pointer(Pointer &&ptr) noexcept
                : _data(ptr._data) {
            ptr._data = nullptr;
        }


        explicit Pointer(const T &t)
                : _data(new T(t)) {}

        explicit Pointer(T &&t) noexcept
                : _data(new T(std::move(t))) {}


        Pointer &operator=(Pointer &&ptr) noexcept {
            _data = ptr._data;
            ptr._data = nullptr;
            return *this;
        }


        T &operator*() noexcept {
            assert(_data != nullptr);
            return *_data;
        }

        const T &operator*() const noexcept {
            assert(_data != nullptr);
            return *_data;
        }

        T *operator->() noexcept {
            assert(_data != nullptr);
            return _data;
        }

        const T *operator->() const noexcept {
            assert(_data != nullptr);
            return _data;
        }


        explicit operator bool() const {
            return _data;
        }


        T *get() const noexcept {
            return _data;
        }

//        T* release() const noexcept {
//            T* tmp = _data;
//            _data = nullptr;
//            return tmp;
//        }

        T *reset(T *ptr = nullptr) noexcept {
            T *tmp = _data;
            _data = ptr;
            return tmp;
        }


        bool operator==(const Pointer &rhs) const noexcept {
            return _data == rhs._data;
        }

        bool operator!=(const Pointer &rhs) const noexcept {
            return _data != rhs._data;
        }


    private:

        T *_data;

    };


    template<typename T>
    class Pointer<T[]> {
    public:

        Pointer(T *ptr = nullptr)
                : _data(ptr) {}

        ~Pointer() {
            delete[] _data;
        }


        Pointer(const Pointer &ptr) = delete;

        Pointer &operator=(const Pointer &ptr) = delete;

        Pointer(Pointer &&ptr) noexcept
                : _data(ptr._data) {
            ptr._data = nullptr;
        }


//        explicit Pointer(const T& t)
//                : _data(new T(t))
//        {}
//
//        explicit Pointer(T&& t) noexcept
//                : _data(new T(std::move(t)))
//        {}




        Pointer &operator=(Pointer &&ptr) noexcept {
            _data = ptr._data;
            ptr._data = nullptr;
            return *this;
        }


        T &operator*() noexcept {
            assert(_data != nullptr);
            return *_data;
        }

        const T &operator*() const noexcept {
            assert(_data != nullptr);
            return *_data;
        }

        T *operator->() noexcept {
            assert(_data != nullptr);
            return _data;
        }

        const T *operator->() const noexcept {
            assert(_data != nullptr);
            return _data;
        }


        T &operator[](u32 index) noexcept {
            return _data[index];
        }

        const T &operator[](u32 index) const noexcept {
            return _data[index];
        }


        explicit operator bool() const {
            return _data;
        }


        T *get() const noexcept {
            return _data;
        }

//        T* release() const noexcept {
//            T* tmp = _data;
//            _data = nullptr;
//            return tmp;
//        }

        T *reset(T *ptr = nullptr) noexcept {
            T *tmp = _data;
            _data = ptr;
            return tmp;
        }


        bool operator==(const Pointer &rhs) const noexcept {
            return _data == rhs._data;
        }

        bool operator!=(const Pointer &rhs) const noexcept {
            return _data != rhs._data;
        }


    private:

        T *_data;

    };

}

#endif //ENDE_POINTER_H
