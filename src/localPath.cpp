#include "localPath.hpp"

cppParser::LocalPath::LocalPath(const std::string& path) : path(path) {}

std::filesystem::path cppParser::LocalPath::Locate(const std::vector<std::filesystem::path>& searchPaths) {
    // check to see if the path exists
    if (std::filesystem::exists(path)) {
        return std::filesystem::path(path);
    }

    // check for the file in local search directories
    for (const auto& directory : searchPaths) {
        // build a test path
        auto testPath = directory / path;
        if (std::filesystem::exists(testPath)) {
            return std::filesystem::canonical(testPath);
        }
    }

    return path;
}

#include "registrar.hpp"
REGISTER_DEFAULT_PASS_THROUGH(cppParser::PathLocator, cppParser::LocalPath, "Default path locator that returns specified local path", std::string);
