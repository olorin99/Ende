#ifndef ENDE_MUTEX_H
#define ENDE_MUTEX_H

#include <Ende/platform.h>
#include <Ende/Pointer.h>
#include <Ende/Result.h>
#include <mutex>

namespace ende::thread {

    template <typename T>
    class MutexLock;

    template <typename T>
    class Mutex {
    public:

        Mutex(const T& t)
            : _mutex(new std::mutex)
            _data(t)
        {}

        Mutex(T&& t)
            : _mutex(new std::mutex)
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


        Result<MutexLock<T>> lock() {
            _mutex->lock();
            return Ok(std::move(MutexLock<T>(this)));
        }

        Result<MutexLock<T>> tryLock() {
            if (_mutex->try_lock())
                return Ok(std::move(MutexLock<T>(this)));
            return Err();
        }

        void unlock() {
            _mutex->unlock();
        }

    private:
        friend MutexLock<T>;

        Pointer<std::mutex> _mutex;
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
