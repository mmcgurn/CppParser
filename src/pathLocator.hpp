#ifndef CPPPARSER_PATHLOCATOR_HPP
#define CPPPARSER_PATHLOCATOR_HPP

#include <filesystem>

namespace cppParser {
/**
 * The path locator class is used behind the scene to locate/download a file or web resource
 */
class PathLocator {
   public:
    virtual std::filesystem::path Locate() = 0;
    virtual ~PathLocator() = default;
};
}  // namespace cppParser

#endif  // CPPPARSER_PATHLOCATOR_HPP
