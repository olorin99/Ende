#ifndef ENDE_IO_H
#define ENDE_IO_H

#include <Ende/filesystem/File.h>

namespace ende {

    fs::File& stdin();

    fs::File& stdout();

    fs::File& stderr();

}

#endif //ENDE_IO_H
