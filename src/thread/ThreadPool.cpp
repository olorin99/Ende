#include "Ende/thread/ThreadPool.h"
#include <Ende/thread/thread.h>

#include <iostream>

ende::thread::ThreadPool::ThreadPool(u8 threadCount)
    : _stop(false),
    _currentJobId(1),
    _jobCount(0),
    _processed(0),
    _jobs(ende::Vector<Job>())
{
    _workers.reserve(threadCount);
    for (i32 i = 0; i < threadCount; i++) {
        _workers.push(std::thread([&]() {
            while (true) {
                Job job;
                {
                    std::unique_lock<std::mutex> lock(_jobMutex);
                    _jobReady.wait(lock, [this]() { return _stop || !_jobs.empty(); });
                    if (!_jobs.empty()) {
                        job = _jobs.front();
                        _jobs.erase(_jobs.begin());
                    } else if (_stop)
                        break;
                }
                job.task(job.id);
                _processed++;
                _jobCount--;
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

u64 ende::thread::ThreadPool::addJob(std::function<void(u64)> task) {
    std::unique_lock<std::mutex> lock(_jobMutex);
    _jobs.push({_currentJobId, std::move(task)});
    _jobReady.notify_one();
    _jobCount++;
    return _currentJobId++;
}