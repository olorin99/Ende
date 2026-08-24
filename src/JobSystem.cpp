module;

#include <Ende/maybe.h>
#include <functional>
#include <expected>
#include <string>
#include <memory>
#include <variant>
#include <thread>

export module ende.JobSystem;

import ende.util;
import ende.graph;
import ende.thread.Semaphore;
import ende.thread.Queue;

namespace ende {

    export template <typename... Args> class JobSystem;
    export template <typename... Args> class JobBuilder;

    export enum class JobError {
        NONE,
        IS_CYCLICAL,
        INVALID_JOB,
        INVALID_RESOURCE,
        RESOURCE_TYPE_DOESNT_MATCH,
        INDEX_OUT_OF_BOUNDS,
    };

    auto mapGraphErrorToJobError(graph::Error error) -> JobError {
        return static_cast<JobError>(error);
    }

    export struct ResourceIndex : public graph::Edge {
        i32 index = -1;
    };

    export template <typename T>
        struct Read {
            explicit Read(T r) : resource(std::move(r)) {}
            explicit Read(T&& r) : resource(std::forward<T>(r)) {}
            T resource;
        };

        export template <typename T>
        struct Write {
            explicit Write(T r) : resource(std::move(r)) {}
            explicit Write(T&& r) : resource(std::forward<T>(r)) {}
            T resource;
        };

        export template <typename T>
        struct ReadWrite {
            explicit ReadWrite(T r) : resource(std::move(r)) {}
            explicit ReadWrite(T&& r) : resource(std::forward<T>(r)) {}
            T resource;
        };

    export template <typename... Args>
    class Job : public graph::Vertex<ResourceIndex> {
    public:

        auto name() const -> std::string_view { return _name; }

        auto priority() const -> i32 { return _priority; }

        auto queueIndex() const -> u32 { return _queueIndex; }

        auto index(const Edge& edge) -> std::expected<ResourceIndex, JobError> {
            for (auto& input : inputs) {
                if (input.index == edge.index) {
                    return input;
                }
            }
            for (auto& output : outputs) {
                if (output.index == edge.index) {
                    return output;
                }
            }
            return std::unexpected(JobError::INVALID_RESOURCE);
        }

        template <typename T>
        auto resource(const ResourceIndex& index) -> std::expected<T*, JobError>;

        auto dispatch() -> std::expected<bool, JobError> {
            return _callback(*this);
        }

    private:
        friend JobSystem<Args...>;
        friend JobBuilder<Args...>;

        JobSystem<Args...>* _system = nullptr;

        std::string _name = {};
        std::function<std::expected<bool, JobError>(Job&)> _callback = {};
        i32 _priority = 0;
        u32 _queueIndex = 0;

        std::vector<i32> _accesses = {};
        std::vector<i32> _queueWaits = {};

        void calcAccess() {
            for (const auto& input : inputs) {
                bool append = true;
                for (auto& access : _accesses) {
                    if (access == input.index) {
                        append = false;
                        break;
                    }
                }
                if (append)
                    _accesses.emplace_back(input.index);
            }
            for (const auto& output : outputs) {
                bool append = true;
                for (auto& access : _accesses) {
                    if (access == output.index) {
                        append = false;
                        break;
                    }
                }
                if (append)
                    _accesses.emplace_back(output.index);
            }
        }

    };

    export template <typename... Args> class JobBuilder {
    public:

        using Graph = graph::Graph<Job<Args...>>;
        using Edge = Graph::Edge;

        auto job() -> Job<Args...>&;


        template <typename T, typename U, typename... Vars>
        void unpack(const T& t, const U& u, const Vars&... args) {
            unpack(t);
            unpack(u, args...);
        }

        template <typename T>
        void unpack(const T& t) {
            // job().addParameter(Parameter(t));
        }

        void unpack(const Read<Edge>& t) {
            reads(t.resource);
            unpack(t.resource);
        }

        void unpack(const Write<Edge>& t) {
            writes(t.resource);
            unpack(t.resource);
        }

        void unpack(const ReadWrite<Edge>& t) {
            readWrites(t.resource);
            unpack(t.resource);
        }


        auto reads(Edge edge) -> JobBuilder<Args...>& {
            job().inputs.emplace_back(edge);
            return *this;
        }

        auto writes(Edge edge) -> JobBuilder<Args...>&;

        auto readWrites(Edge edge) -> JobBuilder<Args...>& {
            return reads(edge).writes(edge);
        }


        auto output(const u32 index = 0) -> std::expected<Edge, JobError> {
            return job().output(index).transform_error(mapGraphErrorToJobError);
        }


        template <typename... Dependencies>
        auto depends(const Dependencies&... args) -> JobBuilder<Args...>& {
            unpack(args...);
            return *this;
        }


        template <typename F>
        auto executes(F callback) -> JobBuilder& {
            return executes(util::makeFunction(callback));
        }

        auto executes(const std::function<void(Job<Args...>&)>& callback) -> JobBuilder<Args...>& {
            job()._callback = [callback](Job<Args...>& job) -> std::expected<bool, JobError> {
                try {
                    callback(job);
                } catch (...) {
                    return std::unexpected(JobError::INVALID_JOB);
                }
                return true;
            };
            return *this;
        }

        auto executes(const std::function<std::expected<bool, JobError>(Job<Args...>&)>& callback) -> JobBuilder<Args...>& {
            job()._callback = [callback](Job<Args...>& job) -> std::expected<bool, JobError> {
                try {
                    return callback(job);
                } catch (...) {
                    return std::unexpected(JobError::INVALID_JOB);
                }
                return true;
            };
            return *this;
        }

    private:
        friend JobSystem<Args...>;

        JobSystem<Args...>* _system = nullptr;
        i32 _index = -1;

        JobBuilder(JobSystem<Args...>* system, i32 index) : _system(system), _index(index) {}

    };

