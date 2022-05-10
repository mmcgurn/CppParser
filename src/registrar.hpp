#ifndef CPPPARSER_REGISTRAR_HPP
#define CPPPARSER_REGISTRAR_HPP

#include <functional>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include "argumentIdentifier.hpp"
#include "demangler.hpp"
#include "factory.hpp"
#include "listing.hpp"

/**
 * Helper macros for registering classes
 */

#define RESOLVE(interfaceTypeFullName, classFullName) \
    template <>                                       \
    std::shared_ptr<interfaceTypeFullName> cppParser::RegisteredInFactory<interfaceTypeFullName, classFullName>::Resolved = cppParser::ResolveAndCreate<interfaceTypeFullName>(nullptr)

/**
 * Register a class that takes a predefined set of arguments
 */
#define REGISTER(interfaceTypeFullName, classFullName, description, ...)                                                       \
    template <>                                                                                                                \
    bool cppParser::RegisteredInFactory<interfaceTypeFullName, classFullName>::Registered =                                    \
        cppParser::Registrar<interfaceTypeFullName>::Register<classFullName>(false, #classFullName, description, __VA_ARGS__); \
    RESOLVE(interfaceTypeFullName, classFullName)

/**
 * Register a class that takes a predefined set of arguments and make it the default implementation
 */
#define REGISTER_DEFAULT(interfaceTypeFullName, classFullName, description, ...)                                              \
    template <>                                                                                                               \
    bool cppParser::RegisteredInFactory<interfaceTypeFullName, classFullName>::Registered =                                   \
        cppParser::Registrar<interfaceTypeFullName>::Register<classFullName>(true, #classFullName, description, __VA_ARGS__); \
    RESOLVE(interfaceTypeFullName, classFullName)

/**
 * Register a class whose constructor takes a factory
 */
#define REGISTER_FACTORY_CONSTRUCTOR(interfaceTypeFullName, classFullName, description)                                                 \
    template <>                                                                                                                         \
    bool cppParser::RegisteredInFactory<interfaceTypeFullName, classFullName>::Registered =                                             \
        cppParser::Registrar<interfaceTypeFullName>::RegisterWithFactoryConstructor<classFullName>(false, #classFullName, description); \
    RESOLVE(interfaceTypeFullName, classFullName)

/**
 * Register a class whose constructor takes a factory and make it the default implementation
 */
#define REGISTER_DEFAULT_FACTORY_CONSTRUCTOR(interfaceTypeFullName, classFullName, description)                                        \
    template <>                                                                                                                        \
    bool cppParser::RegisteredInFactory<interfaceTypeFullName, classFullName>::Registered =                                            \
        cppParser::Registrar<interfaceTypeFullName>::RegisterWithFactoryConstructor<classFullName>(true, #classFullName, description); \
    RESOLVE(interfaceTypeFullName, classFullName)

/**
 * Register a class without constructor arguments
 */
#define REGISTER_WITHOUT_ARGUMENTS(interfaceTypeFullName, classFullName, description)                                                                                                                 \
    template <>                                                                                                                                                                                       \
    bool cppParser::RegisteredInFactory<interfaceTypeFullName, classFullName>::Registered = cppParser::Registrar<interfaceTypeFullName>::Register<classFullName>(false, #classFullName, description); \
    RESOLVE(interfaceTypeFullName, classFullName)

/**
 * Register a class without constructor arguments and make it the default implementation
 */
#define REGISTER_DEFAULT_WITHOUT_ARGUMENTS(interfaceTypeFullName, classFullName, description)                                                                                                        \
    template <>                                                                                                                                                                                      \
    bool cppParser::RegisteredInFactory<interfaceTypeFullName, classFullName>::Registered = cppParser::Registrar<interfaceTypeFullName>::Register<classFullName>(true, #classFullName, description); \
    RESOLVE(interfaceTypeFullName, classFullName)

/**
 * Register a class and a function.  This function is passed a factory and should return a shared_ptr to the class.
 */
#define REGISTER_FACTORY_FUNCTION(interfaceTypeFullName, classFullName, description, function)                                                 \
    template <>                                                                                                                                \
    bool cppParser::RegisteredInFactory<interfaceTypeFullName, classFullName>::Registered =                                                    \
        cppParser::Registrar<interfaceTypeFullName>::RegisterWithFactoryFunction<classFullName>(false, #classFullName, description, function); \
    RESOLVE(interfaceTypeFullName, classFullName)

/**
 * Register a class and a function and make it the default implementation.  This function is passed a factory and should return a shared_ptr to the class.
 */
#define REGISTER_DEFAULT_FACTORY_FUNCTION(interfaceTypeFullName, classFullName, description, function)                                        \
    template <>                                                                                                                               \
    bool cppParser::RegisteredInFactory<interfaceTypeFullName, classFullName>::Registered =                                                   \
        cppParser::Registrar<interfaceTypeFullName>::RegisterWithFactoryFunction<classFullName>(true, #classFullName, description, function); \
    RESOLVE(interfaceTypeFullName, classFullName)

/**
 * Register a class with a single arguments as pass through and make it the default implementation.  This results the specified argument being passed directly to the constructor and is used
 * to element a parser object that holds only one value.
 */
#define REGISTER_PASS_THROUGH(interfaceTypeFullName, classFullName, description, argumentType)                                                                   \
    template <>                                                                                                                                                  \
    bool cppParser::RegisteredInFactory<interfaceTypeFullName, classFullName>::Registered =                                                                      \
        cppParser::Registrar<interfaceTypeFullName>::Register<classFullName>(false, #classFullName, description, cppParser::ArgumentIdentifier<argumentType>{}); \
    RESOLVE(interfaceTypeFullName, classFullName)

/**
 * Register a class with a single arguments as pass through.  This results the specified argument being passed directly to the constructor and is used
 * to element a parser object that holds only one value.
 */
#define REGISTER_DEFAULT_PASS_THROUGH(interfaceTypeFullName, classFullName, description, argumentType)                                                          \
    template <>                                                                                                                                                 \
    bool cppParser::RegisteredInFactory<interfaceTypeFullName, classFullName>::Registered =                                                                     \
        cppParser::Registrar<interfaceTypeFullName>::Register<classFullName>(true, #classFullName, description, cppParser::ArgumentIdentifier<argumentType>{}); \
    RESOLVE(interfaceTypeFullName, classFullName)

namespace cppParser {

template <typename Interface>
class Registrar {
   public:
    Registrar() = delete;

    using TCreateMethod = std::function<std::shared_ptr<Interface>(std::shared_ptr<Factory>)>;

    static std::map<std::string, TCreateMethod>& GetConstructionMethods() {
        static auto methods = new std::map<std::string, TCreateMethod>();
        return *methods;
    }

    static std::string& GetDefaultClassName() {
        static auto defaultClassName = new std::string();
        return *defaultClassName;
    };

    /* Register a class that has factory function */
    template <typename Class>
    static bool RegisterWithFactoryFunction(bool defaultConstructor, const std::string&& className, const std::string&& description, TCreateMethod method) {
        std::map<std::string, TCreateMethod>& methods = GetConstructionMethods();
        if (auto it = methods.find(className); it == methods.end()) {
            // Record the entry
            Listing::Get().RecordListing(Listing::ClassEntry{.interface = typeid(Interface).name(), .className = className, .description = description, .defaultConstructor = defaultConstructor});

            // create method
            methods[className] = method;

            if (defaultConstructor) {
                if (GetDefaultClassName().empty()) {
                    GetDefaultClassName() = className;
                } else {
                    throw std::invalid_argument("the default parameter for " + Demangler::Demangle<Interface>() + " is already set as " + GetDefaultClassName());
                }
            }
        }
        return false;
    }

    /* Register a class that has a constructor that uses a Factory instance */
    template <typename Class>
    static bool RegisterWithFactoryConstructor(bool defaultConstructor, const std::string&& className, const std::string&& description) {
        std::map<std::string, TCreateMethod>& methods = GetConstructionMethods();
        if (auto it = methods.find(className); it == methods.end()) {
            // Record the entry
            Listing::Get().RecordListing(Listing::ClassEntry{.interface = typeid(Interface).name(), .className = className, .description = description, .defaultConstructor = defaultConstructor});

            // create method
            methods[className] = [](std::shared_ptr<Factory> factory) { return std::make_shared<Class>(factory); };

            if (defaultConstructor) {
                if (GetDefaultClassName().empty()) {
                    GetDefaultClassName() = className;
                } else {
                    throw std::invalid_argument("the default parameter for " + Demangler::Demangle<Interface>() + " is already set as " + GetDefaultClassName());
                }
            }
        }
        return false;
    }

    /* Register a class that has an empty constructor*/
    template <typename Class>
    static bool Register(bool defaultConstructor, const std::string&& className, const std::string&& description) {
        std::map<std::string, TCreateMethod>& methods = GetConstructionMethods();
        if (auto it = methods.find(className); it == methods.end()) {
            // Record the entry
            Listing::Get().RecordListing(Listing::ClassEntry{.interface = typeid(Interface).name(), .className = className, .description = description, .defaultConstructor = defaultConstructor});

            // create method
            methods[className] = [](const std::shared_ptr<Factory>& factory) { return std::make_shared<Class>(); };

            if (defaultConstructor) {
                if (GetDefaultClassName().empty()) {
                    GetDefaultClassName() = className;
                } else {
                    throw std::invalid_argument("the default parameter for " + Demangler::Demangle<Interface>() + " is already set as " + GetDefaultClassName());
                }
            }
        }
        return false;
    }

    /* Register a class with a function that takes argument identifiers */
    template <typename Class, typename... Args>
    static bool Register(bool defaultConstructor, const std::string&& className, const std::string&& description, ArgumentIdentifier<Args>&&... args) {
        std::map<std::string, TCreateMethod>& methods = GetConstructionMethods();
        if (auto it = methods.find(className); it == methods.end()) {
            // Record the entry
            Listing::Get().RecordListing(Listing::ClassEntry{
                .interface = typeid(Interface).name(),
                .className = className,
                .description = description,
                .arguments = std::vector({Listing::ArgumentEntry{.name = args.inputName, .interface = typeid(Args).name(), .description = args.description, .optional = args.optional}...}),
                .defaultConstructor = defaultConstructor});

            // create method
            methods[className] = [=](const std::shared_ptr<Factory>& factory) { return std::make_shared<Class>(factory->Get(args)...); };

            if (defaultConstructor) {
                if (GetDefaultClassName().empty()) {
                    GetDefaultClassName() = className;
                } else {
                    throw std::invalid_argument("the default parameter for " + Demangler::Demangle<Interface>() + " is already set as " + GetDefaultClassName());
                }
            }

            return true;
        }
        return false;
    }

    static TCreateMethod GetCreateMethod(const std::string& className) {
        std::map<std::string, TCreateMethod>& methods = GetConstructionMethods();
        if (auto it = methods.find(className); it != methods.end()) return it->second;

        return nullptr;
    }
};

template <typename Interface>
std::shared_ptr<Interface> ResolveAndCreate(std::shared_ptr<Factory> factory) {
    if (factory == nullptr) {
        return nullptr;
    }
    auto childType = factory->GetClassType();

    if (!childType.empty()) {
        std::function<std::shared_ptr<Interface>(std::shared_ptr<Factory>)> createMethod = Registrar<Interface>::GetCreateMethod(childType);
        if (!createMethod) {
            throw std::invalid_argument("unknown type " + childType);
        }

        return createMethod(factory);
    } else {
        // check for a default
        std::string defaultClassName = Registrar<Interface>::GetDefaultClassName();
        std::function<std::shared_ptr<Interface>(std::shared_ptr<Factory>)> createMethod = Registrar<Interface>::GetCreateMethod(defaultClassName);
        if (!createMethod) {
            throw std::invalid_argument("no default creator specified for interface " + Demangler::Demangle<Interface>());
        }

        return createMethod(factory);
    }
}
template <typename Interface, typename Class>
class RegisteredInFactory {
   public:
    static bool Registered;
    static std::shared_ptr<Interface> Resolved;
};

}  // namespace cppParser

#endif  // CPPPARSER_REGISTRAR_HPP
