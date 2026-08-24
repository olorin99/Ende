module;

#include <atomic>
#include <condition_variable>
#include <mutex>

export module ende.thread.Semaphore;

import ende.util;

namespace ende::thread {

    export class TimelineSemaphore {
    public:

        TimelineSemaphore(u64 v = 0) : _value(v) {}

        TimelineSemaphore(const TimelineSemaphore& t) {
            std::lock_guard lock(t._mutex);
            std::lock_guard lock1(_mutex);
            _value.store(t._value.load(std::memory_order_acquire), std::memory_order_release);
            _localValue.store(t._localValue.load(std::memory_order_acquire), std::memory_order_release);
        }

        TimelineSemaphore& operator=(const TimelineSemaphore& t) {
            std::lock_guard lock(t._mutex);
            std::lock_guard lock1(_mutex);
            _value.store(t._value.load(std::memory_order_acquire), std::memory_order_release);
            _localValue.store(t._localValue.load(std::memory_order_acquire), std::memory_order_release);
            return *this;
        }

        void signal(u64 v) {
            {
                std::lock_guard lock(_mutex);
                if (v > _value.load(std::memory_order_relaxed)) {
                    _value.store(v, std::memory_order_release);
                }
            }
            _cv.notify_all();
        }

        void wait(u64 v) {
            std::unique_lock lock(_mutex);
            _cv.wait(lock, [&]() -> bool {
                return _value.load(std::memory_order_acquire) >= v;
            });
        }

        auto value() const -> u64 {
            return _value.load(std::memory_order_acquire);
        }

        auto localValue() const -> u64 {
            return _localValue.load(std::memory_order_acquire);
        }

        auto increment() -> u64 {
            _localValue++;
            return localValue();
        }

    private:
        std::atomic<u64> _value;
        std::atomic<u64> _localValue;
        std::condition_variable _cv;
        mutable std::mutex _mutex;
    };

}
