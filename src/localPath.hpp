#ifndef CPPPARSER_LOCALPATH_HPP
#define CPPPARSER_LOCALPATH_HPP

#include "pathLocator.hpp"

namespace cppParser {

/**
 * The local path class is the simplest version of the PathLocator that just returns the entire path.
 */
class LocalPath : public PathLocator {
   private:
    const std::string path;

   public:
    /**
     * Set the path from a string
     * @param path
     */
    explicit LocalPath(const std::string& path);

    /**
     * Return the defined path
     * @return
     */
    std::filesystem::path Locate(const std::vector<std::filesystem::path>& searchPaths) override;
};

}  // namespace cppParser
#endif  // CPPPARSER_LOCALPATH_HPP
