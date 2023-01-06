
#ifndef ENDE_PROFILE_H
#define ENDE_PROFILE_H

#include <Ende/time/time.h>

namespace ende::profile {

    struct ProfileData {
        const char* label;
        u32 line;
        const char* file;
        time::Instant start;
        time::Instant end;
    };

    void submit(ProfileData&& data);

    class Profile {
    public:

        explicit Profile(const char* label, u32 line, const char* file)
            : _label(label),
            _line(line),
            _file(file),
            _start(time::Instant::now())
        {}

        ~Profile() {
            submit({_label, _line, _file, _start, time::Instant::now()});
        }

    private:

        const char* _label;
        u32 _line;
        const char* _file;
        time::Instant _start;

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
