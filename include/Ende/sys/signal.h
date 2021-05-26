//
// Created by cstro29 on 26/5/21.
//

#ifndef ENDE_SIGNAL_H
#define ENDE_SIGNAL_H

#include <Ende/platform.h>

namespace ende::sys {

    void setSignal(i32 sig, void(*handler)(i32));

    void resetSignal(i32 sig);

}

#endif //ENDE_SIGNAL_H
