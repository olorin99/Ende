#include "Ende/thread/ThreadPool.h"
#include <Ende/thread/thread.h>

#include <iostream>

ende::thread::ThreadPool::ThreadPool(u8 threadCount, bool start)
    : _running(start),
    _stop(false),
    _currentJobId(1),
    _jobCount(0),
    _processed(0),
    _jobs(std::vector<Job>())
{
    _workers.reserve(threadCount);
    for (i32 i = 0; i < threadCount; i++) {
        _workers.push_back(std::thread([&]() {
            while (true) {
                Job job;
                {
                    std::unique_lock<std::mutex> lock(_jobMutex);
                    _jobReady.wait(lock, [this]() { return _stop || (!_jobs.empty() && _running); });
                    if (!_jobs.empty() && _running) {
                        job = std::move(_jobs.front());
                        _jobs.erase(_jobs.begin());


                        lock.unlock();
                        job.task(job.id);
                        job = {};
                        _processed++;
                        _jobCount--;

                        lock.lock();
                        _finished.notify_one();
                    } else if (_stop)
                        break;
                }
            }
        }));
    }
}

ende::thread::ThreadPool::~ThreadPool() {
    std::unique_lock<std::mutex> lock(_jobMutex);
    _stop = true;
    _jobReady.notify_all();
    lock.unlock();

    for (auto& worker : _workers) {
        if (worker.joinable())
            worker.join();
    }

    std::cout << "processed jobs: " << _processed << "\n";
}

auto ende::thread::ThreadPool::wait() -> bool {
    std::unique_lock<std::mutex> lock(_jobMutex);
    _finished.wait(lock, [this]() { return _jobs.empty(); });
    return true;
}