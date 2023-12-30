#ifndef ENDE_THREADPOOL_H
#define ENDE_THREADPOOL_H

#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <functional>
#include <Ende/thread/Mutex.h>

namespace ende::thread {

    class ThreadPool {
    public:

        ThreadPool(u8 threadCount = 1, bool start = true);

        ~ThreadPool();

        auto addJob(std::function<void(u64)> task) -> u64;

        auto wait() -> bool;

        void start() { _running = true; }

        void stop() { _running = false; }

        auto workerCount() const -> u32 { return _workers.size(); }

        auto jobCount() const -> u32 { return _jobCount; }

        auto processedJobs() const -> u32 { return _processed; }

    private:

        struct Job {
            u64 id = 0;
            std::function<void(u64)> task = {};
        };

        std::atomic<bool> _stop;
        std::atomic<bool> _running;
        std::atomic<u64> _currentJobId;
        std::vector<std::thread> _workers;

        std::vector<Job> _jobs;
        std::atomic<u32> _jobCount;
        std::atomic<u64> _processed;
        std::mutex _jobMutex;
        std::condition_variable _jobReady;
        std::condition_variable _finished;

    };

}

#endif //ENDE_THREADPOOL_H
