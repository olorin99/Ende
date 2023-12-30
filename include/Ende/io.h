#ifndef ENDE_IO_H
#define ENDE_IO_H

#include <Ende/filesystem/File.h>

namespace ende {

    auto stdin() -> fs::File&;

    auto stdout() -> fs::File&;

    auto stderr() -> fs::File&;

}

#endif //ENDE_IO_H
