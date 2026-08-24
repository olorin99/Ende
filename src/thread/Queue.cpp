module;

#include <condition_variable>
#include <thread>
#include <mutex>
#include <atomic>
#include <functional>

export module ende.thread.Queue;

import ende.thread.Semaphore;
import ende.util;

namespace ende::thread {

    struct Command {
        std::vector<std::pair<TimelineSemaphore*, u64>> waits = {};
        std::vector<std::pair<TimelineSemaphore*, u64>> signals = {};
        std::function<i32()> callback = {};
    };

    export class Queue {
    public:

        Queue() {
            _thread = std::thread([&]() {
                while (true) {
                    Command command;
                    {
                        std::unique_lock lock(_commandMutex);
                        _ready.wait(lock, [this]() { return _stop || (!_commands.empty() && _running); });
                        if (_stop) break;
                        command = std::move(_commands.front());
                        _commands.erase(_commands.begin());
                    }
                    for (auto& wait : command.waits) {
                        wait.first->wait(wait.second);
                    }
                    auto error = command.callback();
                    if (error != 0) {
                        std::unique_lock lock(_commandMutex);
                        _error = error;
                    }
                    for (auto& signal : command.signals) {
                        signal.first->signal(signal.second);
                    }
                }
            });
        }

        ~Queue() {
            std::unique_lock lock(_commandMutex);
            _stop = true;
            _ready.notify_all();
            lock.unlock();

            if (_thread.joinable()) _thread.join();
        }

        void submit(const Command& command) {
            std::unique_lock lock(_commandMutex);
            _commands.emplace_back(command);
            _ready.notify_one();
        }

        auto timeline() -> TimelineSemaphore& {
            return _timeline;
        }

    private:
        std::thread _thread = {};
        TimelineSemaphore _timeline = {};
        i32 _error = 0;

        std::vector<Command> _commands = {};
        std::mutex _commandMutex = {};
        std::condition_variable _ready = {};
        std::atomic<bool> _stop = false;
        std::atomic<bool> _running = true;
    };

}
