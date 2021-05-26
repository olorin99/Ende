
set(ENDE_ROOT
        "${ENDE_ROOT}"
        CACHE
        PATH
        "~/Projects/libraries/Ende")

find_library(ENDE_LIBRARY
        NAMES
        ende
        PATHS
        "${ENDE_ROOT}"
        PATH_SUFFIXES
        lib)

find_package(ENDE_INCLUDE_DIR
        NAMES
        Ende/ende.h
        PATHS
        "${ENDE_ROOT}"
        PATH_SUFFIXES
        include)