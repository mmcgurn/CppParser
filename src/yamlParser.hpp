#ifndef CPPPARSER_YAMLPARSER_HPP
#define CPPPARSER_YAMLPARSER_HPP

#include <yaml-cpp/yaml.h>
#include <filesystem>
#include <functional>
#include <iostream>
#include "factory.hpp"

namespace cppParser {
class YamlParser : public Factory {
   private:
    const std::string type;
    const std::string nodePath;
    const YAML::Node yamlConfiguration;
    mutable std::map<std::string, int> nodeUsages;
    mutable std::map<std::string, std::shared_ptr<YamlParser>> childFactories;
    const std::vector<std::filesystem::path> searchDirectories;

    // The root YamlParser should store a shared ptr to a     mutable std::weak_ptr<InstanceTracker> instanceTracker;
    std::shared_ptr<InstanceTracker> rootInstanceTracker;

    /***
     * private constructor to create a sub factory
     * @param yamlConfiguration
     * @param nodePath
     * @param type
     */
    YamlParser(const YAML::Node& yamlConfiguration, std::string nodePath, std::string type, std::vector<std::filesystem::path> searchDirectories, std::weak_ptr<InstanceTracker> instanceTracker = {});
    inline void MarkUsage(const std::string& key) const { nodeUsages[key]++; }

    /**
     * Marks all of the keys used.
     */
    void MarkAllUsed() const override {
        for (auto& pairs : nodeUsages) {
            pairs.second++;
        }
    }

    /***
     * Helper Function to get the correct parameters
     */
    template <typename T>
    inline YAML::Node GetParameter(const ArgumentIdentifier<T>& identifier) const {
        // treat this yamlConfiguration as the item if the identifier is default
        if (identifier.inputName.empty()) {
            return yamlConfiguration;
        } else if (yamlConfiguration.IsMap()) {
            return yamlConfiguration[identifier.inputName];
        } else {
            return YAML::Node(YAML::NodeType::Undefined);
        }
    }

    template <typename T>
    inline T GetValueFromYaml(const ArgumentIdentifier<T>& identifier) const {
        // treat this yamlConfiguration as the item if the identifier is default
        auto parameter = GetParameter(identifier);
        if (!parameter) {
            if (identifier.optional) {
                return {};
            } else {
                throw std::invalid_argument("unable to locate " + identifier.inputName + " in " + nodePath);
            }
        }
        MarkUsage(identifier.inputName);
        return parameter.template as<T>();
    }

    /**
     * recursive call to update parameters
     */
    static void ReplaceValue(YAML::Node& yamlConfiguration, const std::string& key, const std::string& value);

   public:
    explicit YamlParser(YAML::Node yamlConfiguration, std::vector<std::filesystem::path> searchDirectories = {}, const std::map<std::string, std::string>& overwriteParameters = {});
    ~YamlParser() override = default;

    /***
     * Direct creation using a yaml string
     * @param yamlString
     */
    explicit YamlParser(const std::string& yamlString, std::vector<std::filesystem::path> searchDirectories = {}, const std::map<std::string, std::string>& overwriteParameters = {});

    /***
     * Read in file from system
     * @param filePath
     */
    explicit YamlParser(const std::filesystem::path& filePath, const std::map<std::string, std::string>& overwriteParameters = {});

    /* gets the class type represented by this factory */
    const std::string& GetClassType() const override { return type; }

    /* return a string*/
    std::string Get(const ArgumentIdentifier<std::string>& identifier) const override {
        // treat this yamlConfiguration as the item if the identifier is default
        auto parameter = GetParameter(identifier);
        if (!parameter) {
            if (identifier.optional) {
                return {};
            } else {
                throw std::invalid_argument("unable to locate " + identifier.inputName + " in " + nodePath);
            }
        }
        MarkUsage(identifier.inputName);
        if (parameter.IsSequence()) {
            // Merge the results into a single space separated string
            std::stringstream ss;
            for (const auto& v : parameter) {
                ss << v.template as<std::string>() << " ";
            }
            return ss.str();
        } else {
            return parameter.template as<std::string>();
        }
    }

    std::vector<std::string> Get(const ArgumentIdentifier<std::vector<std::string>>& identifier) const override { return GetValueFromYaml<std::vector<std::string>>(identifier); }

    bool Get(const ArgumentIdentifier<bool>& identifier) const override { return GetValueFromYaml<bool>(identifier); };

    double Get(const ArgumentIdentifier<double>& identifier) const override { return GetValueFromYaml<double>(identifier); };

    std::vector<int> Get(const ArgumentIdentifier<std::vector<int>>& identifier) const override { return GetValueFromYaml<std::vector<int>>(identifier); }

    std::vector<double> Get(const ArgumentIdentifier<std::vector<double>>& identifier) const override { return GetValueFromYaml<std::vector<double>>(identifier); }

    std::map<std::string, std::string> Get(const ArgumentIdentifier<std::map<std::string, std::string>>& identifier) const override {
        return GetValueFromYaml<std::map<std::string, std::string>>(identifier);
    }

    /* return an int for the specified identifier*/
    int Get(const ArgumentIdentifier<int>& identifier) const override { return GetValueFromYaml<int>(identifier); }

    /* return a factory that serves as the root of the requested item */
    std::shared_ptr<Factory> GetFactory(const std::string& name) const override;

    /* get all children as factory */
    std::vector<std::shared_ptr<Factory>> GetFactorySequence(const std::string& name) const override;

    bool Contains(const std::string& name) const override { return yamlConfiguration.IsMap() && yamlConfiguration[name].IsDefined() && !yamlConfiguration[name].IsNull(); };

    std::unordered_set<std::string> GetKeys() const override;

    /** get unused values **/
    std::vector<std::string> GetUnusedValues() const override;

    /** print a copy of the yaml input as updated **/
    void Print(std::ostream&) const;

    /** provide comparison between factories **/
    virtual bool SameFactory(const Factory& otherFactory) const override {
        const auto otherFactoryPtr = dynamic_cast<const YamlParser*>(&otherFactory);

        if (otherFactoryPtr) {
            return yamlConfiguration == otherFactoryPtr->yamlConfiguration;
        } else {
            return false;
        }
    }

    /**
     * returns the path to a file as specified using a file locator instance.  This override allows searching in search directories
     * @param identifier
     * @return
     */
    std::filesystem::path Get(const ArgumentIdentifier<std::filesystem::path>& identifier) const override;
};
}  // namespace cppParser

#endif  // CPPPARSER_YAMLPARSER_HPP
