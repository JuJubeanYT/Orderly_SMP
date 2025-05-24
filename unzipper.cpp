#include <filesystem>
#include "unzipper.hpp"
#include <cstdlib>
#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>

void copyOverrides(const std::string& src, const std::string& dest) {
    for (const auto& entry : std::filesystem::recursive_directory_iterator(src)) {
        if (!entry.is_regular_file()) continue;
        auto rel = std::filesystem::relative(entry.path(), src);
        auto outPath = std::filesystem::path(dest) / rel;
        std::filesystem::create_directories(outPath.parent_path());
        std::filesystem::copy_file(entry.path(), outPath, std::filesystem::copy_options::overwrite_existing);
    }
}


bool unzip_file(const std::string& zip_path, const std::string& output_dir) {
#ifdef _WIN32
    std::string cmd = "powershell -Command Expand-Archive -Path \"" + zip_path + "\" -DestinationPath \"" + output_dir + "\" -Force";
#elif __APPLE__ || __linux__
    std::string cmd = "unzip -o \"" + zip_path + "\" -d \"" + output_dir + "\"";
#else
    #error "Unsupported platform"
#endif

    int ret = std::system(cmd.c_str());
    return ret == 0;
}

