#ifndef CPPPARSER_DEMANGLER_H
#define CPPPARSER_DEMANGLER_H

#include <filesystem>
#include <map>
#include <numeric>
#include <regex>
#include <string>
#include <typeinfo>
#include <vector>

namespace cppParser {
class Demangler {
   public:
    static std::string Demangle(const std::string &);

   private:
    template <typename Test, template <typename...> class Ref>
    struct IsSpecialization : std::false_type {};

    template <template <typename...> class Ref, typename... Args>
    struct IsSpecialization<Ref<Args...>, Ref> : std::true_type {};

    template <class T>
    inline static std::string GetArgName() {
        if (IsSpecialization<T, std::allocator>::value) {
            return "";
        }
        if (IsSpecialization<T, std::less>::value) {
            return "";
        }
        return Demangle(typeid(T).name());
    }

    template <typename Test, template <typename...> class Ref>
    struct TypeInfo : std::false_type {
        static std::string GetTypeName() { return ""; }
    };

    template <template <typename...> class Ref, typename... Args>
    struct TypeInfo<Ref<Args...>, Ref> {
        static std::string GetTypeName() {
            std::vector<std::string> names = {GetArgName<Args>()...};
            // remove default allocator is listed
            return std::accumulate(names.begin() + 1, names.end(), names.front(), [](std::string x, std::string y) {
                if (y.empty()) {
                    return x;
                }
                return x + "," + y;
            });
        }
    };

   public:
    template <class T>
    inline static std::string Demangle() {
        if constexpr (IsSpecialization<T, std::vector>::value) {
            return TypeInfo<T, std::vector>::GetTypeName() + " list";
        }

        if constexpr (IsSpecialization<T, std::map>::value) {
            return TypeInfo<T, std::map>::GetTypeName() + " map";
        }

        return Demangle(typeid(T).name());
    }

   private:
    inline static std::map<std::string, std::string> prettyNames = {
        {typeid(std::string).name(), "string"},
        {typeid(std::map<std::string, std::string>).name(), "argument map"},
        {typeid(std::vector<int>).name(), "int list"},
        {typeid(std::vector<double>).name(), "double list"},
        {typeid(std::vector<std::string>).name(), "string list"},
        {typeid(std::filesystem::path).name(), "file path or url"},
    };
};
}  // namespace cppParser

#endif  // ABLATELIBRARY_DEMANGLER_H
