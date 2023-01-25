#ifndef CPPPARSERTESTS_MOCKFACTORY_HPP
#define CPPPARSERTESTS_MOCKFACTORY_HPP

#include <map>
#include <string>
#include "factory.hpp"
#include "gmock/gmock.h"

using namespace cppParser;

namespace cppParserTesting {

MATCHER_P(NameIs, name, "") { return (arg.inputName == name); }

class MockFactory : public cppParser::Factory {
   public:
    MOCK_METHOD(std::shared_ptr<Factory>, GetFactory, (const std::string& name), (override, const));
    MOCK_METHOD(std::vector<std::shared_ptr<cppParser::Factory>>, GetFactorySequence, (const std::string& name), (override, const));
    MOCK_METHOD(const std::string&, GetClassType, (), (override, const));
    MOCK_METHOD(int, Get, (const ArgumentIdentifier<int>&), (override, const));
    MOCK_METHOD(double, Get, (const ArgumentIdentifier<double>&), (override, const));
    MOCK_METHOD(std::string, Get, (const ArgumentIdentifier<std::string>&), (override, const));

    MOCK_METHOD(std::vector<int>, Get, (const ArgumentIdentifier<std::vector<int>>&), (override, const));
    MOCK_METHOD(std::vector<double>, Get, (const ArgumentIdentifier<std::vector<double>>&), (override, const));
    MOCK_METHOD(std::vector<std::string>, Get, (const ArgumentIdentifier<std::vector<std::string>>&), (override, const));

    MOCK_METHOD(std::vector<std::vector<int>>, Get, (const ArgumentIdentifier<std::vector<std::vector<int>>>&), (override, const));
    MOCK_METHOD(std::vector<std::vector<double>>, Get, (const ArgumentIdentifier<std::vector<std::vector<double>>>&), (override, const));
    MOCK_METHOD(std::vector<std::vector<std::string>>, Get, (const ArgumentIdentifier<std::vector<std::vector<std::string>>>&), (override, const));

    MOCK_METHOD(bool, Get, (const ArgumentIdentifier<bool>&), (override, const));
    MOCK_METHOD((std::map<std::string, std::string>), Get, ((const ArgumentIdentifier<std::map<std::string, std::string>>&)), (override, const));
    MOCK_METHOD(bool, Contains, (const std::string& name), (override, const));
    MOCK_METHOD(std::unordered_set<std::string>, GetKeys, (), (const, override));
    MOCK_METHOD(bool, SameFactory, (const Factory& otherFactory), (const, override));
    MOCK_METHOD(std::vector<std::string>, GetUnusedValues, (), (const, override));
    MOCK_METHOD(void, MarkAllUsed, (), (const, override));
};
}  // namespace cppParserTesting

#endif  // ABLATELIBRARY_MOCKFACTORY_HPP
