#ifndef CPPPARSER_ARGUMENTIDENTIFIER_HPP
#define CPPPARSER_ARGUMENTIDENTIFIER_HPP
#include <optional>
#include <string>
#include "enumWrapper.hpp"

/**
 * possible ArgumentType types
 */
enum class ArgumentType {Required, Optional, Global};

#define TMP_COMMA ,

#define ARG(interfaceTypeFullName, inputName, description) \
    cppParser::ArgumentIdentifier<interfaceTypeFullName> { inputName, description, Required }

#define OPT(interfaceTypeFullName, inputName, description) \
    cppParser::ArgumentIdentifier<interfaceTypeFullName> { inputName, description, Optional }

#define ENUM(interfaceTypeFullName, inputName, description) \
    cppParser::ArgumentIdentifier<cppParser::EnumWrapper<interfaceTypeFullName>> { inputName, description, Required }

namespace cppParser {
template <typename Interface>
struct ArgumentIdentifier {
    const std::string inputName;
    const std::string description;
    const ArgumentType type;
    bool operator==(const ArgumentIdentifier<Interface>& other) const { return inputName == other.inputName && type == other.type; }
};

template <typename Interface>
std::ostream& operator<<(std::ostream& os, const ArgumentIdentifier<Interface>& arg) {
    switch(arg.type){
        case ArgumentType::Required:
            os << arg.inputName << ": " << arg.description;
            break;
        case ArgumentType::Optional:
            os << arg.inputName << "(OPT): " << arg.description;
            break;
        case ArgumentType::Optional:
            os << arg.inputName << "(Automatic): " << arg.description;
            break;
    }
    return os;
}

}  // namespace cppParser

#endif  // CPPPARSER_ARGUMENTIDENTIFIER_HPP
