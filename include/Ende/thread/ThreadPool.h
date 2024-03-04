#ifndef ENDE_THREADPOOL_H
#define ENDE_THREADPOOL_H

#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <functional>
#include <Ende/thread/Mutex.h>
#include <future>
#include <queue>

namespace ende::thread {

    class ThreadPool {
    public:

        ThreadPool(u8 threadCount = 1, bool start = true);

        ~ThreadPool();

        struct Job {
            u64 id = 0;
            std::function<void(u64)> task = {};
        };

        template <typename Func, typename ReturnType = std::invoke_result_t<Func&&>>
        auto addJob(Func&& f) -> std::future<ReturnType> {
            auto p = std::make_shared<std::promise<ReturnType>>();
            auto future = p->get_future();
            auto task = [func = std::move(f), promise = std::move(p)] (u64 id) mutable {
                promise->set_value(func());
            };
            enqueueJob(std::move(task));
            return future;
        }

        template <typename Func, typename... Args, typename ReturnType = std::invoke_result_t<Func&&, Args&&...>>
        auto addJob(Func&& f, Args&&... args) -> std::future<ReturnType> {
            auto p = std::make_shared<std::promise<ReturnType>>();
            auto future = p->get_future();
            auto task = [func = std::move(f), ...largs = std::move(args), promise = std::move(p)] (u64 id) mutable {
                promise->set_value(func(largs...));
            };
            enqueueJob(std::move(task));
            return future;
        }

        auto wait() -> bool;

        void start() { _running = true; }

        void stop() { _running = false; }

        auto workerCount() const -> u32 { return _workers.size(); }

        auto jobCount() const -> u32 { return _jobCount; }

        auto processedJobs() const -> u32 { return _processed; }

    private:

        template <typename Func>
        auto enqueueJob(Func&& f) -> u64 {
            std::unique_lock<std::mutex> lock(_jobMutex);
            _jobs.emplace_back(std::move(Job{_currentJobId, std::forward<Func>(f)}));
            _jobReady.notify_one();
            _jobCount++;
            return _currentJobId++;
        }

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
