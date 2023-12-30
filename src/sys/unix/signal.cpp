//
// Created by cstro29 on 26/5/21.
//

#include "Ende/sys/signal.h"
#include <vector>
#include <csignal>

std::vector<struct sigaction> oldHandlers;

void ende::sys::setSignal(i32 sig, void (*handler)(i32)) {
    if (!handler) return;
    if (oldHandlers.size() <= sig)
        oldHandlers.resize(sig + 1);
    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(sig, &sa, &oldHandlers[sig]);
}

void ende::sys::resetSignal(i32 sig) {
    if (oldHandlers.size() <= sig)
        return;
    sigaction(sig, &oldHandlers[sig], nullptr);
}