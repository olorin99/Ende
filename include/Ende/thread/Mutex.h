#ifndef ENDE_MUTEX_H
#define ENDE_MUTEX_H

#include <Ende/platform.h>
#include <expected>
#include <mutex>

namespace ende::thread {

    template <typename T>
    class MutexLock;

    template <typename T>
    class Mutex { //TODO: make so actually works
    public:

        Mutex(const T& t)
            : _mutex(new std::mutex),
            _data(t)
        {}

        Mutex(T&& t)
            : _mutex(new std::mutex),
            _data(std::forward<T>(t))
        {}

        Mutex(const Mutex&) = delete;

        Mutex(Mutex&& mutex) noexcept {
            std::swap(_mutex, mutex._mutex);
            std::swap(_data, mutex._data);
        }

        Mutex& operator=(const Mutex&) = delete;

        Mutex& operator=(Mutex&& mutex) noexcept {
            std::swap(_mutex, mutex._mutex);
            std::swap(_data, mutex._data);
            return *this;
        }


        auto lock() -> MutexLock<T> {
            _mutex->lock();
            return std::move(MutexLock<T>(this));
        }

        auto tryLock() -> std::expected<MutexLock<T>, int> {
            if (_mutex->try_lock())
                return std::move(MutexLock<T>(this));
            return std::unexpected(-1);
        }

        void unlock() {
            _mutex->unlock();
        }

    private:
        friend MutexLock<T>;

        std::unique_ptr<std::mutex> _mutex;
        T _data;

    };


    template <typename T>
    class MutexLock {
    public:

        MutexLock(Mutex<T>* mutex)
            : _mutex(mutex)
        {}

        MutexLock(const MutexLock&) = delete;

        MutexLock(MutexLock&& lock) noexcept
            : _mutex(nullptr)
        {
            std::swap(_mutex, lock._mutex);
        }

        ~MutexLock() {
            if (_mutex)
                _mutex->unlock();
        }

        T& operator*() {
            assert(_mutex);
            return _mutex->_data;
        }

    private:
        Mutex<T>* _mutex;
    };

}

#endif //ENDE_MUTEX_H
