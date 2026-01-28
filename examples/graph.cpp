
#include <cstdio>
#include <ranges>
#include <string>
#include <Ende/graph/graph.h>

using namespace ende::graph;

struct BufferEdge : public Edge {};
struct ImageEdge : public Edge {};


struct RenderVertex : public Vertex<ImageEdge, BufferEdge> {
    std::string name;
};

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


    auto vertices = std::to_array({
        vertex0, vertex1, vertex2
    });

    const auto graphSpan = std::span(vertices.data(), vertices.size());

    auto topological = TRY_MAIN(topologicalSort(graphSpan, edge4, 5));

    for (const auto& vertex : topological) {
        printf("%d, ", vertex.id);
    }
    printf("\n");




    auto graph = Graph<Vertex<ImageEdge, BufferEdge>>();
    graph.reserveVertices(3);

    auto& v0 = graph.addVertex();
    auto& v1 = graph.addVertex();
    auto& v2 = graph.addVertex();

    auto e0 = graph.addEdge<ImageEdge>(v0, v1);
    graph.addEdge<BufferEdge>(v0, v2);
    graph.addEdge(v1, v2);

    // auto e0 = graph.addEdge();
    // auto e1 = graph.addEdge();
    // auto e2 = graph.addEdge();
    // auto e3 = graph.addEdge();
    auto e4 = graph.addEdge();

    // v0.outputs = { e0, e1, e2 };

    // v1.inputs = { e0, e1 };
    // v1.outputs = { e3 };

    // v2.inputs = { e2, e3 };
    v2.outputs = { e4 };

    // auto t = TRY_MAIN(topologicalSort(graph.getVertices(), e4, graph.edgeCount()));
    auto t = TRY_MAIN(graph.sort(e4));

    for (const auto& vertex : t) {
        printf("%d, ", vertex.id);
    }
    printf("\n");



    {
        auto g = Graph<RenderVertex>();
        g.reserveVertices(30);

        auto& top = g.addVertex();
        top.name = "Top";

        auto geometryPass = [&] (bool meshShader) -> Graph<RenderVertex>::Vertex& {
            auto& clearMeshletInstances = g.addVertex();
            clearMeshletInstances.name = "ClearMeshletInstances";
            auto& cullMeshes = g.addVertex();
            cullMeshes.name = "CullMeshes";
            auto& writeCullMeshletsCommand = g.addVertex();
            writeCullMeshletsCommand.name = "WriteCullMeshletsCommand";
            auto& cullMeshlets = g.addVertex();
            cullMeshlets.name = "CullMeshlets";
            auto& writeDrawCommand = g.addVertex();
            writeDrawCommand.name = "WriteDrawCommand";

            auto& drawMeshlets = g.addVertex();
            drawMeshlets.name = "DrawMeshlets";
            auto& writePrimitives = g.addVertex();
            writePrimitives.name = "WritePrimitives";

            g.addEdge(top, clearMeshletInstances);

            g.addEdge(clearMeshletInstances, cullMeshes);
            g.addEdge(cullMeshes, writeCullMeshletsCommand);
            g.addEdge(writeCullMeshletsCommand, cullMeshlets);
            g.addEdge(cullMeshlets, writeDrawCommand);
            g.addEdge(clearMeshletInstances, cullMeshlets);

            if (!meshShader) {
                g.addEdge(writeDrawCommand, writePrimitives);
                g.addEdge(writePrimitives, drawMeshlets);
            } else {
                g.addEdge(writeDrawCommand, drawMeshlets);
            }
            return drawMeshlets;
        };

        auto& draw0 = geometryPass(false);

        auto& draw1 = geometryPass(true);

        auto& compositePass = g.addVertex();
        compositePass.name = "CompositePass";
        g.addEdge(draw0, compositePass);
        g.addEdge(draw1, compositePass);


        // auto root = g.addEdge();
        // drawMeshlets.outputs = { root };

        auto sortedBottomUp = TRY_MAIN(g.sort(compositePass));
        auto sortedTopDown = TRY_MAIN(g.sort(top, true));

        for (const auto& vertex : sortedBottomUp) {
            printf("%d: %s, ", vertex.id, vertex.name.c_str());
        }
        printf("\n");
        for (const auto& vertex : sortedTopDown) {
            printf("%d: %s, ", vertex.id, vertex.name.c_str());
        }
        printf("\n");

        auto bottomUpSpan = std::span(sortedBottomUp.data(), sortedBottomUp.size());
        auto shortestPathDistances = TRY_MAIN(shortestPath(bottomUpSpan, g.edgeCount()));
        for (const auto& [distance, vertex] : std::views::zip(shortestPathDistances, sortedBottomUp)) {
            printf("(%d, %d): %s, ", vertex.id, distance, vertex.name.c_str());
        }
        printf("\n");


        auto longestPathDistances = TRY_MAIN(longestPath(bottomUpSpan, g.edgeCount()));
        for (const auto& [distance, vertex] : std::views::zip(longestPathDistances, sortedBottomUp)) {
            printf("(%d, %d): %s, ", vertex.id, distance, vertex.name.c_str());
        }
        printf("\n");

    }

    return 0;
}