    template <typename... Args>
    class JobSystem : public graph::Graph<Job<Args...>> {
    public:

        using Graph = graph::Graph<Job<Args...>>;
        using Resource = std::variant<Args...>;

        JobSystem(u32 queueCount = 1) {
            for (u32 i = 0; i < queueCount; i++) {
                _queues.emplace_back(std::make_unique<thread::Queue>());
            }
        }

        auto addJob(const std::string& name, i32 priority = 0, u32 queueIndex = 0) -> JobBuilder<Args...> {
            auto& vertex = this->addVertex();

            vertex._system = this;
            vertex._name = name;
            vertex._priority = priority;
            vertex._queueIndex = queueIndex;

            return JobBuilder<Args...>(this, this->vertexCount() - 1);
        };

        auto job(i32 index) -> Job<Args...>& {
            return this->getVertices()[index];
        }

        template <typename T>
        auto addResource(T value) -> Graph::Edge {
            auto& edge = this->addEdge();

            edge.index = _resources.size();
            _resources.emplace_back(value);

            return edge;
        }

        auto aliasResource(const ResourceIndex& index) -> ResourceIndex {
            auto& edge = this->addEdge();
            edge.index = index.index;
            return edge;
        }

        template <typename T>
        auto resource(const ResourceIndex& index) -> std::expected<T*, JobError> {
            if (index.index > _resources.size()) return std::unexpected(JobError::INDEX_OUT_OF_BOUNDS);

            auto& resource = _resources[index.index];
            if (!std::holds_alternative<T>(resource)) return std::unexpected(JobError::INVALID_RESOURCE);

            return &std::get<T>(resource);
        }


        void setRootJob(JobBuilder<Args...>& builder, bool topdown = false) {
            _rootJob = builder._index;
            _topdown = topdown;
        }

        auto dispatch() -> std::expected<bool, JobError> {
            if (_rootJob < 0) return std::unexpected(JobError::INVALID_JOB);

            _orderedJobs = maybe(this->sort(job(_rootJob), _topdown).transform_error(mapGraphErrorToJobError));

            trackSync();

            u32 currentQueue = 0;
            for (auto& job : _orderedJobs) {
                const auto queueIndex = job.queueIndex();
                auto& queue = _queues[queueIndex];

                std::vector<std::pair<thread::TimelineSemaphore*, u64>> waits = {};

                for (auto& wait : job._queueWaits) {

                    auto& waitingJob = _orderedJobs[wait];
                    auto waitingQueueIndex = waitingJob.queueIndex();

                    auto& waitingQueue = _queues[waitingQueueIndex];
                    waits.emplace_back(std::make_pair(&waitingQueue->timeline(), waitingQueue->timeline().localValue()));
                }

                queue->submit({
                    waits,
                    {std::make_pair(&queue->timeline(), queue->timeline().increment())},
                    [&]() -> i32 {
                        auto error = job.dispatch();
                        if (!error.has_value()) return static_cast<i32>(error.error());
                        return 0;
                    }
                });
            }

            return true;
        }

        void wait() {
            for (auto& queue : _queues) {
                queue->timeline().wait(queue->timeline().localValue());
            }
        }

    private:
        friend JobBuilder<Args...>;

        std::vector<std::unique_ptr<thread::Queue>> _queues = {};

        std::vector<Job<Args...>> _orderedJobs = {};

        std::vector<Resource> _resources = {};
        i32 _rootJob = -1;
        bool _topdown = false;

        void trackSync() {
            for (auto& job : _orderedJobs) {
                job.calcAccess();
            }

            for (i32 jobIndex = _orderedJobs.size() - 1; jobIndex >= 0; jobIndex--) {
                auto& job = _orderedJobs[jobIndex];

                for (auto& access : job._accesses) {
                    const auto prevIndex = getPrevAccess(jobIndex, access);
                    if (prevIndex < 0) continue;
                    const auto& prevJob = _orderedJobs[prevIndex];
                    if (prevJob.queueIndex() != job.queueIndex()) {
                        job._queueWaits.emplace_back(prevJob.queueIndex());
                    }
                }
            }
        }

        auto getPrevAccess(const i32 startIndex, const i32 resourceIndex) -> i32 {
            for (i32 jobIndex = startIndex - 1; jobIndex >= 0; jobIndex--) {
                const auto& job = _orderedJobs[jobIndex];

                for (auto& access : job._accesses) {
                    if (access == resourceIndex)
                        return jobIndex;
                }
            }
            return -1;
        }

    };

}

template <typename... Args>
template <typename T>
auto ende::Job<Args...>::resource(const ResourceIndex& index) -> std::expected<T*, JobError> {
    return _system->template resource<T>(index);
}

template <typename... Args>
auto ende::JobBuilder<Args...>::job() -> ende::Job<Args...>&  {
    return _system->job(_index);
}

template <typename... Args>
auto ende::JobBuilder<Args...>::writes(Edge edge) -> JobBuilder<Args...>& {
    edge = _system->aliasResource(edge);
    job().outputs.emplace_back(edge);
    return *this;
}
