//
// Created by cstro29 on 20/5/21.
//

#ifndef ENDE_SHARED_H
#define ENDE_SHARED_H

#include <Ende/platform.h>
#include <Ende/util/hash.h>
#include <utility>
#include <cassert>

namespace ende {

    // reference counter
    template <typename T>
    class Shared {
    public:

        Shared(T* ptr = nullptr)
                : _controller(new controller{1, ptr})
        {}

        ~Shared() {
            detach();
        }


        Shared(const Shared& rc)
                : _controller(rc._controller)
        {
            ++_controller->count;
        }

        Shared(Shared&& rc) noexcept
                : _controller(rc._controller)
        {
            rc._controller = nullptr;
        }

        Shared& operator=(const Shared& rc) {
            if (_controller == rc._controller)
                return *this;
            detach();
            _controller = rc._controller;
            if (_controller)
                ++_controller->count;
            return *this;
        }

        Shared& operator=(Shared&& rc) noexcept {
            if (_controller == rc._controller)
                return *this;
            detach();
            _controller = rc._controller;
            rc._controller = nullptr;
            return *this;
        }


        explicit Shared(const T& t)
                : _controller{new controller{1, new T(t)}}
        {}

        explicit Shared(T&& t) noexcept
                : _controller{new controller{1, new T(std::move(t))}}
        {}


        Shared& operator=(const T& t) {
            detach();
            _controller->resource = new T(t);
            return *this;
        }

        Shared& operator=(T&& t) noexcept {
            detach();
            _controller->resource = new T(std::move(t));
            return *this;
        }


        Shared& operator=(T* ptr) {
            detach();
            *this = Shared(ptr);
            return *this;
        }



//        Shared& operator=(T* ptr) noexcept {
//            detach();
//            if (!_controller)
//                _controller = new controller{1, ptr};
//            else
//
//            return *this;
//        }


        T& operator*() const {
            assert(_controller && _controller->resource);
            return *_controller->resource;
        }

        T* operator->() const {
            assert(_controller && _controller->resource);
            return _controller->resource;
        }


        T* get() const {
            assert(_controller && _controller->resource);
            return _controller->resource;
        }

        bool unique() const noexcept {
            assert(_controller);
            return _controller->count == 1;
        }

        u32 count() const noexcept {
//            if (!_controller)
//                return 0;
            assert(_controller);
            return _controller->count;
        }


        void swap(Shared& rc) noexcept {
            controller* tmp = _controller;
            _controller = rc._controller;
            rc._controller = tmp;
        }

        // returns true if last reference and object deleted
        bool detach() {
            if (!_controller)
                return false;
            if (_controller->count > 0) {
                --_controller->count;
                return false;
            } else {
                delete _controller->resource;
                delete _controller;
                _controller = nullptr;
                return true;
            }
        }



        explicit operator bool() const {
            return _controller && _controller->resource;
        }

        bool operator==(const Shared& rhs) const {
            return _controller->resource == rhs._controller->resource;
        }

        bool operator!=(const Shared& rhs) const {
            return _controller->resource != rhs._controller->resource;
        }

        bool operator==(std::nullptr_t) const {
            return _controller->resource == nullptr;
        }



    private:

        struct controller {
            u32 count;
            T* resource;
        } *_controller;

    };



    template <typename T>
    struct util::Hash<Shared<T>> {
        u64 operator()(const Shared<T>& value) {
            if (!value)
                return 0;
            return Hash<T>()(*value.get());
        }
    };


}

#endif //ENDE_SHARED_H
