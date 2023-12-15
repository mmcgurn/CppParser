#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>
#include "gtest/gtest.h"
#include "registrar.hpp"
#include "yamlParser.hpp"

int global_array[100] = {-1};
using namespace cppParser;
namespace fs = std::filesystem;

class YamlMockClass1 {};

int main(int argc, char **argv) {
    while (true) {
        // arrange
        cppParser::Registrar<YamlMockClass1>::Register<YamlMockClass1>(true, std::string("YamlMockClass1"), "this is a simple mock class");

        std::stringstream yaml;
        yaml << "---" << std::endl;
        yaml << " item1: " << std::endl;
        yaml << "   subItem1: 1.0" << std::endl;
        yaml << " item2: " << std::endl;
        yaml << "   subItem1: 1.0" << std::endl;
        yaml << " item3: &anchor1" << std::endl;
        yaml << "   subItem1: 2.0" << std::endl;
        yaml << " item4: *anchor1" << std::endl;
        yaml << " subFact1:" << std::endl;
        yaml << "   item5: &anchor2" << std::endl;
        yaml << "     subItem1: 1.0" << std::endl;
        yaml << "   item6: *anchor1" << std::endl;
        yaml << " item7: *anchor2" << std::endl;
        yaml << " itemList:" << std::endl;
        yaml << "   - #item 8" << std::endl;
        yaml << "     *anchor2" << std::endl;
        yaml << "   - #item 9" << std::endl;
        yaml << "     subItem1: 1.0" << std::endl;
        yaml << " itemMap:" << std::endl;
        yaml << "   item10: " << std::endl;
        yaml << "     subItem1: 1.0" << std::endl;
        yaml << "   item11: *anchor1" << std::endl;

        auto yamlParser = std::make_shared<YamlParser>(yaml.str());

        // act
        auto instance1 = yamlParser->GetByName<YamlMockClass1>("item1");
        auto instance2 = yamlParser->GetByName<YamlMockClass1>("item2");
        auto instance3 = yamlParser->GetByName<YamlMockClass1>("item3");
        auto instance4 = yamlParser->GetByName<YamlMockClass1>("item4");
        auto subFactory1 = yamlParser->GetFactory("subFact1");
        auto instance5 = subFactory1->GetByName<YamlMockClass1>("item5");
        auto instance6 = subFactory1->GetByName<YamlMockClass1>("item6");
        auto instance7 = yamlParser->GetByName<YamlMockClass1>("item7");

        auto itemList = yamlParser->GetByName<std::vector<YamlMockClass1>>("itemList");
        auto instance8 = itemList[0];
        auto instance9 = itemList[1];

        auto itemMap = yamlParser->GetByName<std::map<std::string, YamlMockClass1>>("itemMap");
        auto instance10 = itemMap["item10"];
        auto instance11 = itemMap["item11"];

        int *x = new int(10);
        sleep(10);
        std::cout << "it" << std::endl;
    }
    return 0;
}