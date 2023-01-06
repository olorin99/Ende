
#include <Ende/profile/profile.h>

int main() {

    u32 a = 0;

    for (int i = 0; i < 10000; i++) {
        PROFILE_NAMED("hello");

        a = a + i;
        printf("%d\n", a);
    }



    return 0;
}