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

        auto writes(Edge edge) -> JobBuilder<Args...>& {
            job().outputs.emplace_back(edge);
            return *this;
        }

        auto readWrites(Edge edge) -> JobBuilder<Args...>& {
            return reads(edge).writes(edge);
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

        auto addJob(const std::string& name) -> JobBuilder<Args...> {
            auto& vertex = this->addVertex();

            vertex._system = this;
            vertex._name = name;

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

            auto sorted = this->sort(job(_rootJob), _topdown);

            for (auto& job : *sorted) {
                maybe(job.dispatch());
            }

            return true;
        }

    private:
        friend JobBuilder<Args...>;

        std::vector<Resource> _resources = {};
        i32 _rootJob = -1;
        bool _topdown = false;

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
