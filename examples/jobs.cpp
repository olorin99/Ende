#include <Ende/maybe.h>
#include <print>
#include <variant>

import ende;

int main() {

    auto system = ende::JobSystem();

    auto a = system.addResource(100);
    auto b = system.addResource(0);
    auto c = system.addResource(-100);


    system.addJob("A")
        .depends(ende::Read(a), ende::Write(b))
        .executes([](ende::Job& job) {
            std::printf("%s\n", job.name().data());
            std::printf("inputs:\n");
            for (auto& resource : job.inputs) {
                std::printf("\t%d: %d\n", job.index(resource)->index, **job.resource<i32>(resource));
            }
            std::printf("outputs:\n");
            for (auto& resource : job.outputs) {
                std::printf("\t%d: %d\n", job.index(resource)->index, **job.resource<i32>(resource));
            }
        });

    system.addJob("B")
        .depends(ende::Read(b), ende::Write(c))
        .executes([=](ende::Job& job) -> std::expected<bool, ende::JobError> {
            std::printf("%s\n", job.name().data());
            std::printf("inputs:\n");
            for (auto& resource : job.inputs) {
                std::printf("\t%d: %d\n", job.index(resource)->index, **job.resource<i32>(resource));
            }
            std::printf("outputs:\n");
            for (auto& resource : job.outputs) {
                std::printf("\t%d: %d\n", job.index(resource)->index, **job.resource<i32>(resource));
            }

            auto bb = maybe(job.resource<i32>(b));
            auto cc = maybe(job.resource<i32>(c));
            *cc = *bb;
            return true;
        });

    auto jobC = system.addJob("C")
        .depends(ende::Read(c))
        .executes([](ende::Job& job) {
            std::printf("%s\n", job.name().data());
            std::printf("inputs:\n");
            for (auto& resource : job.inputs) {
                std::printf("\t%d: %d\n", job.index(resource)->index, **job.resource<i32>(resource));
            }
            std::printf("outputs:\n");
            for (auto& resource : job.outputs) {
                std::printf("\t%d: %d\n", job.index(resource)->index, **job.resource<i32>(resource));
            }
        });


    system.setRootJob(jobC);
    maybe_conv(i32, system.dispatch());

    return 0;
}
