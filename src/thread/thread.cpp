#include "Ende/thread/thread.h"
#include <Ende/sys/system.h>

void ende::thread::sleep(const time::Duration &duration) {
    sys::sleep(duration.spec());
}