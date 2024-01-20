#include <Ende/sys/notify.h>
#include <sys/inotify.h>

int main() {

    auto fd = ende::sys::notify::init(IN_NONBLOCK);

    auto watch = ende::sys::notify::addWatch(fd, "~", IN_ACCESS);




    return 0;
}