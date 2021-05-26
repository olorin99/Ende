//
// Created by cstro29 on 19/5/21.
//

#ifndef ENDE_SPAN_H
#define ENDE_SPAN_H

#include <Ende/platform.h>
#include <type_traits>
#include <array>

namespace ende {

    template <typename T>
    class Span {
    public:

        typedef T* iterator;
        typedef const T* const_iterator;


        constexpr Span(std::nullptr_t) noexcept
            : _data(nullptr),
            _size(0)
        {}

        constexpr Span() noexcept
            : _data(nullptr),
            _size(0)
        {}

        constexpr Span(T* data, u32 size) noexcept
            : _data(data),
            _size(size)
        {}

        constexpr Span(T* start, T* end) noexcept
            : _data(start),
            _size(end - start)
        {}

        template <std::size_t N>
        constexpr Span(T (&arr)[N]) noexcept
            : _data(arr),
            _size(N)
        {}

        template <u32 N>
        constexpr Span(std::array<T, N>& array) noexcept
            : _data(array.data()),
            _size(N)
        {}

        template <typename Container>
        constexpr Span(Container& container)
            : _data(std::data(container)),
            _size(std::size(container))
        {}


        template <typename U, typename std::is_convertible<U, T>::value>
        Span(Span<U>&& span) noexcept {
            std::swap(_data, span._data);
            std::swap(_size, span._size);
        }


        template <typename Container>
        constexpr Span& operator=(Container& container) {
            _data = std::data(container);
            _size = std::size(container);
            return *this;
        }



        constexpr u32 size() const noexcept {
            return _size;
        }

        constexpr T* data() noexcept {
            return _data;
        }

        constexpr const T* data() const noexcept {
            return _data;
        }

        constexpr T& operator[](i32 index) noexcept {
            return *(_data + index);
        }

        constexpr const T& operator[](i32 index) const noexcept {
            return *(_data + index);
        }

        constexpr T& front() noexcept {
            return *_data;
        }

        constexpr const T& front() const noexcept {
            return *_data;
        }

        constexpr T& back() noexcept {
            return *(_data + _size - 1);
        }

        constexpr const T& back() const noexcept {
            return *(_data + _size - 1);
        }

        constexpr T* begin() noexcept {
            return _data;
        }

        constexpr const T* begin() const noexcept {
            return _data;
        }

        constexpr T* end() noexcept {
            return _data + _size;
        }

        constexpr const T* end() const noexcept {
            return _data + _size;
        }

        template <u32 Offset, u32 Count>
        constexpr Span<T> subspan() const noexcept {
            return Span(data() + Offset, Count);
        }

        constexpr Span<T> subspan(u32 offset, u32 count) const noexcept {
            return Span(data() + offset, count);
        }


        explicit operator bool() const {
            return _data;
        }

    private:

        T* _data;
        u32 _size;

    };



    // specialization for const char*
    template <>
    class Span<const char> {
    public:

        typedef const char* iterator;
        typedef const char* const_iterator;


        constexpr Span(std::nullptr_t) noexcept
        : _data(nullptr),
        _size(0)
        {}

        constexpr Span() noexcept
                : _data(nullptr),
                  _size(0)
        {}

        constexpr Span(const char* data, u32 size) noexcept
                : _data(data),
                  _size(size)
        {}

        constexpr Span(const char* start, const char* end) noexcept
                : _data(start),
                  _size(end - start)
        {}

        template <std::size_t N>
        constexpr Span(const char (&arr)[N]) noexcept
                : _data(arr),
                  _size(N - 1)
        {}

        template <typename Container>
        constexpr Span(Container& container)
                : _data(std::data(container)),
                  _size(std::size(container))
        {}


        template <typename U, typename std::is_convertible<U, const char>::value>
        Span(Span<U>&& span) noexcept {
            std::swap(_data, span._data);
            std::swap(_size, span._size);
        }


        template <typename Container>
        constexpr Span& operator=(Container& container) {
            _data = std::data(container);
            _size = std::size(container);
            return *this;
        }



        constexpr u32 size() const noexcept {
            return _size;
        }

        constexpr const char* data() noexcept {
            return _data;
        }

        constexpr const char* data() const noexcept {
            return _data;
        }

        constexpr const char& operator[](i32 index) noexcept {
            return *(_data + index);
        }

        constexpr const char& operator[](i32 index) const noexcept {
            return *(_data + index);
        }

        constexpr const char& front() noexcept {
            return *_data;
        }

        constexpr const char& front() const noexcept {
            return *_data;
        }

        constexpr const char& back() noexcept {
            return *(_data + _size - 1);
        }

        constexpr const char& back() const noexcept {
            return *(_data + _size - 1);
        }

        constexpr const char* begin() noexcept {
            return _data;
        }

        constexpr const char* begin() const noexcept {
            return _data;
        }

        constexpr const char* end() noexcept {
            return _data + _size;
        }

        constexpr const char* end() const noexcept {
            return _data + _size;
        }

        template <u32 Offset, u32 Count>
        constexpr Span<const char> subspan() const noexcept {
            return Span(data() + Offset, Count);
        }

        constexpr Span<const char> subspan(u32 offset, u32 count) const noexcept {
            return Span(data() + offset, count);
        }


        explicit operator bool() const {
            return _data;
        }

    private:

        const char* _data;
        u32 _size;

    };


    template <>
    class Span<void> {
    public:

        typedef void* iterator;
        typedef const void* const_iterator;


        constexpr Span(std::nullptr_t) noexcept
        : _data(nullptr),
        _size(0)
        {}

        constexpr Span() noexcept
                : _data(nullptr),
                  _size(0)
        {}

