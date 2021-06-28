//
// Created by cstro29 on 3/6/21.
//

#include "Ende/thread/thread.h"
#include <Ende/sys/system.h>

void ende::thread::sleep(const time::Duration &duration) {
    sys::sleep(duration.spec());
}