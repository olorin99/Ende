
#include <cstdio>
#include <Ende/graph/graph.h>

using namespace ende::graph;

struct BufferEdge : public Edge {};

int main() {

    BufferEdge edge0;// = { .id = 0 };
    edge0.id = 0;
    Edge edge1 = { .id = 1 };
    Edge edge2 = { .id = 2 };
    Edge edge3 = { .id = 3 };
    Edge edge4 = { .id = 4 };

    Vertex<Edge, BufferEdge> vertex0 = { .id = 0 };
    Vertex<Edge, BufferEdge> vertex1 = { .id = 1 };
    Vertex<Edge, BufferEdge> vertex2 = { .id = 2 };

    vertex0.outputs = {
        edge0, edge1, edge2
    };

    vertex1.inputs = { edge0, edge1 };
    vertex1.outputs = { edge3 };

    vertex2.inputs = { edge2, edge3 };
    vertex2.outputs = { edge4 };


    auto graph = std::to_array({
        vertex0, vertex1, vertex2
    });

    const auto graphSpan = std::span(graph.data(), graph.size());

    auto topological = TRY_MAIN(topologicalSort(graphSpan, edge4, 5));

    for (const auto& vertex : topological) {
        printf("%d, ", vertex.id);
    }


    return 0;
}