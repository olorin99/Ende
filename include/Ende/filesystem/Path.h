//
// Created by cstro29 on 28/5/21.
//

#ifndef ENDE_PATH_H
#define ENDE_PATH_H

#include <Ende/platform.h>
#include <Ende/Vector.h>

namespace ende::fs {

    class Path {
    public:
#ifdef ENDE_UNIX
        static constexpr char PATH_DELIM = '/';
#else
        static consteval char PATH_DELIM = '\\';
#endif

        Path() = default;

        Path(std::string&& path);


        Path(const Path& p);

        Path(Path&& p);

        Path& operator=(const Path& p);

        Path& operator=(Path&& p) noexcept;


        std::string operator*() const;

        Path operator/=(const Path& rhs);

        bool operator==(const Path& rhs) const;

        bool operator!=(const Path& rhs) const;


        void pop();

        void clear();

        bool empty() const;


        bool isFile() const;

        bool isDirectory() const;



        class Iterator {
        public:

            typedef Iterator self_type;
            typedef std::forward_iterator_tag iterator_category;

            Iterator(const Path& path, i32 const* index)
                : _path(path),
                _index(index)
            {}


            inline std::string operator*() const {
                return _path._path.substr(*_index, (_index + 1) == _path._components.end() ? std::string::npos : *(_index + 1) - *_index);
            }

            inline std::string operator[](std::ptrdiff_t dif) {
//                Vector<i32>::iterator tmp = _index + dif;
                i32 const* tmp = _index + dif;
                return _path._path.substr(*tmp, (tmp + 1) == _path._components.end() ? std::string::npos : *(tmp + 1) - *tmp);
            }

            inline Iterator& operator++() {
                ++_index;
                return *this;
            }

            inline const Iterator operator++(int) {
                Iterator tmp = *this;
                _index++;
                return tmp;
            }



            inline bool operator==(const Iterator& rhs) const {
                return &_path == &rhs._path && _index == rhs._index;
            }

            inline bool operator!=(const Iterator& rhs) const {
                return &_path != &rhs._path || _index != rhs._index;
            }

            inline bool operator<(const Iterator& rhs) const {
                return _index < rhs._index;
            }

            inline bool operator>(const Iterator& rhs) const {
                return _index > rhs._index;
            }

            inline bool operator<=(const Iterator& rhs) const {
                return _index <= rhs._index;
            }

            inline bool operator>=(const Iterator& rhs) const {
                return _index >= rhs._index;
            }


        private:

            const Path& _path;
//            Vector<i32>::iterator _index;
            i32 const* _index;

        };

        Iterator begin();

        Iterator end();

        const Iterator begin() const;

        const Iterator end() const;


    private:

        void normalize();

        void split();

        std::string _path;
        Vector<i32> _components;

    };

}

inline ende::fs::Path operator""_path(const char* str, std::size_t) {
    return ende::fs::Path(str);
}

#endif //ENDE_PATH_H
