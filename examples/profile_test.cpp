
#include <Ende/profile/profile.h>
#include <Ende/profile/ProfileManager.h>

int main() {

    u32 a = 0;

    for (int i = 0; i < 10000; i++) {
        PROFILE_NAMED("hello");

        if (i % 100 == 0)
            ende::profile::ProfileManager::frame();

        a = a + i;
        printf("%d\n", a);
    }

    u32 current = ende::profile::ProfileManager::getCurrentFrame();

    for (u32 i = 0; i < 60; i++) {
        u32 frame = (current + i) % 60;
        auto f = ende::profile::ProfileManager::getFrameData(frame);
        printf("%d\n", frame);
        for (auto& data : f) {
            printf("%d, %s: %ld\n", frame, data.label, std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::time_point_cast<std::chrono::nanoseconds>(data.end).time_since_epoch()).count() - std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::time_point_cast<std::chrono::nanoseconds>(data.start).time_since_epoch()).count());
        }
    }


    return 0;
}