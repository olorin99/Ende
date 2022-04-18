
#include <Ende/thread/thread.h>
#include <Ende/thread/ThreadPool.h>

#include <iostream>

int main() {

    ende::thread::ThreadPool pool(2);

    for (u32 i = 0; i < 1000; i++) {
        pool.addJob([](u64 id) {
            std::cout << "This is job number: " << id << "\n";
        });
    }


    std::cout << "processed: " << pool.processedJobs() << '\n';
    std::cout << "jobs: " << pool.jobCount() << '\n';

    return 0;
}