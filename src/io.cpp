#include "Ende/io.h"

ende::fs::File &ende::stdin() {
    static fs::File file(::stdin, fs::in);
    return file;
}

ende::fs::File &ende::stdout() {
    static fs::File file(::stdout, fs::out);
    return file;
}

ende::fs::File &ende::stderr() {
    static fs::File file(::stderr, fs::out);
    return file;
}