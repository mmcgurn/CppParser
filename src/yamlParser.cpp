#include "yamlParser.hpp"

#include <utility>

cppParser::YamlParser::YamlParser(const YAML::Node& yamlConfiguration, std::string nodePath, std::string type, std::function<std::filesystem::path(std::string)> locateFileFunction,
                                  std::weak_ptr<InstanceTracker> instanceTracker)
    : Factory(instanceTracker), type(std::move(type)), nodePath(std::move(nodePath)), yamlConfiguration(yamlConfiguration), locateFileFunction(std::move(locateFileFunction)) {
    // store each child in the map with zero usages
    for (auto childNode : yamlConfiguration) {
        nodeUsages[YAML::key_to_string(childNode.first)] = 0;
    }
}

cppParser::YamlParser::YamlParser(YAML::Node yamlConfiguration, std::function<std::filesystem::path(std::string)> locateFileFunction, const std::map<std::string, std::string>& overwriteParameters)
    : YamlParser(yamlConfiguration, "root", "", std::move(locateFileFunction)) {
    // create the root instance of the tracker
    rootInstanceTracker = std::make_shared<InstanceTracker>();
    instanceTracker = rootInstanceTracker;

    // override/add any of the values in the overwriteParameters
    for (const auto& pair : overwriteParameters) {
        ReplaceValue(yamlConfiguration, pair.first, pair.second);
    }
}

cppParser::YamlParser::YamlParser(const std::string& yamlString, std::function<std::filesystem::path(std::string)> locateFileFunction, const std::map<std::string, std::string>& overwriteParameters)
    : YamlParser(YAML::Load(yamlString), std::move(locateFileFunction), overwriteParameters) {}

cppParser::YamlParser::YamlParser(const std::filesystem::path& filePath, std::function<std::filesystem::path(std::string)> locateFileFunction,
                                  const std::map<std::string, std::string>& overwriteParameters)
    : YamlParser(YAML::LoadFile(filePath), std::move(locateFileFunction), overwriteParameters) {}

std::shared_ptr<cppParser::Factory> cppParser::YamlParser::GetFactory(const std::string& name) const {
    // Check to see if the child factory has already been created
    if (childFactories.count(name) == 0) {
        if (name.empty()) {
            auto parameter = yamlConfiguration;
            auto childPath = nodePath;

            // This is the child, so assume that the tag is empty
            auto tagType = "";

            // Mark all children here on used, because they will be counted in the child
            MarkAllUsed();
            MarkUsage(name);
            childFactories[name] = std::shared_ptr<YamlParser>(new YamlParser(parameter, childPath, tagType, locateFileFunction, instanceTracker));
        } else {
            auto parameter = yamlConfiguration[name];
            auto childPath = nodePath + "/" + name;

            if (!parameter) {
                throw std::invalid_argument("unable to find item " + name + " in " + nodePath);
            }

            auto tagType = parameter.Tag();
            // Remove the ! or ? from the tag
            tagType = !tagType.empty() ? tagType.substr(1) : tagType;

            // mark usage and store pointer
            MarkUsage(name);
            childFactories[name] = std::shared_ptr<YamlParser>(new YamlParser(parameter, childPath, tagType, locateFileFunction, instanceTracker));
        }
    }

    return childFactories[name];
}

std::vector<std::shared_ptr<cppParser::Factory>> cppParser::YamlParser::GetFactorySequence(const std::string& name) const {
    auto parameter = yamlConfiguration[name];
    if (!parameter) {
        throw std::invalid_argument("unable to find list " + name + " in " + nodePath);
    }

    if (!parameter.IsSequence()) {
        throw std::invalid_argument("item " + name + " is expected to be a sequence in " + nodePath);
    }

    std::vector<std::shared_ptr<Factory>> children;

    // march over each child
    for (std::size_t i = 0; i < parameter.size(); i++) {
        std::string childName = name + "/" + std::to_string(i);

        if (childFactories.count(childName) == 0) {
            auto childParameter = parameter[i];

            if (!childParameter.IsDefined()) {
                throw std::invalid_argument("item " + childName + " is expected to be a defined in " + nodePath + "/" + name);
            }

            std::string childPath = nodePath + "/" + childName;

            auto tagType = childParameter.Tag();
            // Remove the ! or ? from the tag
            tagType = !tagType.empty() ? tagType.substr(1) : tagType;

            // mark usage and store pointer
            childFactories[childName] = std::shared_ptr<YamlParser>(new YamlParser(childParameter, childPath, tagType, locateFileFunction, instanceTracker));
        }

        children.push_back(childFactories[childName]);
    }

    MarkUsage(name);

    return children;
}

std::vector<std::string> cppParser::YamlParser::GetUnusedValues() const {
    std::vector<std::string> unused;

    for (const auto& children : nodeUsages) {
        if (children.second == 0) {
            unused.push_back(nodePath + "/" + children.first);
        }
    }

    // Add any unused children from used children
    for (const auto& childFactory : childFactories) {
        auto unusedChildren = childFactory.second->GetUnusedValues();
        unused.insert(std::end(unused), std::begin(unusedChildren), std::end(unusedChildren));
    }

    return unused;
}

std::unordered_set<std::string> cppParser::YamlParser::GetKeys() const {
    std::unordered_set<std::string> keys;

    for (auto childNode : yamlConfiguration) {
        keys.insert(key_to_string(childNode.first));
    }

    return keys;
}

std::filesystem::path cppParser::YamlParser::Get(const cppParser::ArgumentIdentifier<std::filesystem::path>& identifier) const {
    // The yaml parser just refers to the global environment to file the file
    auto file = Get(cppParser::ArgumentIdentifier<std::string>{.inputName = identifier.inputName, .optional = identifier.optional});

    if (identifier.optional && file.empty()) {
        return {};
    } else if (locateFileFunction) {
        return locateFileFunction(file);
    } else {
        return file;
    }
}

void cppParser::YamlParser::Print(std::ostream& stream) const { stream << "---" << std::endl << yamlConfiguration << std::endl; }

void cppParser::YamlParser::ReplaceValue(YAML::Node& yamlConfiguration, const std::string& key, const std::string& value) {
    // Check to see if there are any separators in the key
    auto separator = key.find("::");
    if (separator == std::string::npos) {
        // Allow the yaml parser to parse the string like normal
        auto node = YAML::Load(value);
        yamlConfiguration[key] = node;
    } else {
        // strip off the first part of the key and try again
        auto thisKey = key.substr(0, separator);

        // check to see if this key is an index
        if (!thisKey.empty() && thisKey[0] == '[' && thisKey.back() == ']') {
            auto index = std::stoi(thisKey.substr(1, thisKey.size() - 2));
            auto childConfig = yamlConfiguration[index];
            ReplaceValue(childConfig, key.substr(separator + 2), value);
        } else {
            auto childConfig = yamlConfiguration[thisKey];
            ReplaceValue(childConfig, key.substr(separator + 2), value);
        }
    }
}
