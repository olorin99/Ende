
#include <Ende/thread/ThreadPool.h>

#include <Ende/profile/profile.h>
#include <Ende/math/random.h>

#include <chrono>
#include <iostream>
#include <thread>

int main() {

    ende::thread::ThreadPool pool(4, true);

    for (u32 i = 0; i < 100; i++) {
        pool.addJob([i]() {
            auto waitTime = ende::math::rand(0, 1000);
            std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
            std::cout << "This is job number: " << i << "\nwaited for: " << waitTime << "ms\n";
            return true;
        });
    }


    std::cout << "workers: " << pool.workerCount() << '\n';
    std::cout << "processed: " << pool.processedJobs() << '\n';
    std::cout << "jobs: " << pool.jobCount() << '\n';
    pool.start();
    pool.wait();

    std::vector<std::future<u32>> futures = {};
    for (u32 i = 0; i < 100; i++) {
        futures.push_back(pool.addJob([] (u32 index, u32 rand) -> u32 {
            return index * rand;
        }, i, std::rand()));
    }

    for (auto& future : futures) {
        future.wait();
        std::cout << "num: " << future.get() << '\n';
    }

    return 0;
}