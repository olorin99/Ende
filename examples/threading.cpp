
#include <Ende/thread/thread.h>
#include <Ende/thread/ThreadPool.h>

#include <Ende/profile/profile.h>
#include <Ende/math/random.h>

#include <iostream>

int main() {

    ende::thread::ThreadPool pool(4, true);

    for (u32 i = 0; i < 100; i++) {
        pool.addJob([]() {
            auto waitTime = ende::math::rand(0, 1);
            sleep(waitTime);
            std::cout << "This is job number: " << 1 << "\nwaited for: " << waitTime << "ms\n";
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