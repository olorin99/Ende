#include <sys/inotify.h>

import ende;

int main() {

    auto fd = ende::sys::notify::init(IN_NONBLOCK);

    auto watch = ende::sys::notify::addWatch(fd, "~", ende::sys::notify::Mask::ACCESS);

    return 0;
}
