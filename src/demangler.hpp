#ifndef CPPPARSER_DEMANGLER_H
#define CPPPARSER_DEMANGLER_H

#include <filesystem>
#include <map>
#include <numeric>
#include <string>
#include <vector>

namespace cppParser {
class Demangler {
   public:
    static std::string Demangle(const std::string &);

   private:
    template <typename Test, template <typename...> class Ref>
    struct IsSpecialization : std::false_type {
        static std::string GetTypeName() { return ""; }
    };

    template <template <typename...> class Ref, typename... Args>
    struct IsSpecialization<Ref<Args...>, Ref> : std::true_type {
        static std::string GetTypeName() {
            std::vector<std::string> names = {Demangle(typeid(Args).name()...)};
            return std::accumulate(names.begin() + 1, names.end(), names.front(), [](std::string x, std::string y) { return x + "," + y; });
        }
    };

   public:
    template <class T>
    inline static std::string Demangle() {
        if constexpr (IsSpecialization<T, std::vector>::value) {
            return IsSpecialization<T, std::vector>::GetTypeName() + " list";
        }

        if constexpr (IsSpecialization<T, std::map>::value) {
            return IsSpecialization<T, std::vector>::GetTypeName() + " map";
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
