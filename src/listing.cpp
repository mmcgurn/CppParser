#include "listing.hpp"
#include "demangler.hpp"

void cppParser::Listing::RecordListing(cppParser::Listing::ClassEntry entry) { entries[entry.interface].push_back(std::move(entry)); }

cppParser::Listing &cppParser::Listing::Get() {
    if (listing == nullptr) {
        listing = std::shared_ptr<Listing>(new Listing());
    }

    return *listing;
}

std::ostream &cppParser::operator<<(std::ostream &os, const cppParser::Listing &listing) {
    for (const auto &interface : listing.entries) {
        os << "# " << interface.first << std::endl;
        for (const auto &classEntry : interface.second) {
            os << classEntry;
        }
    }

    return os;
}

std::ostream &cppParser::operator<<(std::ostream &os, const cppParser::Listing::ArgumentEntry &argumentEntry) {
    // if there is no name or description just return
    if (argumentEntry.name.empty() && argumentEntry.description.empty()) {
        return os;
    }
    os << argumentEntry.name << (argumentEntry.optional ? "" : " (req) ") << std::endl;
    os << ": "
       << "(" << argumentEntry.interface << ") " << argumentEntry.description;
    os << std::endl << std::endl;
    return os;
}

std::ostream &cppParser::operator<<(std::ostream &os, const cppParser::Listing::ClassEntry &classEntry) {
    os << "## " << classEntry.className << (classEntry.defaultConstructor ? "*" : "") << std::endl << classEntry.description << std::endl << std::endl;
    for (const auto &argumentEntry : classEntry.arguments) {
        os << argumentEntry;
    }
    return os;
}

void cppParser::Listing::ReplaceListing(std::shared_ptr<Listing> replacementListing) { listing = std::move(replacementListing); }

bool cppParser::Listing::ClassEntry::operator==(const cppParser::Listing::ClassEntry &other) const {
    return className == other.className && interface == other.interface && description == other.description && arguments == other.arguments;
}

bool cppParser::Listing::ArgumentEntry::operator==(const cppParser::Listing::ArgumentEntry &other) const {
    return name == other.name && interface == other.interface && description == other.description;
}
