
#ifndef ENDE_PROFILE_H
#define ENDE_PROFILE_H

import ende.profile;

#ifdef ENDE_PROFILE
#define PROFILE ende::profile::Profile profile##__LINE__(PRETTY_FUNC, __LINE__, __FILE__);
#define PROFILE_NAMED(x) ende::profile::Profile profile##__LINE__((x), __LINE__, __FILE__);
#else
#define PROFILE
#define PROFILE_NAMED(x)
#endif

#endif // ENDE_PROFILE_H
