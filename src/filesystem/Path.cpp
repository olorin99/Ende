//
// Created by cstro29 on 28/5/21.
//

#include "Ende/filesystem/Path.h"
#include <Ende/sys/filesystem.h>

ende::fs::Path::Path(std::string &&path)
    : _path(std::forward<std::string>(path))
{
    split();
}

ende::fs::Path::Path(const Path &p)
    : _path(p._path),
    _components(p._components)
{}

ende::fs::Path::Path(Path &&p) {
    std::swap(_path, p._path);
    std::swap(_components, p._components);
}

ende::fs::Path & ende::fs::Path::operator=(const Path &p) {
    _path = p._path;
    _components = p._components;
    return *this;
}

ende::fs::Path & ende::fs::Path::operator=(Path &&p) noexcept {
    std::swap(_path, p._path);
    std::swap(_components, p._components);
    return *this;
}


std::string ende::fs::Path::operator*() const {
    return _path;
}

ende::fs::Path ende::fs::Path::operator/(const Path &rhs) {
    Path tmp = *this;
    tmp /= rhs;
    return tmp;
}

ende::fs::Path& ende::fs::Path::operator/=(const Path &rhs) {
    _path.reserve(_path.size() + rhs._path.size() + 1);
    _path += PATH_DELIM + rhs._path;
    split();
    return *this;
}

bool ende::fs::Path::operator==(const Path &rhs) const {
    return _path == rhs._path;
}

bool ende::fs::Path::operator!=(const Path &rhs) const {
    return _path != rhs._path;
}


void ende::fs::Path::pop() {
    _path.erase(_components.back(), std::string::npos);
}

void ende::fs::Path::clear() {
    _path.clear();
    _components.clear();
}

bool ende::fs::Path::empty() const {
    return _path.empty() && _components.empty();
}

bool ende::fs::Path::isFile() const {
    return sys::stat(_path).unwrap_or({}).fileType.file();
}

bool ende::fs::Path::isDirectory() const {
    return sys::isdir(_path);
}



ende::fs::Path::Iterator ende::fs::Path::begin() {
    return Iterator(*this, _components.begin());
}

ende::fs::Path::Iterator ende::fs::Path::end() {
    return Iterator(*this, _components.end());
}

const ende::fs::Path::Iterator ende::fs::Path::begin() const {
    return Iterator(*this, _components.begin());
}

const ende::fs::Path::Iterator ende::fs::Path::end() const {
    return Iterator(*this, _components.end());
}




void ende::fs::Path::normalize() {
    u64 index = 0;
    while ((index = _path.find(PATH_DELIM + PATH_DELIM)) != std::string::npos)
        _path.erase(index, 1);
}

void ende::fs::Path::split() {
    _components.clear();

    i32 index = 0;
    u64 last = 0;

    while ((index = _path.find(PATH_DELIM, index)) != std::string::npos) {
        _components.push(index);
        last = ++index;
    }

}