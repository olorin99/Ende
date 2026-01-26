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

    template <IsEdge... Args>
    struct EdgeHelper {

        template <std::size_t I = 0>
        static auto getId(typename Vertex<Args...>::Edge edge) -> std::expected<u32, Error> {
            if constexpr (I < std::variant_size_v<typename Vertex<Args...>::Edge>) {
                if (std::holds_alternative<std::variant_alternative_t<I, typename Vertex<Args...>::Edge>>(edge)) {
                    return std::get<I>(edge).id;
                }
                return getId<I + 1>(edge);
            }
            return std::unexpected(Error::EDGE_TYPE_DOESNT_MATCH);
        }

    };



    template <IsEdge... Args>
    auto topologicalSort(std::span<Vertex<Args...>> vertices, typename Vertex<Args...>::Edge rootEdge, u32 edgeCount) -> std::expected<std::vector<Vertex<Args...>>, Error>  {
        std::vector<std::vector<Vertex<Args...>>> adjacencies = {};
        adjacencies.resize(edgeCount);
        for (auto& vertex : vertices) {
            for (auto& output : vertex.outputs) {
                auto edgeId = TRY(EdgeHelper<Args...>::getId(output));
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
                auto edgeId = TRY(EdgeHelper<Args...>::getId(input));
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

        auto rootId = TRY(EdgeHelper<Args...>::getId(rootEdge));
        for (auto& adjacent : adjacencies[rootId]) {
            if (!TRY(dfs(adjacent))) return std::unexpected(Error::IS_CYCLICAL);
        }

        return topological;
    }
}

#endif //ENDE_GRAPH_H