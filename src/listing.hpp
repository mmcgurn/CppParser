#ifndef CPPPARSER_LISTING_HPP
#define CPPPARSER_LISTING_HPP

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace cppParser {
class Listing {
   public:
    struct ArgumentEntry {
        const std::string name;
        const std::string interface;
        const std::string description;
        const bool optional;

        bool operator==(const ArgumentEntry &other) const;
    };

    struct ClassEntry {
        const std::string interface;
        const std::string className;
        const std::string description;
        const std::vector<ArgumentEntry> arguments;
        const bool defaultConstructor;

        bool operator==(const ClassEntry &other) const;
    };

    // provide access to the listing
    virtual void RecordListing(ClassEntry entry);

    // get the singleton instance
    static Listing &Get();

    Listing(Listing &other) = delete;

    void operator=(const Listing &) = delete;

    virtual ~Listing() = default;

    // Provide a way to replace the listing
    static void ReplaceListing(std::shared_ptr<Listing>);

    std::map<std::string, std::vector<ClassEntry>> entries;

   protected:
    Listing() = default;

   private:
    inline static std::shared_ptr<Listing> listing;
};

// define operators to write to a stream
std::ostream &operator<<(std::ostream &os, const Listing &listing);

std::ostream &operator<<(std::ostream &os, const Listing::ArgumentEntry &argumentEntry);

std::ostream &operator<<(std::ostream &os, const Listing::ClassEntry &classEntry);

}  // namespace cppParser
#endif  // CPPPARSER_LISTING_HPP
