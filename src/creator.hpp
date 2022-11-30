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

/**
 * Use this symbol to mark that a derived class is default
 */
static inline std::string DerivedSymbol = "->";

template <typename Interface>
class Creator {
   public:
    Creator() = delete;

    using TCreateMethod = std::function<std::shared_ptr<Interface>(std::shared_ptr<Factory>)>;

    /**
     * static map of construction methods for this interface
     * @return
     */
    static std::map<std::string, TCreateMethod>& GetConstructionMethods() {
        static auto methods = new std::map<std::string, TCreateMethod>();
        return *methods;
    }

    /**
     * static map of constructor methods for classes/interfaces that inherit from this
     * @return
     */
    static std::map<std::string, std::function<TCreateMethod(const std::string&)>>& GetDerivedConstructionMethods() {
        static auto methods = new std::map<std::string, std::function<TCreateMethod(const std::string&)>>();
        return *methods;
    }

    static std::string& GetDefaultClassName() {
        static auto defaultClassName = new std::string();
        return *defaultClassName;
    };

    /**
     * check this and inherited class creator methods
     * @param className
     * @return
     */
    static TCreateMethod GetCreateMethod(const std::string& className) {
        std::map<std::string, TCreateMethod>& methods = GetConstructionMethods();
        if (className.empty()) {
            const auto& defaultClassName = Creator<Interface>::GetDefaultClassName();
            if (auto it = methods.find(defaultClassName); it != methods.end()) return it->second;

            // Also check th derived classes
            const auto& derivedConstructionMethods = GetDerivedConstructionMethods();
            if (auto it = derivedConstructionMethods.find(DerivedSymbol + defaultClassName); it != derivedConstructionMethods.end()) return it->second(className);
        }
        // Now check each method for the class name
        if (auto it = methods.find(className); it != methods.end()) return it->second;

        // check each of the derivedConstructionMethods
        for (auto& [derivedClassName, derivedConstructionMethod] : GetDerivedConstructionMethods()) {
            auto testResult = derivedConstructionMethod(className);
            if (testResult != nullptr) {
                return testResult;
            }
        }

        return nullptr;
    }
};

}  // namespace cppParser

#endif  // CPPPARSER_REGISTRAR_HPP
