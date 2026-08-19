module;

#include <Ende/maybe.h>
#include <algorithm>
#include <expected>
#include <functional>
#include <span>
#include <variant>
#include <vector>

export module ende.graph;

import ende.util;

namespace ende::graph {

export struct Edge {
    i32 id = -1;
    u32 weight = 1;
};

export template <typename... Args>
struct VariantEdge : public Edge {
    std::variant<Args...> value = {};
};

export template <typename T>
concept IsEdge = requires(T t) {
    { std::is_base_of_v<Edge, T> };
};

export enum class Error {
    NONE,
    IS_CYCLICAL,
    INVALID_VERTEX,
    INVALID_EDGE,
    EDGE_TYPE_DOESNT_MATCH,
    INDEX_OUT_OF_BOUNDS,
};

export template <IsEdge E>
struct Vertex {
    using Edge = E;

    u32 id = {};
    std::vector<Edge> inputs = {};
    std::vector<Edge> outputs = {};

    constexpr auto input(u32 index = 0) -> std::expected<E, Error> {
        if (index >= inputs.size()) {
            return std::unexpected(Error::INDEX_OUT_OF_BOUNDS);
        }
        return inputs[index];
    }

    constexpr auto output(u32 index = 0) -> std::expected<E, Error> {
        if (index >= outputs.size()) {
            return std::unexpected(Error::INDEX_OUT_OF_BOUNDS);
        }
        return outputs[index];
    }
};

export template <typename V = Vertex<Edge>>
auto buildAdjacencies(const std::span<const V> vertices, const u32 edgeCount, const bool topdown = false) -> std::expected<std::vector<std::vector<V>>, Error> {
    std::vector<std::vector<V>> adjacencies = {};
    adjacencies.resize(edgeCount);
    for (auto &vertex : vertices) {
        if (topdown) {
            for (auto &input : vertex.inputs) {
                auto edgeId = input.id;
                adjacencies[edgeId].push_back(vertex);
            }
        } else {
            for (auto &output : vertex.outputs) {
                auto edgeId = output.id;
                adjacencies[edgeId].push_back(vertex);
            }
        }
    }
    return adjacencies;
}

export template <typename V = Vertex<Edge>>
auto topologicalSort(const std::span<const V> vertices, std::span<const typename V::Edge> rootEdges, const u32 edgeCount, const bool topdown = false) -> std::expected<std::vector<V>, Error> {
    auto adjacencies = maybe(buildAdjacencies(vertices, edgeCount, topdown));

    std::vector<bool> visited = {};
    visited.resize(vertices.size());
    std::vector<bool> onStack = {};
    onStack.resize(vertices.size());

    std::vector<V> topological = {};

    std::function<std::expected<bool, Error>(V)> dfs = [&](V vertex) -> std::expected<bool, Error> {
        visited[vertex.id] = true;
        onStack[vertex.id] = true;
        for (auto &edge : (topdown ? vertex.outputs : vertex.inputs)) {
            auto edgeId = edge.id;
            for (auto &adjacent : adjacencies[edgeId]) {
                if (visited[adjacent.id] && onStack[adjacent.id])
                    return std::unexpected(Error::IS_CYCLICAL);
                if (!visited[adjacent.id]) {
                    if (!dfs(adjacent)) {
                        return std::unexpected(Error::IS_CYCLICAL);
                    }
                }
            }
        }
        topological.push_back(vertex);
        onStack[vertex.id] = false;
        return true;
    };

    for (auto &rootEdge : rootEdges) {
        auto rootId = rootEdge.id;
        for (auto &adjacent : adjacencies[rootId]) {
            if (!maybe(dfs(adjacent)))
                return std::unexpected(Error::IS_CYCLICAL);
        }
    }

    if (topdown) {
        std::ranges::reverse(topological);
    }

    return topological;
}

export template <typename V = Vertex<Edge>>
auto topologicalSort(const std::span<const V> vertices, const typename V::Edge rootEdge, const u32 edgeCount, const bool topdown = false) -> std::expected<std::vector<V>, Error> {
    const auto rootEdges = std::span(&rootEdge, 1);
    return topologicalSort(vertices, rootEdges, edgeCount, topdown);
}

export template <typename V = Vertex<Edge>>
auto topologicalSort(const std::span<const V> vertices, const V rootVertex, const u32 edgeCount, const bool topdown = false) -> std::expected<std::vector<V>, Error> {
    auto sorted = maybe(topologicalSort(vertices, topdown ? rootVertex.outputs : rootVertex.inputs, edgeCount, topdown));
    if (topdown) {
        sorted.insert(sorted.begin(), rootVertex);
    } else {
        sorted.emplace_back(rootVertex);
    }
    return sorted;
}

export template <typename V = Vertex<Edge>>
auto shortestPath(std::span<const V> vertices, u32 edgeCount) -> std::expected<std::vector<u32>, Error> {
    auto adjacencies = maybe(buildAdjacencies(vertices, edgeCount, true));

    auto maxVertexId = std::ranges::max_element(vertices.begin(), vertices.end(), [&](const auto &a, const auto &b) { return a.id < b.id; });

    std::vector<u32> distances = {};
    distances.resize(maxVertexId->id + 1);
    for (auto &distance : distances) {
        distance = std::numeric_limits<u32>::max();
    }
    distances[vertices.front().id] = 0;

    for (auto &vertex : vertices) {
        auto distance = distances[vertex.id];

        for (auto &edge : vertex.outputs) {
            auto edgeId = edge.id;
            auto weight = edge.weight;
            for (auto &adjacent : adjacencies[edgeId]) {

                auto &adjacentDistance = distances[adjacent.id];
                auto edgeDistance = distance + weight;

                if (adjacentDistance > edgeDistance) {
                    adjacentDistance = edgeDistance;
                }
            }
        }
    }

    const auto newEnd = std::ranges::remove(distances, std::numeric_limits<u32>::max()).begin();
    distances.erase(newEnd, distances.end());

    return distances;
}

export template <typename V = Vertex<Edge>>
auto longestPath(std::span<const V> vertices, u32 edgeCount) -> std::expected<std::vector<u32>, Error> {
    auto adjacencies = maybe(buildAdjacencies(vertices, edgeCount, true));

    auto maxVertexId = std::ranges::max_element(vertices.begin(), vertices.end(), [&](const auto &a, const auto &b) { return a.id < b.id; });

    std::vector<u32> distances = {};
    distances.resize(maxVertexId->id + 1);
    for (auto &distance : distances) {
        distance = std::numeric_limits<u32>::min();
    }
    distances[vertices.front().id] = 0;

    for (auto &vertex : vertices) {
        auto distance = distances[vertex.id];

        for (auto &edge : vertex.outputs) {
            auto edgeId = edge.id;
            auto weight = edge.weight;
            for (auto &adjacent : adjacencies[edgeId]) {

                auto &adjacentDistance = distances[adjacent.id];
                auto edgeDistance = distance + weight;

                if (adjacentDistance < edgeDistance) {
                    adjacentDistance = edgeDistance;
                }
            }
        }
    }

    const auto newEnd = std::ranges::remove(distances, std::numeric_limits<u32>::max()).begin();
    distances.erase(newEnd, distances.end());

    return distances;
}

export template <typename V = Vertex<Edge>>
class Graph {
  public:
    using Vertex = V;
    using Edge = Vertex::Edge;

