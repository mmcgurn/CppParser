#ifndef CPPPARSER_CREATOR_HPP
#define CPPPARSER_CREATOR_HPP

#include <functional>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include "argumentIdentifier.hpp"
#include "demangler.hpp"
#include "listing.hpp"

namespace cppParser {

class Factory;

template <typename Interface>
class Creator {
   public:
    Creator() = delete;

    using TCreateMethod = std::function<std::shared_ptr<Interface>(std::shared_ptr<Factory>)>;

    static std::map<std::string, TCreateMethod>& GetConstructionMethods() {
        static auto methods = new std::map<std::string, TCreateMethod>();
        return *methods;
    }

    static std::string& GetDefaultClassName() {
        static auto defaultClassName = new std::string();
        return *defaultClassName;
    };

    static TCreateMethod GetCreateMethod(const std::string& className) {
        std::map<std::string, TCreateMethod>& methods = GetConstructionMethods();
        if (className.empty()) {
            const auto& defaultClassName = Creator<Interface>::GetDefaultClassName();
            if (auto it = methods.find(defaultClassName); it != methods.end()) return it->second;
        }
        if (auto it = methods.find(className); it != methods.end()) return it->second;

        return nullptr;
    }
};

}  // namespace cppParser

#endif  // CPPPARSER_REGISTRAR_HPP
