#pragma once
#include <string>
#include <vector>

struct ModFile {
    std::string path;
    std::string url;
};

std::vector<ModFile> parseModrinthIndex(const std::string& jsonFile);