    auto addVertex() -> Vertex & {
        auto vertex = Vertex();
        vertex.id = _vertexIndex++;
        _vertices.emplace_back(vertex);
        return _vertices.back();
    }

    auto addEdge(const u32 weight = 1) -> Edge & {
        auto edge = Edge();
        edge.id = _edgeIndex++;
        edge.weight = weight;
        _edges.emplace_back(edge);
        return _edges.back();
    }

    auto addEdge(Vertex &parent, Vertex &child, const u32 weight = 1) -> Edge & {
        auto edge = addEdge(weight);
        parent.outputs.emplace_back(edge);
        child.inputs.emplace_back(edge);
        return _edges.back();
    }

    auto getVertices() -> std::span<Vertex> {
        return std::span(_vertices.data(), _vertices.size());
    }

    auto getVertices() const -> std::span<const Vertex> {
        return std::span(_vertices.data(), _vertices.size());
    }

    auto getEdges() -> std::span<Edge> {
        return std::span(_edges.data(), _edges.size());
    }

    auto getEdges() const -> std::span<const Edge> {
        return std::span(_edges.data(), _edges.size());
    }

    auto vertexCount() const -> u32 { return _vertices.size(); }
    auto edgeCount() const -> u32 { return _edges.size(); }

    void reserveVertices(u32 count) {
        _vertices.reserve(count);
    }

    void reserveEdges(u32 count) {
        _edges.reserve(0);
    }

    void reset() {
        _vertices.clear();
        _edges.clear();
        _vertexIndex = 0;
        _edgeIndex = 0;
    }

    auto sort(Edge rootEdge, bool topdown = false) const -> std::expected<std::vector<Vertex>, Error> {
        return topologicalSort(getVertices(), rootEdge, edgeCount(), topdown);
    }

    auto sort(Vertex rootVertex, bool topdown = false) const -> std::expected<std::vector<Vertex>, Error> {
        return topologicalSort(getVertices(), rootVertex, edgeCount(), topdown);
    }

  private:
    std::vector<Vertex> _vertices = {};
    std::vector<Edge> _edges = {};

    u32 _vertexIndex = 0;
    u32 _edgeIndex = 0;
};
}
