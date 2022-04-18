
#include <Ende/thread/thread.h>
#include <Ende/thread/ThreadPool.h>

#include <Ende/profile/profile.h>
#include <Ende/math/random.h>

#include <iostream>

int main() {

    ende::thread::ThreadPool pool(4, false);

    for (u32 i = 0; i < 100; i++) {
        pool.addJob([](u64 id) {
            auto upper = (5_sec).nanoseconds();
            auto waitTime = ende::time::Duration{0, (u32)ende::math::rand((i64)0, upper)};
            ende::thread::sleep(waitTime);
            std::cout << "This is job number: " << id << "\nwaited for: " << waitTime.milliseconds() << "ms\n";
        });
    }


    std::cout << "workers: " << pool.workerCount() << '\n';
    std::cout << "processed: " << pool.processedJobs() << '\n';
    std::cout << "jobs: " << pool.jobCount() << '\n';
//    pool.start();
    pool.wait();

    return 0;
}