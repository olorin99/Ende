module;

#include <stdio.h>

export module ende.io;

import ende.filesystem.File;

namespace ende {

export auto stdin() -> fs::File &;

export auto stdout() -> fs::File &;

export auto stderr() -> fs::File &;

} // namespace ende

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
