
#include <Ende/filesystem/File.h>

int main(int argc, char* argv[]) {

    if (argc < 1) return -1;

    const auto path = std::filesystem::path(argv[argc - 1]);

    auto fileResult = ende::fs::File::open(path);
    if (!fileResult)
        return fileResult.error();

    fileResult.transform([](ende::fs::File& file) {
        auto contents = file.read();
        std::printf("%s", contents.c_str());
    });

    return 0;
}