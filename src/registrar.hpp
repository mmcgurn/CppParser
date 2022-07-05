#ifndef CPPPARSER_REGISTRAR_HPP
#define CPPPARSER_REGISTRAR_HPP

#include "factory.hpp"

/**
 * Register a class that takes a predefined set of arguments
 */
#define REGISTER(interfaceTypeFullName, classFullName, description, ...)                    \
    template <>                                                                             \
    bool cppParser::RegisteredInFactory<interfaceTypeFullName, classFullName>::Registered = \
        cppParser::Registrar<interfaceTypeFullName>::Register<classFullName>(false, #classFullName, description, __VA_ARGS__);

/**
 * Register a class that takes a predefined set of arguments and make it the default implementation
 */
#define REGISTER_DEFAULT(interfaceTypeFullName, classFullName, description, ...)            \
    template <>                                                                             \
    bool cppParser::RegisteredInFactory<interfaceTypeFullName, classFullName>::Registered = \
        cppParser::Registrar<interfaceTypeFullName>::Register<classFullName>(true, #classFullName, description, __VA_ARGS__);

/**
 * Register a class whose constructor takes a factory
 */
#define REGISTER_FACTORY_CONSTRUCTOR(interfaceTypeFullName, classFullName, description)     \
    template <>                                                                             \
    bool cppParser::RegisteredInFactory<interfaceTypeFullName, classFullName>::Registered = \
        cppParser::Registrar<interfaceTypeFullName>::RegisterWithFactoryConstructor<classFullName>(false, #classFullName, description);

/**
 * Register a class whose constructor takes a factory and make it the default implementation
 */
#define REGISTER_DEFAULT_FACTORY_CONSTRUCTOR(interfaceTypeFullName, classFullName, description) \
    template <>                                                                                 \
    bool cppParser::RegisteredInFactory<interfaceTypeFullName, classFullName>::Registered =     \
        cppParser::Registrar<interfaceTypeFullName>::RegisterWithFactoryConstructor<classFullName>(true, #classFullName, description);

/**
 * Register a class without constructor arguments
 */
#define REGISTER_WITHOUT_ARGUMENTS(interfaceTypeFullName, classFullName, description) \
    template <>                                                                       \
    bool cppParser::RegisteredInFactory<interfaceTypeFullName, classFullName>::Registered = cppParser::Registrar<interfaceTypeFullName>::Register<classFullName>(false, #classFullName, description);

/**
 * Register a class without constructor arguments and make it the default implementation
 */
#define REGISTER_DEFAULT_WITHOUT_ARGUMENTS(interfaceTypeFullName, classFullName, description) \
    template <>                                                                               \
    bool cppParser::RegisteredInFactory<interfaceTypeFullName, classFullName>::Registered = cppParser::Registrar<interfaceTypeFullName>::Register<classFullName>(true, #classFullName, description);

/**
 * Register a class and a function.  This function is passed a factory and should return a shared_ptr to the class.
 */
#define REGISTER_FACTORY_FUNCTION(interfaceTypeFullName, classFullName, description, function) \
    template <>                                                                                \
    bool cppParser::RegisteredInFactory<interfaceTypeFullName, classFullName>::Registered =    \
        cppParser::Registrar<interfaceTypeFullName>::RegisterWithFactoryFunction<classFullName>(false, #classFullName, description, function);

/**
 * Register a class and a function and make it the default implementation.  This function is passed a factory and should return a shared_ptr to the class.
 */
#define REGISTER_DEFAULT_FACTORY_FUNCTION(interfaceTypeFullName, classFullName, description, function) \
    template <>                                                                                        \
    bool cppParser::RegisteredInFactory<interfaceTypeFullName, classFullName>::Registered =            \
        cppParser::Registrar<interfaceTypeFullName>::RegisterWithFactoryFunction<classFullName>(true, #classFullName, description, function);

/**
 * Register a class with a single arguments as pass through and make it the default implementation.  This results the specified argument being passed directly to the constructor and is
 * used to element a parser object that holds only one value.
 */
#define REGISTER_PASS_THROUGH(interfaceTypeFullName, classFullName, description, argumentType) \
    template <>                                                                                \
    bool cppParser::RegisteredInFactory<interfaceTypeFullName, classFullName>::Registered =    \
        cppParser::Registrar<interfaceTypeFullName>::Register<classFullName>(false, #classFullName, description, cppParser::ArgumentIdentifier<argumentType>{});

/**
 * Register a class with a single arguments as pass through.  This results the specified argument being passed directly to the constructor and is used
 * to element a parser object that holds only one value.
 */
#define REGISTER_DEFAULT_PASS_THROUGH(interfaceTypeFullName, classFullName, description, argumentType) \
                                                                                                       \
    template <>                                                                                        \
    bool cppParser::RegisteredInFactory<interfaceTypeFullName, classFullName>::Registered =            \
        cppParser::Registrar<interfaceTypeFullName>::Register<classFullName>(true, #classFullName, description, cppParser::ArgumentIdentifier<argumentType>{});

namespace cppParser {
template <typename Interface>
/**
 * Registrar class to register class with the Creator
 * @tparam Interface
 */
class Registrar {
   public:
    Registrar() = delete;

    /* Register a class that has factory function */
    template <typename Class>
    static bool RegisterWithFactoryFunction(bool defaultConstructor, const std::string&& className, const std::string&& description, typename cppParser::Creator<Interface>::TCreateMethod method) {
        std::map<std::string, typename cppParser::Creator<Interface>::TCreateMethod>& methods = cppParser::Creator<Interface>::GetConstructionMethods();
        if (auto it = methods.find(className); it == methods.end()) {
            // Record the entry
            Listing::Get().RecordListing(
                Listing::ClassEntry{.interface = Demangler::Demangle<Interface>(), .className = className, .description = description, .defaultConstructor = defaultConstructor});

            // create method
            methods[className] = method;

            if (defaultConstructor) {
                if (cppParser::Creator<Interface>::GetDefaultClassName().empty()) {
                    cppParser::Creator<Interface>::GetDefaultClassName() = className;
                } else {
                    throw std::invalid_argument("the default parameter for " + Demangler::Demangle<Interface>() + " is already set as " + cppParser::Creator<Interface>::GetDefaultClassName());
                }
            }
        }
        return false;
    }

    /* Register a class that has a constructor that uses a Factory instance */
    template <typename Class>
    static bool RegisterWithFactoryConstructor(bool defaultConstructor, const std::string&& className, const std::string&& description) {
        std::map<std::string, typename cppParser::Creator<Interface>::TCreateMethod>& methods = cppParser::Creator<Interface>::GetConstructionMethods();
        if (auto it = methods.find(className); it == methods.end()) {
            // Record the entry
            Listing::Get().RecordListing(
                Listing::ClassEntry{.interface = Demangler::Demangle<Interface>(), .className = className, .description = description, .defaultConstructor = defaultConstructor});

            // create method
            methods[className] = [](std::shared_ptr<Factory> factory) { return std::make_shared<Class>(factory); };

            if (defaultConstructor) {
                if (cppParser::Creator<Interface>::GetDefaultClassName().empty()) {
                    cppParser::Creator<Interface>::GetDefaultClassName() = className;
                } else {
                    throw std::invalid_argument("the default parameter for " + Demangler::Demangle<Interface>() + " is already set as " + cppParser::Creator<Interface>::GetDefaultClassName());
                }
            }
        }
        return false;
    }

    /* Register a class that has an empty constructor*/
    template <typename Class>
    static bool Register(bool defaultConstructor, const std::string&& className, const std::string&& description) {
        std::map<std::string, typename cppParser::Creator<Interface>::TCreateMethod>& methods = cppParser::Creator<Interface>::GetConstructionMethods();
        if (auto it = methods.find(className); it == methods.end()) {
            // Record the entry
            Listing::Get().RecordListing(
                Listing::ClassEntry{.interface = Demangler::Demangle<Interface>(), .className = className, .description = description, .defaultConstructor = defaultConstructor});

            // create method
            methods[className] = [](const std::shared_ptr<Factory>& factory) { return std::make_shared<Class>(); };

            if (defaultConstructor) {
                if (cppParser::Creator<Interface>::GetDefaultClassName().empty()) {
                    cppParser::Creator<Interface>::GetDefaultClassName() = className;
                } else {
                    throw std::invalid_argument("the default parameter for " + Demangler::Demangle<Interface>() + " is already set as " + cppParser::Creator<Interface>::GetDefaultClassName());
                }
            }
        }
        return false;
    }

    /* Register a class with a function that takes argument identifiers */
    template <typename Class, typename... Args>
    static bool Register(bool defaultConstructor, const std::string&& className, const std::string&& description, ArgumentIdentifier<Args>&&... args) {
        std::map<std::string, typename cppParser::Creator<Interface>::TCreateMethod>& methods = cppParser::Creator<Interface>::GetConstructionMethods();
        if (auto it = methods.find(className); it == methods.end()) {
            // Record the entry
            Listing::Get().RecordListing(Listing::ClassEntry{
                .interface = Demangler::Demangle<Interface>(),
                .className = className,
                .description = description,
                .arguments = std::vector({Listing::ArgumentEntry{.name = args.inputName, .interface = Demangler::Demangle<Args>(), .description = args.description, .optional = args.optional}...}),
                .defaultConstructor = defaultConstructor});

            // create method
            methods[className] = [=](const std::shared_ptr<Factory>& factory) { return std::make_shared<Class>(factory->Get(args)...); };

            if (defaultConstructor) {
                if (cppParser::Creator<Interface>::GetDefaultClassName().empty()) {
                    cppParser::Creator<Interface>::GetDefaultClassName() = className;
                } else {
                    throw std::invalid_argument("the default parameter for " + Demangler::Demangle<Interface>() + " is already set as " + cppParser::Creator<Interface>::GetDefaultClassName());
                }
            }

            return true;
        }
        return false;
    }
};

template <typename Interface, typename Class>
class RegisteredInFactory {
   public:
    static bool Registered;
    static std::shared_ptr<Interface> Resolved;
};

}  // namespace cppParser

#endif  // CPPPARSER_REGISTRAR_HPP
