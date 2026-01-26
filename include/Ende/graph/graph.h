#ifndef ENDE_GRAPH_H
#define ENDE_GRAPH_H

#include <expected>
#include <span>
#include <Ende/platform.h>
#include <vector>
#include <variant>
#include <functional>
#include <stdexcept>

namespace ende::graph {

    struct Edge {
        u32 id;
    };

    template <typename T>
    concept IsEdge = requires(T t)
    {
        { std::is_base_of_v<Edge, T> };
    };

    template <IsEdge... Args>
    struct Vertex {
        using Edge = std::variant<Args...>;

        u32 id = {};
        std::vector<Edge> inputs = {};
        std::vector<Edge> outputs = {};
    };


    enum class Error {
        IS_CYCLICAL,

        EDGE_TYPE_DOESNT_MATCH,
    };

    template <IsEdge Edge>
    struct EdgeHelper {

        template <std::size_t I = 0>
        static auto getId(Edge edge) -> std::expected<u32, Error> {
            if constexpr (I < std::variant_size_v<Edge>) {
                if (std::holds_alternative<std::variant_alternative_t<I, Edge>>(edge)) {
                    return std::get<I>(edge).id;
                }
                return getId<I + 1>(edge);
            }
            return std::unexpected(Error::EDGE_TYPE_DOESNT_MATCH);
        }

        template <std::size_t I = 0>
        static auto setId(Edge edge, u32 id) -> std::expected<Edge, Error> {
            if constexpr (I < std::variant_size_v<Edge>) {
                if (std::holds_alternative<std::variant_alternative_t<I, Edge>>(edge)) {
                    std::variant_alternative_t<I, Edge> e = std::get<I>(edge);
                    e.id = id;
                    return e;
                }
                return setId<I + 1>(edge, id);
            }
            return std::unexpected(Error::EDGE_TYPE_DOESNT_MATCH);
        }

        template <typename T, std::size_t I = 0>
        static constexpr auto holdsType() -> bool {
            if constexpr (I < std::variant_size_v<Edge>) {
                if constexpr (std::is_same_v<T, std::variant_alternative_t<0, Edge>>) {
                    return true;
                }
                return holdsType<T, I + 1>();
            }
            return false;
        }

    };

    template <IsEdge... Args>
    auto topologicalSort(std::span<Vertex<Args...>> vertices, std::span<typename Vertex<Args...>::Edge> rootEdges, u32 edgeCount) -> std::expected<std::vector<Vertex<Args...>>, Error> {
        std::vector<std::vector<Vertex<Args...>>> adjacencies = {};
        adjacencies.resize(edgeCount);
        for (auto& vertex : vertices) {
            for (auto& output : vertex.outputs) {
                auto edgeId = TRY(EdgeHelper<typename Vertex<Args...>::Edge>::getId(output));
                adjacencies[edgeId].push_back(vertex);
            }
        }

        std::vector<bool> visited = {};
        visited.resize(vertices.size());
        std::vector<bool> onStack = {};
        onStack.resize(vertices.size());

        std::vector<Vertex<Args...>> topological = {};

        std::function<std::expected<bool, Error>(Vertex<Args...>)> dfs = [&] (Vertex<Args...> vertex) -> std::expected<bool, Error> {
            visited[vertex.id] = true;
            onStack[vertex.id] = true;
            for (auto& input : vertex.inputs) {
                auto edgeId = TRY(EdgeHelper<typename Vertex<Args...>::Edge>::getId(input));
                for (auto& adjacent : adjacencies[edgeId]) {
                    if (visited[adjacent.id] && onStack[adjacent.id])
                        return false;
                    if (!visited[adjacent.id]) {
                        if (!dfs(adjacent)) {
                            return false;
                        }
                    }
                }
            }
            topological.push_back(vertex);
            onStack[vertex.id] = false;
            return true;
        };

        for (auto& rootEdge : rootEdges) {
            auto rootId = TRY(EdgeHelper<typename Vertex<Args...>::Edge>::getId(rootEdge));
            for (auto& adjacent : adjacencies[rootId]) {
                if (!TRY(dfs(adjacent))) return std::unexpected(Error::IS_CYCLICAL);
            }
        }

        return topological;
    }

    template <IsEdge... Args>
    auto topologicalSort(std::span<Vertex<Args...>> vertices, typename Vertex<Args...>::Edge rootEdge, u32 edgeCount) -> std::expected<std::vector<Vertex<Args...>>, Error> {
        auto rootEdges = std::span(&rootEdge, 1);
        return topologicalSort(vertices, rootEdges, edgeCount);
    }

    template <IsEdge... Args>
    auto topologicalSort(std::span<Vertex<Args...>> vertices, Vertex<Args...> rootVertex, u32 edgeCount) -> std::expected<std::vector<Vertex<Args...>>, Error> {
        return topologicalSort(vertices, rootVertex.inputs, edgeCount);
    }

    template <typename V = Vertex<Edge>>
    class Graph {
    public:

        using Vertex = V;
        using Edge = Vertex::Edge;


        auto addVertex() -> Vertex& {
            auto vertex = Vertex();
            vertex.id = _vertexIndex++;
            _vertices.emplace_back(vertex);
            return _vertices.back();
        }

        template <IsEdge E = Edge>
        auto addEdge() -> Edge {
            auto edge = EdgeHelper<Edge>::setId(E(), _edgeIndex++);
            _edges.emplace_back(*edge);
            return _edges.back();
        }

        template <IsEdge E = Edge>
        auto addEdge(Vertex& parent, Vertex& child) -> Edge {
            auto edge = addEdge<E>();
            parent.outputs.emplace_back(edge);
            child.inputs.emplace_back(edge);
            return edge;
        }

        auto getVertices() -> std::span<Vertex> {
            return std::span(_vertices.data(), _vertices.size());
        }

        auto getEdges() -> std::span<Edge> {
            return std::span(_edges.data(), _edges.size());
        }

        auto vertexCount() -> u32 { return _vertices.size(); }
        auto edgeCount() -> u32 { return _edges.size(); }

        void reserveVertices(u32 count) {
            _vertices.reserve(count);
        }

        void reserveEdges(u32 count) {
            _edges.reserve(0);
        }



        auto sort(Edge rootEdge) -> std::expected<std::vector<Vertex>, Error> {
            return topologicalSort(getVertices(), rootEdge, edgeCount());
        }

        auto sort(Vertex rootVertex) -> std::expected<std::vector<Vertex>, Error> {
            return topologicalSort(getVertices(), rootVertex, edgeCount());
        }

    private:

        std::vector<Vertex> _vertices = {};
        std::vector<Edge> _edges = {};

        u32 _vertexIndex = 0;
        u32 _edgeIndex = 0;

    };
}

#endif //ENDE_GRAPH_H