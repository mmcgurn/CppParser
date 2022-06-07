#include "localPath.hpp"

cppParser::LocalPath::LocalPath(const std::string& path) : path(path) {}

std::filesystem::path cppParser::LocalPath::Locate() { return path; }

#include "registrar.hpp"
REGISTER_DEFAULT_PASS_THROUGH(cppParser::PathLocator, cppParser::LocalPath, "Default path locator that returns specified local path", std::string);
