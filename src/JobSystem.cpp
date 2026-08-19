module;

#include <Ende/maybe.h>
#include <functional>
#include <expected>
#include <string>
#include <variant>

export module ende.JobSystem;

import ende.util;
import ende.graph;

namespace ende {

    export class JobSystem;
    export class JobBuilder;

    export enum class JobError {
        NONE,
        IS_CYCLICAL,
        INVALID_JOB,
        INVALID_RESOURCE,
        RESOURCE_TYPE_DOESNT_MATCH,
        INDEX_OUT_OF_BOUNDS,
    };

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


    export class Job : public graph::Vertex<ResourceIndex> {
    public:


        auto name() const -> std::string_view { return _name; }

        auto index(const Edge& edge) -> std::expected<ResourceIndex, JobError> {
            for (auto& input : inputs) {
                if (std::get<ResourceIndex>(input).index == std::get<ResourceIndex>(edge).index) {
                    return std::get<ResourceIndex>(input);
                }
            }
            for (auto& output : outputs) {
                if (std::get<ResourceIndex>(output).index == std::get<ResourceIndex>(edge).index) {
                    return std::get<ResourceIndex>(output);
                }
            }
            return std::unexpected(JobError::INVALID_RESOURCE);
        }

        template <typename T>
        auto resource(const Edge& index) -> std::expected<T*, JobError>;

        template <typename T>
        auto resource(const ResourceIndex& index) -> std::expected<T*, JobError>;

        auto dispatch() -> std::expected<bool, JobError> {
            return _callback(*this);
        }

    private:
        friend JobSystem;
        friend JobBuilder;

        JobSystem* _system = nullptr;

        std::string _name = {};
        std::function<std::expected<bool, JobError>(Job&)> _callback = {};

    };

    export class JobBuilder {
    public:

        using Graph = graph::Graph<Job>;
        using Edge = Graph::Edge;

        auto job() -> Job&;


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


        auto reads(Edge edge) -> JobBuilder& {
            job().inputs.emplace_back(edge);
            return *this;
        }

        auto writes(Edge edge) -> JobBuilder& {
            job().outputs.emplace_back(edge);
            return *this;
        }

        auto readWrites(Edge edge) -> JobBuilder& {
            return reads(edge).writes(edge);
        }



        template <typename... Args>
        auto depends(const Args&... args) -> JobBuilder& {
            unpack(args...);
            return *this;
        }


        template <typename F>
        auto executes(F callback) -> JobBuilder& {
            return executes(util::makeFunction(callback));
        }

        auto executes(const std::function<void(Job&)>& callback) -> JobBuilder& {
            job()._callback = [callback](Job& job) -> std::expected<bool, JobError> {
                try {
                    callback(job);
                } catch (...) {
                    return std::unexpected(JobError::INVALID_JOB);
                }
                return true;
            };
            return *this;
        }

        auto executes(const std::function<std::expected<bool, JobError>(Job&)>& callback) -> JobBuilder& {
            job()._callback = [callback](Job& job) -> std::expected<bool, JobError> {
                try {
                    return callback(job);
                } catch (...) {
                    return std::unexpected(JobError::INVALID_JOB);
                }
                return true;
            };
            return *this;
        }

        template <typename... Args>
        auto executes(const std::function<void(Job&, Args...)>& callback) -> JobBuilder& {
            job()._callback = [callback](Job& job) -> std::expected<bool, JobError> {
                try {
                    callback(job);
                } catch (...) {
                    return std::unexpected(JobError::INVALID_JOB);
                }
                return true;
            };
            return *this;
        }

    private:
        friend JobSystem;

        JobSystem* _system = nullptr;
        i32 _index = -1;

        JobBuilder(JobSystem* system, i32 index) : _system(system), _index(index) {}

    };

    class JobSystem : public graph::Graph<Job> {
    public:

        using Graph = graph::Graph<Job>;
        using Resource = std::variant<i32>;

        auto addJob(const std::string& name) -> JobBuilder {
            auto& vertex = addVertex();

            vertex._system = this;
            vertex._name = name;

            return JobBuilder(this, vertexCount() - 1);
        };

        auto job(i32 index) -> Job& {
            return getVertices()[index];
        }

        template <typename T>
        auto addResource(T value) -> Graph::Edge {
            auto& edge = addEdge();

            std::get<ResourceIndex>(edge).index = _resources.size();
            _resources.emplace_back(value);

            return edge;
        }

        template <typename T>
        auto resource(const ResourceIndex& index) -> std::expected<T*, JobError> {
            if (index.index > _resources.size()) return std::unexpected(JobError::INDEX_OUT_OF_BOUNDS);

            auto& resource = _resources[index.index];
            if (!std::holds_alternative<T>(resource)) return std::unexpected(JobError::INVALID_RESOURCE);

            return &std::get<T>(resource);
        }


        void setRootJob(JobBuilder& builder, bool topdown = false) {
            _rootJob = builder._index;
            _topdown = topdown;
        }

        auto dispatch() -> std::expected<bool, JobError> {
            if (_rootJob < 0) return std::unexpected(JobError::INVALID_JOB);

            auto sorted = sort(job(_rootJob), _topdown);

            for (auto& job : *sorted) {
                maybe(job.dispatch());
            }

            return true;
        }

    private:
        friend JobBuilder;

        std::vector<Resource> _resources = {};
        i32 _rootJob = -1;
        bool _topdown = false;

    };

}


template <typename T>
auto ende::Job::resource(const Edge& index) -> std::expected<T*, JobError> {
    return resource<T>(std::get<ResourceIndex>(index));
}

template <typename T>
auto ende::Job::resource(const ResourceIndex& index) -> std::expected<T*, JobError> {
    return _system->resource<T>(index);
}

auto ende::JobBuilder::job() -> ende::Job&  {
    return _system->job(_index);
}
