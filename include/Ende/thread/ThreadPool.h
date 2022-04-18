#ifndef ENDE_THREADPOOL_H
#define ENDE_THREADPOOL_H

#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <Ende/Vector.h>
#include <functional>
#include <Ende/thread/Mutex.h>

namespace ende::thread {

    class ThreadPool {
    public:

        ThreadPool(u8 threadCount = 1, bool start = true);

        ~ThreadPool();

        u64 addJob(std::function<void(u64)> task);

        bool wait();

        void start() { _running = true; }

        void stop() { _running = false; }

        u32 workerCount() const { return _workers.size(); }

        u32 jobCount() const { return _jobCount; }

        u32 processedJobs() const { return _processed; }

    private:

        struct Job {
            u64 id = 0;
            std::function<void(u64)> task = {};
        };

        std::atomic<bool> _stop;
        std::atomic<bool> _running;
        std::atomic<u64> _currentJobId;
        ende::Vector<std::thread> _workers;

        ende::Vector<Job> _jobs;
        std::atomic<u32> _jobCount;
        std::atomic<u64> _processed;
        std::mutex _jobMutex;
        std::condition_variable _jobReady;
        std::condition_variable _finished;

    };

}

#endif //ENDE_THREADPOOL_H