        constexpr Span(void* data, u32 size) noexcept
                : _data(data),
                  _size(size)
        {}

        constexpr Span(void* start, void* end) noexcept
                : _data(start),
                  _size(static_cast<char*>(end) - static_cast<char*>(start))
        {}

        template <std::size_t N>
        constexpr Span(void* (&arr)[N]) noexcept
                : _data(arr),
                  _size(N)
        {}

        template <u32 N, typename T>
        constexpr Span(std::array<T, N>& array) noexcept
                : _data(array.data()),
                  _size(N)
        {}

        template <typename Container>
        constexpr Span(Container& container)
                : _data(std::data(container)),
                  _size(std::size(container))
        {}


        template <typename U, typename std::is_convertible<U, void*>::value>
        Span(Span<U>&& span) noexcept {
            std::swap(_data, span._data);
            std::swap(_size, span._size);
        }


        template <typename Container>
        constexpr Span& operator=(Container& container) {
            _data = std::data(container);
            _size = std::size(container);
            return *this;
        }



        constexpr u32 size() const noexcept {
            return _size;
        }

        constexpr void* data() noexcept {
            return _data;
        }

        constexpr const void* data() const noexcept {
            return _data;
        }

        constexpr void* operator[](i32 index) noexcept {
            return static_cast<void*>((static_cast<char*>(_data) + index));
        }

        constexpr const void* operator[](i32 index) const noexcept {
            return static_cast<void*>((static_cast<char*>(_data) + index));
        }

        constexpr void* front() noexcept {
            return _data;
        }

        constexpr const void* front() const noexcept {
            return _data;
        }

        constexpr void* back() noexcept {
            return static_cast<void*>((static_cast<char*>(_data) + _size - 1));
        }

        constexpr const void* back() const noexcept {
            return static_cast<void*>((static_cast<char*>(_data) + _size - 1));
        }

        constexpr void* begin() noexcept {
            return _data;
        }

        constexpr const void* begin() const noexcept {
            return _data;
        }

        constexpr void* end() noexcept {
            return static_cast<void*>((static_cast<char*>(_data) + _size));
        }

        constexpr const void* end() const noexcept {
            return static_cast<void*>((static_cast<char*>(_data) + _size));
        }

        template <u32 Offset, u32 Count>
        constexpr Span<void> subspan() const noexcept {
            return Span(data() + Offset, Count);
        }

        constexpr Span<void> subspan(u32 offset, u32 count) const noexcept {
            return Span(static_cast<char*>(_data) + offset, count);
        }


        explicit operator bool() const {
            return _data;
        }

    private:

        void* _data;
        u32 _size;

    };

    template <>
    class Span<const void> {
    public:

        typedef const void* iterator;
        typedef const void* const_iterator;


        constexpr Span(std::nullptr_t) noexcept
        : _data(nullptr),
        _size(0)
        {}

        constexpr Span() noexcept
                : _data(nullptr),
                  _size(0)
        {}

        constexpr Span(const void* data, u32 size) noexcept
                : _data(data),
                  _size(size)
        {}

        constexpr Span(const void* start, const void* end) noexcept
                : _data(start),
                  _size(static_cast<const char*>(end) - static_cast<const char*>(start))
        {}

        template <std::size_t N>
        constexpr Span(const void* (&arr)[N]) noexcept
                : _data(arr),
                  _size(N)
        {}

        template <u32 N, typename T>
        constexpr Span(std::array<T, N>& array) noexcept
                : _data(array.data()),
                  _size(N)
        {}

        template <typename Container>
        constexpr Span(Container& container)
                : _data(std::data(container)),
                  _size(std::size(container))
        {}


        template <typename U, typename std::is_convertible<U, const void*>::value>
        Span(Span<U>&& span) noexcept {
            std::swap(_data, span._data);
            std::swap(_size, span._size);
        }


        template <typename Container>
        constexpr Span& operator=(Container& container) {
            _data = std::data(container);
            _size = std::size(container);
            return *this;
        }



        constexpr u32 size() const noexcept {
            return _size;
        }

        constexpr const void* data() noexcept {
            return _data;
        }

        constexpr const void* data() const noexcept {
            return _data;
        }

        constexpr const void* operator[](i32 index) noexcept {
            return static_cast<const void*>((static_cast<const char*>(_data) + index));
        }

        constexpr const void* operator[](i32 index) const noexcept {
            return static_cast<const void*>((static_cast<const char*>(_data) + index));
        }

        constexpr const void* front() noexcept {
            return _data;
        }

        constexpr const void* front() const noexcept {
            return _data;
        }

        constexpr const void* back() noexcept {
            return static_cast<const void*>((static_cast<const char*>(_data) + _size - 1));
        }

        constexpr const void* back() const noexcept {
            return static_cast<const void*>((static_cast<const char*>(_data) + _size - 1));
        }

        constexpr const void* begin() noexcept {
            return _data;
        }

        constexpr const void* begin() const noexcept {
            return _data;
        }

        constexpr const void* end() noexcept {
            return static_cast<const void*>((static_cast<const char*>(_data) + _size));
        }

        constexpr const void* end() const noexcept {
            return static_cast<const void*>((static_cast<const char*>(_data) + _size));
        }

        template <u32 Offset, u32 Count>
        constexpr Span<const void> subspan() const noexcept {
            return Span(static_cast<const char*>(_data) + Offset, Count);
        }

        constexpr Span<const void> subspan(u32 offset, u32 count) const noexcept {
            return Span(static_cast<const char*>(_data) + offset, count);
        }


        explicit operator bool() const {
            return _data;
        }

    private:

        const void* _data;
        u32 _size;

    };
}

#endif //ENDE_SPAN_H
