#ifndef CPPPARSER_ENUMWRAPPER_HPP
#define CPPPARSER_ENUMWRAPPER_HPP
#include <optional>
#include <sstream>
#include <string>

namespace cppParser {
template <typename Enum>
class EnumWrapper {
   private:
    Enum value;

   public:
    EnumWrapper(Enum value) : value(value) {}

    EnumWrapper(std::string value) {
        std::istringstream stream(value);
        stream >> this->value;
    }

    operator Enum() const { return value; }
};
}  // namespace cppParser

#endif  // CPPPARSER_ENUMWRAPPER_HPP
