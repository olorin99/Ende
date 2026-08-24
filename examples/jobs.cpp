#include <Ende/maybe.h>
#include <print>
#include <utility>
#include <variant>
#include <cmath>

import ende;

int main() {

    auto system = ende::JobSystem<i32, std::string>();

    auto a = system.addResource(100);
    auto b = system.addResource(0);
    auto c = system.addResource(-100);


    auto jobA = system.addJob("A")
        .depends(ende::Read(a), ende::Write(b))
        .executes([](ende::Job<i32, std::string>& job) {
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
        .executes([=](ende::Job<i32, std::string>& job) -> std::expected<bool, ende::JobError> {
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
        .executes([](ende::Job<i32, std::string>& job) {
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

    {
        // let a = 5
        // let b = 100
        // let c = 193^2
        // let d = 0.102341
        // let x = ((a^2) * ((b + 12) * c)) / d

        const f32 ax = std::pow(5, 2);
        const f32 bx = (100 + 12) * std::pow(193, 2);
        const f32 cx = ax * bx;
        const f32 dx = cx / 0.102341f;

        const f32 solution = (std::pow(5, 2) * ((100 + 12) * std::pow(193, 2))) / 0.102341f;

        auto equation = ende::JobSystem<f32>(3);

        auto a = equation.addResource<f32>(5);
        auto b = equation.addResource<f32>(100);
        auto c = equation.addResource<f32>(std::pow(193, 2));
        auto d = equation.addResource<f32>(0.102341);

        a = maybe_conv(i32, equation.addJob("A", 0, 0)
            .depends(ende::Write(a))
            .executes([=](ende::Job<f32>& job) -> std::expected<bool, ende::JobError> {
                auto aa = maybe(job.resource<f32>(a));
                *aa = std::pow(*aa, 2);
                return true;
            }).output());

        b = maybe_conv(i32, equation.addJob("B", 0, 1)
            .depends(ende::Write(b), ende::Read(c))
            .executes([=](ende::Job<f32>& job) -> std::expected<bool, ende::JobError> {
                auto bb = maybe(job.resource<f32>(b));
                auto cc = maybe(job.resource<f32>(c));
                *bb = (*bb + 12) * *cc;
                return true;
            }).output());

        c = maybe_conv(i32, equation.addJob("C", 0, 2)
            .depends(ende::Read(a), ende::Read(b), ende::Write(c))
            .executes([=](ende::Job<f32>& job) -> std::expected<bool, ende::JobError> {
               auto aa = maybe(job.resource<f32>(a));
               auto bb = maybe(job.resource<f32>(b));
               auto cc = maybe(job.resource<f32>(c));
               *cc = *aa * *bb;
               return true;
            }).output());

        auto root = equation.addJob("D", 0, 0)
            .depends(ende::Read(c), ende::Write(d))
            .executes([=](ende::Job<f32>& job) -> std::expected<bool, ende::JobError> {
                auto cc = maybe(job.resource<f32>(c));
                auto dd = maybe(job.resource<f32>(d));
                *dd = *cc / *dd;
                return true;
            });

        equation.setRootJob(root);
        maybe_conv(i32, equation.dispatch());
        equation.wait();

        auto result = maybe_conv(i32, equation.resource<f32>(d));
        std::printf("%f : %f\n", *result, solution);
    }

    return 0;
}
