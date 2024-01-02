
#ifndef ENDE_PROFILE_H
#define ENDE_PROFILE_H

#include <Ende/time/time.h>
#include <chrono>

namespace ende::profile {

    struct ProfileData {
        const char* label;
        u32 line;
        const char* file;
        std::chrono::high_resolution_clock::time_point start;
        std::chrono::high_resolution_clock::time_point end;
    };

    void submit(ProfileData&& data);

    class Profile {
    public:

        explicit Profile(const char* label, u32 line, const char* file)
            : _label(label),
            _line(line),
            _file(file),
            _start(std::chrono::high_resolution_clock::now())
        {}

        ~Profile() {
            submit({_label, _line, _file, _start, std::chrono::high_resolution_clock::now()});
        }

    private:

        const char* _label;
        u32 _line;
        const char* _file;
        std::chrono::high_resolution_clock::time_point _start;

    };


}

#ifdef ENDE_PROFILE
#define PROFILE ende::profile::Profile profile##__LINE__(PRETTY_FUNC, __LINE__, __FILE__);
#define PROFILE_NAMED(x) ende::profile::Profile profile##__LINE__((x), __LINE__, __FILE__);
#else
#define PROFILE
#define PROFILE_NAMED(x)
#endif

#endif //ENDE_PROFILE_H
