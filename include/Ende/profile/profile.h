
#ifndef ENDE_PROFILE_H
#define ENDE_PROFILE_H

#include <Ende/time/time.h>

namespace ende::profile {

    struct ProfileData {
        const char* label;
        i32 line;
        const char* file;
        time::Duration time;
    };

    void submit(ProfileData&& data);

    class Profile {
    public:

        explicit Profile(const char* label, i32 line, const char* file)
            : _label(label),
            _line(line),
            _file(file),
            _start(time::Instant::now())
        {}

        ~Profile() {
            submit({_label, _line, _file, _start.elapsed()});
        }

    private:

        const char* _label;
        i32 _line;
        const char* _file;
        time::Instant _start;

    };


}

#define PROFILE ende::profile::Profile profile##__LINE__(PRETTY_FUNC, __LINE__, __FILE__);
#define PROFILE_NAMED(x) ende::profile::Profile profile##__LINE__((x), __LINE__, __FILE__);

#endif //ENDE_PROFILE_H
