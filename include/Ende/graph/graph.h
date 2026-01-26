#ifndef ENDE_GRAPH_H
#define ENDE_GRAPH_H

#include <algorithm>
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

    template <typename V = Vertex<Edge>>
    auto topologicalSort(std::span<V> vertices, std::span<typename V::Edge> rootEdges, u32 edgeCount, bool topdown = false) -> std::expected<std::vector<V>, Error> {
        std::vector<std::vector<V>> adjacencies = {};
        adjacencies.resize(edgeCount);
        for (auto& vertex : vertices) {
            if (topdown) {
                for (auto& input : vertex.inputs) {
                    auto edgeId = TRY(EdgeHelper<typename V::Edge>::getId(input));
                    adjacencies[edgeId].push_back(vertex);
                }
            } else {
                for (auto& output : vertex.outputs) {
                    auto edgeId = TRY(EdgeHelper<typename V::Edge>::getId(output));
                    adjacencies[edgeId].push_back(vertex);
                }
            }
        }

        std::vector<bool> visited = {};
        visited.resize(vertices.size());
        std::vector<bool> onStack = {};
        onStack.resize(vertices.size());

        std::vector<V> topological = {};

        std::function<std::expected<bool, Error>(V)> dfs = [&] (V vertex) -> std::expected<bool, Error> {
            visited[vertex.id] = true;
            onStack[vertex.id] = true;
            for (auto& edge : (topdown ? vertex.outputs : vertex.inputs)) {
                auto edgeId = TRY(EdgeHelper<typename V::Edge>::getId(edge));
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
            auto rootId = TRY(EdgeHelper<typename V::Edge>::getId(rootEdge));
            for (auto& adjacent : adjacencies[rootId]) {
                if (!TRY(dfs(adjacent))) return std::unexpected(Error::IS_CYCLICAL);
            }
        }

        if (topdown) {
            std::ranges::reverse(topological);
        }

        return topological;
    }

    template <typename V = Vertex<Edge>>
    auto topologicalSort(std::span<V> vertices, typename V::Edge rootEdge, u32 edgeCount, bool topdown = false) -> std::expected<std::vector<V>, Error> {
        auto rootEdges = std::span(&rootEdge, 1);
        return topologicalSort(vertices, rootEdges, edgeCount, topdown);
    }

    template <typename V = Vertex<Edge>>
    auto topologicalSort(std::span<V> vertices, V rootVertex, u32 edgeCount, bool topdown = false) -> std::expected<std::vector<V>, Error> {
        auto sorted = TRY(topologicalSort(vertices, topdown ? rootVertex.outputs : rootVertex.inputs, edgeCount, topdown));
        if (topdown) {
            sorted.insert(sorted.begin(), rootVertex);
        } else {
            sorted.emplace_back(rootVertex);
        }
        return sorted;
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



        auto sort(Edge rootEdge, bool topdown = false) -> std::expected<std::vector<Vertex>, Error> {
            return topologicalSort(getVertices(), rootEdge, edgeCount(), topdown);
        }

        auto sort(Vertex rootVertex, bool topdown = false) -> std::expected<std::vector<Vertex>, Error> {
            return topologicalSort(getVertices(), rootVertex, edgeCount(), topdown);
        }

    private:

        std::vector<Vertex> _vertices = {};
        std::vector<Edge> _edges = {};

        u32 _vertexIndex = 0;
        u32 _edgeIndex = 0;

    };
}

#endif //ENDE_GRAPH_H