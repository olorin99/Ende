#include "Ende/thread/ThreadPool.h"
#include <Ende/thread/thread.h>

#include <iostream>

ende::thread::ThreadPool::ThreadPool(u8 threadCount, bool start)
    : _running(start),
    _stop(false),
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
                    _jobReady.wait(lock, [this]() { return _stop || (!_jobs.empty() && _running); });
                    if (!_jobs.empty() && _running) {
                        job = _jobs.front();
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

u64 ende::thread::ThreadPool::addJob(std::function<void(u64)> task) {
    std::unique_lock<std::mutex> lock(_jobMutex);
    _jobs.push({_currentJobId, std::move(task)});
    _jobReady.notify_one();
    _jobCount++;
    return _currentJobId++;
}

bool ende::thread::ThreadPool::wait() {
    std::unique_lock<std::mutex> lock(_jobMutex);
    _finished.wait(lock, [this]() { return _jobs.empty(); });
    return true;
}