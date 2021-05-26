//
// Created by cstro29 on 19/5/21.
//

#ifndef ENDE_VECTOR_H
#define ENDE_VECTOR_H

#include <Ende/platform.h>
#include <Ende/sys/memory.h>
#include <Ende/Optional.h>
//#include <Ende/util/traits.h>
#include <cassert>
#include <stdexcept>

namespace ende {

    template <typename T>
    class Vector {
    public:

        typedef T value_type;
        typedef T& reference;
        typedef const T& const_reference;
        typedef T* pointer;
        typedef const T* const_pointer;

        typedef pointer iterator;
        typedef const_pointer const_iterator;


        Vector()
                : _capacity(10),
                  _size(0),
                  _data(sys::malloc<T>(_capacity))
        {}

        Vector(u64 size)
                : _capacity(size),
                  _size(0),
                  _data(sys::malloc<T>(_capacity))
        {
            for (u64 i = 0; i < size; i++)
                push(T());
        }


        Vector(u64 size, const T& val)
                : _capacity(size),
                  _size(0),
                  _data(sys::malloc<T>(_capacity))
        {
            for (u64 i = 0; i < size; i++)
                push(val);
        }


        Vector(u64 size, T* data)
                : _capacity(size),
                  _size(0),
                  _data(sys::malloc<T>(_capacity))
        {
            for (u64 i = 0; i < size; i++)
                push(data[i]);
        }


        Vector(const std::initializer_list<T>& list)
                : _capacity(list.size()),
                  _size(0),
                  _data(sys::malloc<T>(_capacity))
        {
            for (auto& i : list)
                push(i);
        }

        ~Vector() {
            clear();
            ::free(_data);
//            if (!_data)
//                ::free(_data);
        }


        // copy constructor
        Vector(const Vector& vector)
                : _capacity(vector._capacity),
                  _size(vector._size),
                  _data(sys::malloc<T>(_capacity))
        {
            for (u64 i = 0; i < _size; i++)
                new (_data + i) T(vector._data[i]);
//                _data[i] = vector._data[i];
        }

        // move constructor
        Vector(Vector&& vector) noexcept
                : _capacity(0),
                  _size(0),
                  _data(nullptr)
        {
            std::swap(_capacity, vector._capacity);
            std::swap(_size, vector._size);
            std::swap(_data, vector._data);
        }

        // copy assignment
        Vector& operator=(const Vector& vector) {
            reserve(vector._capacity);
            _size = vector._size;
            for (u64 i = 0; i < _size; i++)
                new (_data + i) value_type(vector._data[i]);
        };

        // move assignment
        Vector& operator=(Vector&& vector) noexcept {
            std::swap(_capacity, vector._capacity);
            std::swap(_size, vector._size);
            std::swap(_data, vector._data);
        }


        Vector& operator=(std::pair<T*, u64> data) {
            clear();
            reserve(data.second);

            for (u32 i = 0; i < data.second; i++)
                push(data.first[i]);
        }


        void push(const value_type& value) {
            if (_size >= _capacity)
                reserve(_capacity * 2);
            new (_data + _size++) value_type(value);
        }

        void push(value_type&& value) {
            if (_size >= _capacity)
                reserve(_capacity * 2);
            new (_data + _size++) value_type(std::forward<value_type>(value));
        }


        template <typename... Args>
        void emplace(Args&&... args) {
            if (_size >= _capacity)
                reserve(_capacity * 2);
            new (_data + _size++) value_type(std::forward<Args>(args)...);
        }



        Optional<value_type> pop() {
            if (empty())
                return none{none::init_tag{}};
            return Some(std::move(_data[--_size]));
        }

        void clear() {
            while (!empty())
                pop();
        }


        u64 size() const {
            return _size;
        }

        u64 capacity() const {
            return _capacity;
        }

        bool empty() const {
            return _size == 0;
        }



        reference operator[](u64 index) {
            assert(_size > index);
            return _data[index];
        }

        const_reference operator[](u64 index) const {
            assert(_size > index);
            return _data[index];
        }

        reference at(u64 index) {
            if (index > _size)
                throw std::out_of_range("invalid index");
            return _data[index];
        }

        const_reference at(u64 index) const {
            if (index > _size)
                throw std::out_of_range("invalid index");
            return _data;
        }



        reference front() {
            return *_data;
        }

        reference back() {
            return _data[_size - 1];
        }

        const_reference front() const {
            return *_data;
        }

        const_reference back() const {
            return _data[_size - 1];
        }



        pointer data() {
            return _data;
        }

        const_pointer data() const {
            return _data;
        }



        iterator begin() {
            return _data;
        }

        iterator end() {
            return _data + _size;
        }

        const_iterator begin() const {
            return _data;
        }

        const_iterator end() const {
            return _data + _size;
        }



        Optional<iterator> find(const T& t) {
            for (auto it = begin(); it != end(); ++it) {
                if (*it == t)
                    return Some(it);
            }
            return None;
        }

        Optional<const_iterator> find(const T& t) const {
            for (auto it = begin(); it != end(); ++it) {
                if (*it == t)
                    return Some(it);
            }
            return None;
        }



        iterator insert(iterator pos, value_type&& value) {
            if (_size >= _capacity)
                reserve(_capacity * 2);
            if (pos == end()) {
                push(std::forward<value_type>(value));
                return pos;
            }
            std::copy(pos, _data + _size, pos + 1);
            new (pos) value_type(std::forward<value_type>(value));
            _size++;
            return pos;
        }

        iterator insert(iterator pos, iterator first, iterator last) {
            // incase a single increase is not enough
            while (_size + (last - first) >= _capacity)
                reserve(_capacity * 2);
            std::copy(pos, _data + _size, pos + (last - first));
            std::copy(first, last, pos);
            _size += (last - first);
            return pos;
        }

        iterator erase(iterator pos) {
            if (pos == end() - 1) {
                pop();
                return pos;
            }
            pos->~T();
            std::copy(pos + 1, _data + _size, pos);
            _size--;
            return pos;
        }



        bool operator==(const Vector& rhs) const {
            if (_size != rhs._size)
                return false;
            for (u64 i = 0; i < _size; i++) {
                if (_data[i] != rhs._data[i])
                    return false;
            }
            return true;
        }

        bool operator!=(const Vector& rhs) const {
            if (_size != rhs._size)
                return true;
            for (u64 i = 0; i < _size; i++) {
                if (_data[i] != rhs._data[i])
                    return true;
            }
            return false;
        }



        template <typename F>
        Vector<std::invoke_result_t<F, value_type>> map(const F& func) {
            Vector<std::invoke_result_t<F, value_type>> vector;
            vector.reserve(_size);

            for (auto& element : *this)
                vector.push(func(element));
            return vector;
        }

        template <typename F>
        Vector<std::invoke_result_t<F, value_type>> map(const F& func) const {
            Vector<std::invoke_result_t<F, value_type>> vector;
            vector.reserve(_size);

            for (auto& element : *this)
                vector.push(func(element));
            return vector;
        }




        void reserve(u64 capacity) {
            if (_capacity >= capacity)
                return;

            _data = sys::realloc(_data, capacity);
            _capacity = capacity;
        }

        void resize(u64 size) {
            reserve(size);
            while (_size > size)
                pop();
            while (_size < size)
                push(value_type());
        }

        void shrink() {
            _data = sys::realloc(_data, _size > 0 ? _size : 10);
            _capacity = _size;
        }


    private:

        u64 _capacity;
        u64 _size;
        pointer _data;

    };


}

#endif //ENDE_VECTOR_H
