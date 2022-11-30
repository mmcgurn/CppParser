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

    struct DerivedEntry {
        const std::string interface;
        const std::string className;
        const bool defaultConstructor;

        bool operator==(const DerivedEntry &other) const;
    };

    /**
     * record normal listing
     * @param entry
     */
    virtual void RecordListing(ClassEntry entry);

    /**
     * record derived listing
     * @param entry
     */
    virtual void RecordListing(DerivedEntry entry);

    // get the singleton instance
    static Listing &Get();

    Listing(Listing &other) = delete;

    void operator=(const Listing &) = delete;

    virtual ~Listing() = default;

    // Provide a way to replace the listing
    static void ReplaceListing(std::shared_ptr<Listing>);

    std::map<std::string, std::vector<ClassEntry>> entries;

    std::map<std::string, std::vector<DerivedEntry>> derivedEntries;

   protected:
    Listing() = default;

   private:
    inline static std::shared_ptr<Listing> listing;
};

// define operators to write to a stream
std::ostream &operator<<(std::ostream &os, const Listing &listing);

std::ostream &operator<<(std::ostream &os, const Listing::ArgumentEntry &argumentEntry);

std::ostream &operator<<(std::ostream &os, const Listing::ClassEntry &classEntry);

std::ostream &operator<<(std::ostream &os, const Listing::DerivedEntry &classEntry);

}  // namespace cppParser
#endif  // CPPPARSER_LISTING_HPP
