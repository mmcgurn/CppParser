#ifndef CPPPARSER_FACTORY_HPP
#define CPPPARSER_FACTORY_HPP

#include <filesystem>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>
#include "argumentIdentifier.hpp"
#include "instanceTracker.hpp"

namespace cppParser {

class Factory;
template <typename Interface>
std::shared_ptr<Interface> ResolveAndCreate(std::shared_ptr<Factory> factory);

class Factory {
   protected:
    mutable std::weak_ptr<InstanceTracker> instanceTracker;

   public:
    explicit Factory(std::weak_ptr<InstanceTracker> instanceTracker = {}) : instanceTracker(instanceTracker) {}
    virtual ~Factory() = default;

    /* return a factory that serves as the root of the requested item */
    virtual std::shared_ptr<Factory> GetFactory(const std::string& name) const = 0;

    virtual std::vector<std::shared_ptr<Factory>> GetFactorySequence(const std::string& name) const = 0;

    /* gets the class type represented by this factory */
    virtual const std::string& GetClassType() const = 0;

    /* return a string*/
    virtual std::string Get(const ArgumentIdentifier<std::string>& identifier) const = 0;

    /* return an int for the specified identifier*/
    virtual int Get(const ArgumentIdentifier<int>& identifier) const = 0;

    /* return a bool for the specified identifier*/
    virtual bool Get(const ArgumentIdentifier<bool>& identifier) const = 0;

    /* return a double for the specified identifier*/
    virtual double Get(const ArgumentIdentifier<double>& identifier) const = 0;

    /* return a vector of strings */
    virtual std::vector<std::string> Get(const ArgumentIdentifier<std::vector<std::string>>& identifier) const = 0;

    /* return a vector of int */
    virtual std::vector<int> Get(const ArgumentIdentifier<std::vector<int>>& identifier) const = 0;

    /* return a vector of double */
    virtual std::vector<double> Get(const ArgumentIdentifier<std::vector<double>>& identifier) const = 0;

    /* return a map of strings */
    virtual std::map<std::string, std::string> Get(const ArgumentIdentifier<std::map<std::string, std::string>>& identifier) const = 0;

    /* returns the path to a file as specified byname */
    virtual std::filesystem::path Get(const ArgumentIdentifier<std::filesystem::path>& identifier) const = 0;

    /* check to see if the child is contained*/
    virtual bool Contains(const std::string& name) const = 0;

    virtual std::unordered_set<std::string> GetKeys() const = 0;

    /* provide a virtual IsSameFactory function */
    virtual bool SameFactory(const Factory& otherFactory) const = 0;

    /* provide a virtual IsSameFactory function */
    inline bool operator==(const Factory& otherFactory) const { return SameFactory(otherFactory); }

    /** Promote tracking used/unused values to the factory **/
    virtual std::vector<std::string> GetUnusedValues() const = 0;

    /* produce a shared pointer for the specified interface and type */
    template <typename Interface>
    std::shared_ptr<Interface> Get(const ArgumentIdentifier<Interface>& identifier) const {
        if (identifier.optional && !Contains(identifier.inputName)) {
            return {};
        }

        // build the child factory
        auto childFactory = GetFactory(identifier.inputName);

        // check to see if the child factory has already been used to create an instance of the interface
        if (auto instanceTrackerPtr = instanceTracker.lock()) {
            if (auto instance = instanceTrackerPtr->GetInstance<Interface>(childFactory)) {
                // Mark that child factory as using all children because this factory won't be used
                childFactory->MarkAllUsed();

                return instance;
            }
        }

        // build the new instance
        auto newInstance = ResolveAndCreate<Interface>(childFactory);

        // store the instance if needed
        if (auto instanceTrackerPtr = instanceTracker.lock()) {
            instanceTrackerPtr->template SetInstance<Interface>(childFactory, newInstance);
        }

        return newInstance;
    }

    template <typename Interface>
    std::vector<std::shared_ptr<Interface>> Get(const ArgumentIdentifier<std::vector<Interface>>& identifier) const {
        if (identifier.optional && !Contains(identifier.inputName)) {
            return {};
        }

        auto childFactories = GetFactorySequence(identifier.inputName);

        // Build and resolve the list
        std::vector<std::shared_ptr<Interface>> results;
        for (auto childFactory : childFactories) {
            results.push_back(ResolveAndCreate<Interface>(childFactory));
        }

        return results;
    }

    template <typename Interface>
    std::map<std::string, std::shared_ptr<Interface>> Get(const ArgumentIdentifier<std::map<std::string, Interface>>& identifier) const {
        if (identifier.optional && !Contains(identifier.inputName)) {
            return {};
        }

        auto childFactory = GetFactory(identifier.inputName);
        auto childrenNames = childFactory->GetKeys();

        // Build and resolve the list
        std::map<std::string, std::shared_ptr<Interface>> results;
        for (auto childName : childrenNames) {
            auto subChildFactory = childFactory->GetFactory(childName);

            results[childName] = (ResolveAndCreate<Interface>(subChildFactory));
        }

        return results;
    }

    template <typename Interface>
    inline auto GetByName(const std::string inputName) {
        return Get(ArgumentIdentifier<Interface>{.inputName = inputName});
    }

    template <typename Interface, typename DefaultValueInterface>
    inline auto GetByName(const std::string inputName, DefaultValueInterface defaultValue) {
        if (Contains(inputName)) {
            return Get(ArgumentIdentifier<Interface>{.inputName = inputName});
        } else {
            return defaultValue;
        }
    }

    template <typename ENUM>
    ENUM Get(const ArgumentIdentifier<EnumWrapper<ENUM>>& identifier) const {
        auto stringValue = Get(ArgumentIdentifier<std::string>{.inputName = identifier.inputName, .optional = identifier.optional});
        return EnumWrapper<ENUM>(stringValue);
    }

   protected:
    /** Mark the values as used*/
    virtual void MarkAllUsed() const = 0;
};
}  // namespace cppParser

#endif  // CPPPARSER_FACTORY_HPP