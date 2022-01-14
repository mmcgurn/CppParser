#include <fstream>
#include <memory>
#include <sstream>
#include "gtest/gtest.h"
#include "registrar.hpp"
#include "yamlParser.hpp"

namespace cppParserTesting {

using namespace cppParser;
namespace fs = std::filesystem;

TEST(YamlParserTests, ShouldCreateFromString) {
    // arrange
    std::string yaml = "---\n item: \"im a string!\"";

    // act
    auto yamlParser = std::make_shared<YamlParser>(yaml);

    // assert
    ASSERT_EQ("im a string!", yamlParser->Get(ArgumentIdentifier<std::string>{"item"}));
    ASSERT_EQ("", yamlParser->GetClassType());
}

TEST(YamlParserTests, ShouldCreateFromFile) {
    // arrange
    fs::path tempPath = fs::temp_directory_path();
    tempPath /= "tempFile.yaml";

    std::ofstream ofs(tempPath);
    ofs << "---" << std::endl;
    ofs << " item: \"im a string!\"";
    ofs.close();

    // act
    auto yamlParser = std::make_shared<YamlParser>(tempPath);

    // assert
    ASSERT_EQ("im a string!", yamlParser->Get(ArgumentIdentifier<std::string>{"item"}));
    ASSERT_EQ("", yamlParser->GetClassType());

    // cleanup
    fs::remove(tempPath);
}

TEST(YamlParserTests, ShouldParseStrings) {
    // arrange
    std::stringstream yaml;
    yaml << "---" << std::endl;
    yaml << " item: im_a_string " << std::endl;
    yaml << " item 2: im a string " << std::endl;
    yaml << " item3: \"im a string \" " << std::endl;

    // act
    auto yamlParser = std::make_shared<YamlParser>(yaml.str());

    // assert
    ASSERT_EQ("im_a_string", yamlParser->Get(ArgumentIdentifier<std::string>{"item"}));
    ASSERT_EQ("im a string", yamlParser->Get(ArgumentIdentifier<std::string>{"item 2"}));
    ASSERT_EQ("im a string ", yamlParser->Get(ArgumentIdentifier<std::string>{"item3"}));
    ASSERT_EQ("", yamlParser->GetClassType());
}

TEST(YamlParserTests, ShouldThrowErrorForMissingString) {
    // arrange
    std::stringstream yaml;
    yaml << "---" << std::endl;
    yaml << " item: im_a_string " << std::endl;

    // act
    auto yamlParser = std::make_shared<YamlParser>(yaml.str());

    // assert
    ASSERT_THROW(yamlParser->Get(ArgumentIdentifier<std::string>{"itemNotThere"}), std::invalid_argument);
}

TEST(YamlParserTests, ShouldReturnCorrectStringForOptionalValue) {
    // arrange
    std::stringstream yaml;
    yaml << "---" << std::endl;
    yaml << " item: im_a_string " << std::endl;
    std::string emptyString = {};

    // act
    auto yamlParser = std::make_shared<YamlParser>(yaml.str());
    // assert
    ASSERT_EQ("im_a_string", yamlParser->Get(ArgumentIdentifier<std::string>{.inputName = "item", .optional = true}));
    ASSERT_EQ(emptyString, yamlParser->Get(ArgumentIdentifier<std::string>{.inputName = "item 2", .optional = true}));
    ASSERT_EQ("", yamlParser->GetClassType());
}

TEST(YamlParserTests, ShouldParseInts) {
    // arrange
    std::stringstream yaml;
    yaml << "---" << std::endl;
    yaml << " item: 22" << std::endl;
    yaml << " item 2: 1 " << std::endl;
    yaml << " item3: \"3 \" " << std::endl;
    yaml << " item4: \"not an int \" " << std::endl;

    // act
    auto yamlParser = std::make_shared<YamlParser>(yaml.str());

    // assert
    ASSERT_EQ(22, yamlParser->Get(ArgumentIdentifier<int>{"item"}));
    ASSERT_EQ(1, yamlParser->Get(ArgumentIdentifier<int>{"item 2"}));
    ASSERT_EQ(3, yamlParser->Get(ArgumentIdentifier<int>{"item3"}));
    ASSERT_THROW(yamlParser->Get(ArgumentIdentifier<int>{"item4"}), YAML::BadConversion);
}

TEST(YamlParserTests, ShouldParseDouble) {
    // arrange
    std::stringstream yaml;
    yaml << "---" << std::endl;
    yaml << " item: 22.3" << std::endl;
    yaml << " item 2: 1 " << std::endl;
    yaml << " item3: \"3.3 \" " << std::endl;
    yaml << " item4: \"not a double \" " << std::endl;

    // act
    auto yamlParser = std::make_shared<YamlParser>(yaml.str());

    // assert
    ASSERT_EQ(22.3, yamlParser->Get(ArgumentIdentifier<double>{"item"}));
    ASSERT_EQ(1, yamlParser->Get(ArgumentIdentifier<double>{"item 2"}));
    ASSERT_EQ(3.3, yamlParser->Get(ArgumentIdentifier<double>{"item3"}));
    ASSERT_THROW(yamlParser->Get(ArgumentIdentifier<double>{"item4"}), YAML::BadConversion);
}

TEST(YamlParserTests, ShouldThrowErrorForMissingInt) {
    // arrange
    std::stringstream yaml;
    yaml << "---" << std::endl;
    yaml << " item: im_a_string " << std::endl;

    // act
    auto yamlParser = std::make_shared<YamlParser>(yaml.str());

    // assert
    ASSERT_THROW(yamlParser->Get(ArgumentIdentifier<std::string>{"itemNotThere"}), std::invalid_argument);
}

TEST(YamlParserTests, ShouldReturnCorrectInForOptionalValue) {
    // arrange
    std::stringstream yaml;
    yaml << "---" << std::endl;
    yaml << " item: 22" << std::endl;
    int defaultValue = {};

    // act
    auto yamlParser = std::make_shared<YamlParser>(yaml.str());

    // assert
    ASSERT_EQ(22, yamlParser->Get(ArgumentIdentifier<int>{.inputName = "item", .optional = true}));
    ASSERT_EQ(defaultValue, yamlParser->Get(ArgumentIdentifier<int>{.inputName = "item 2", .optional = true}));
}

TEST(YamlParserTests, ShouldParseBools) {
    // arrange
    std::stringstream yaml;
    yaml << "---" << std::endl;
    yaml << " item: true" << std::endl;
    yaml << " item 2: False " << std::endl;
    yaml << " item3: false " << std::endl;
    yaml << " item4: \"truafeae \" " << std::endl;
    yaml << " item5: True " << std::endl;
    yaml << " item6: true " << std::endl;

    // act
    auto yamlParser = std::make_shared<YamlParser>(yaml.str());

    // assert
    ASSERT_EQ(true, yamlParser->Get(ArgumentIdentifier<bool>{"item"}));
    ASSERT_EQ(false, yamlParser->Get(ArgumentIdentifier<bool>{"item 2"}));
    ASSERT_EQ(false, yamlParser->Get(ArgumentIdentifier<bool>{"item3"}));
    ASSERT_THROW(yamlParser->Get(ArgumentIdentifier<bool>{"item4"}), YAML::BadConversion);
    ASSERT_EQ(true, yamlParser->Get(ArgumentIdentifier<bool>{"item5"}));
    ASSERT_EQ(true, yamlParser->Get(ArgumentIdentifier<bool>{"item6"}));
}

TEST(YamlParserTests, ShouldThrowErrorForMissingBool) {
    // arrange
    std::stringstream yaml;
    yaml << "---" << std::endl;
    yaml << " item: im_a_string " << std::endl;

    // act
    auto yamlParser = std::make_shared<YamlParser>(yaml.str());

    // assert
    ASSERT_THROW(yamlParser->Get(ArgumentIdentifier<bool>{"itemNotThere"}), std::invalid_argument);
}

TEST(YamlParserTests, ShouldReturnCorrectBoolForOptionalValue) {
    // arrange
    std::stringstream yaml;
    yaml << "---" << std::endl;
    yaml << " item: true" << std::endl;
    bool defaultValue = {};

    // act
    auto yamlParser = std::make_shared<YamlParser>(yaml.str());

    // assert
    ASSERT_EQ(true, yamlParser->Get(ArgumentIdentifier<bool>{.inputName = "item", .optional = true}));
    ASSERT_EQ(defaultValue, yamlParser->Get(ArgumentIdentifier<int>{.inputName = "item 2", .optional = true}));
}

TEST(YamlParserTests, ShouldParseSubFactory) {
    // arrange
    std::stringstream yaml;
    yaml << "---" << std::endl;
    yaml << " item: 22" << std::endl;
    yaml << " item 2: " << std::endl;
    yaml << "   child1: 12  " << std::endl;
    yaml << "   child2: im a string " << std::endl;

    // act
    auto yamlParser = std::make_shared<YamlParser>(yaml.str());

    auto child = yamlParser->GetFactory("item 2");

    // assert
    ASSERT_EQ(12, child->Get(ArgumentIdentifier<int>{"child1"}));
    ASSERT_EQ("im a string", child->Get(ArgumentIdentifier<std::string>{"child2"}));
    ASSERT_EQ("", child->GetClassType());
}

TEST(YamlParserTests, ShouldParseSubFactoryWithTag) {
    // arrange
    std::stringstream yaml;
    yaml << "---" << std::endl;
    yaml << " item: 22" << std::endl;
    yaml << " item 2: !ablate::info::green" << std::endl;
    yaml << "   child1: 12  " << std::endl;
    yaml << "   child2: im a string " << std::endl;

    // act
    auto yamlParser = std::make_shared<YamlParser>(yaml.str());

    auto child = yamlParser->GetFactory("item 2");

    // assert
    ASSERT_EQ(12, child->Get(ArgumentIdentifier<int>{"child1"}));
    ASSERT_EQ("im a string", child->Get(ArgumentIdentifier<std::string>{"child2"}));
    ASSERT_EQ("ablate::info::green", child->GetClassType());
}

TEST(YamlParserTests, ShouldThrowErrorForMissingChild) {
    // arrange
    std::stringstream yaml;
    yaml << "---" << std::endl;
    yaml << " item: 22" << std::endl;
    yaml << " item 2: !ablate::info::green" << std::endl;
    yaml << "   child1: 12  " << std::endl;
    yaml << "   child2: im a string " << std::endl;

    // act
    auto yamlParser = std::make_shared<YamlParser>(yaml.str());

    // assert
    ASSERT_THROW(yamlParser->GetFactory("item 3"), std::invalid_argument);
}

TEST(YamlParserTests, ShouldOnlyCreateOneChildInstance) {
    // arrange
    std::stringstream yaml;
    yaml << "---" << std::endl;
    yaml << " item: 22" << std::endl;
    yaml << " item 2: !ablate::info::green" << std::endl;
    yaml << "   child1: 12  " << std::endl;
    yaml << "   child2: im a string " << std::endl;

    auto yamlParser = std::make_shared<YamlParser>(yaml.str());

    // act
    auto childA = yamlParser->GetFactory("item 2");
    auto childB = yamlParser->GetFactory("item 2");

    // assert
    ASSERT_EQ(childA, childB);
}

TEST(YamlParserTests, ShouldReportUnusedChildren) {
    // arrange
    std::stringstream yaml;
    yaml << "---" << std::endl;
    yaml << " item1: 22" << std::endl;
    yaml << " item2: !ablate::info::green" << std::endl;
    yaml << "   child1: 12  " << std::endl;
    yaml << "   child2: im a string " << std::endl;
    yaml << " item3:" << std::endl;
    yaml << "   child1: im a string " << std::endl;
    yaml << "   child2: im a string " << std::endl;
    yaml << " item4: 24" << std::endl;
    yaml << " item5:" << std::endl;
    yaml << "   child1: " << std::endl;
    yaml << "     childchild1: 1 " << std::endl;
    yaml << "     childchild2: 1 " << std::endl;
    yaml << "   child2: im a string " << std::endl;
    yaml << " item6:" << std::endl;
    yaml << "   -           " << std::endl;
    yaml << "      child1: 1 " << std::endl;
    yaml << "      child2: 2" << std::endl;
    yaml << "   -           " << std::endl;
    yaml << "      child1: 3" << std::endl;
    yaml << "      child2: 4" << std::endl;
    yaml << " item7:" << std::endl;
    yaml << "     childchild1: 1 " << std::endl;
    yaml << "     childchild2: 2 " << std::endl;

    auto yamlParser = std::make_shared<YamlParser>(yaml.str());

    // act
    yamlParser->Get(ArgumentIdentifier<int>{"item1"});
    yamlParser->GetFactory("item2")->Get(ArgumentIdentifier<std::string>{"child1"});
    yamlParser->GetFactory("item5")->GetFactory("child1")->Get(ArgumentIdentifier<int>{"childchild1"});
    yamlParser->GetFactorySequence("item6")[0]->Get(ArgumentIdentifier<int>{"child2"});
    yamlParser->GetFactory("item7")->GetFactory("")->Get(ArgumentIdentifier<int>{"childchild1"});

    // assert
    auto unusedValues = yamlParser->GetUnusedValues();
    ASSERT_EQ(9, unusedValues.size());
    ASSERT_EQ("root/item3", unusedValues[0]);
    ASSERT_EQ("root/item4", unusedValues[1]);
    ASSERT_EQ("root/item2/child2", unusedValues[2]);
    ASSERT_EQ("root/item5/child2", unusedValues[3]);
    ASSERT_EQ("root/item5/child1/childchild2", unusedValues[4]);
    ASSERT_EQ("root/item6/0/child1", unusedValues[5]);
    ASSERT_EQ("root/item6/1/child1", unusedValues[6]);
    ASSERT_EQ("root/item6/1/child2", unusedValues[7]);
    ASSERT_EQ("root/item7/childchild2", unusedValues[8]);
}

TEST(YamlParserTests, ShouldGetListOfStrings) {
    // arrange
    std::stringstream yaml;
    yaml << "---" << std::endl;
    yaml << " item1: 22" << std::endl;
    yaml << " item2:" << std::endl;
    yaml << "   - string 1  " << std::endl;
    yaml << "   - string 2 " << std::endl;
    yaml << "   - string 3 " << std::endl;

    auto yamlParser = std::make_shared<YamlParser>(yaml.str());

    // act
    auto list = yamlParser->Get(ArgumentIdentifier<std::vector<std::string>>{"item2"});

    // assert
    std::vector<std::string> expectedValues = {"string 1", "string 2", "string 3"};
    ASSERT_EQ(list, expectedValues);
}

TEST(YamlParserTests, ShouldGetCorrectValueForOptionalListOfStrings) {
    // arrange
    std::stringstream yaml;
    yaml << "---" << std::endl;
    yaml << " item1: 22" << std::endl;

    auto yamlParser = std::make_shared<YamlParser>(yaml.str());

    // act
    auto list = yamlParser->Get(ArgumentIdentifier<std::vector<std::string>>{.inputName = "item2", .optional = true});

    // assert
    std::vector<std::string> expectedValues = {};
    ASSERT_EQ(list, expectedValues);
}

TEST(YamlParserTests, ShouldGetListOfInts) {
    // arrange
    std::stringstream yaml;
    yaml << "---" << std::endl;
    yaml << " item1: 22" << std::endl;
    yaml << " item2:" << std::endl;
    yaml << "   - 1  " << std::endl;
    yaml << "   - 2 " << std::endl;
    yaml << "   - 3 " << std::endl;
    yaml << " item3: [4, 5, 6]" << std::endl;

    auto yamlParser = std::make_shared<YamlParser>(yaml.str());

    // act
    auto list1 = yamlParser->Get(ArgumentIdentifier<std::vector<int>>{"item2"});
    auto list2 = yamlParser->Get(ArgumentIdentifier<std::vector<int>>{"item3"});

    // assert
    std::vector<int> expectedValues1 = {1, 2, 3};
    ASSERT_EQ(list1, expectedValues1);
    std::vector<int> expectedValues2 = {4, 5, 6};
    ASSERT_EQ(list2, expectedValues2);
}

TEST(YamlParserTests, ShouldGetCorrectValueForOptionalListOfInt) {
    // arrange
    std::stringstream yaml;
    yaml << "---" << std::endl;
    yaml << " item1: 22" << std::endl;

    auto yamlParser = std::make_shared<YamlParser>(yaml.str());

    // act
    auto list = yamlParser->Get(ArgumentIdentifier<std::vector<int>>{.inputName = "item2", .optional = true});

    // assert
    std::vector<int> expectedValues = {};
    ASSERT_EQ(list, expectedValues);
}

TEST(YamlParserTests, ShouldGetListOfDouble) {
    // arrange
    std::stringstream yaml;
    yaml << "---" << std::endl;
    yaml << " item1: 22" << std::endl;
    yaml << " item2:" << std::endl;
    yaml << "   - 1.1  " << std::endl;
    yaml << "   - 2 " << std::endl;
    yaml << "   - 3.3 " << std::endl;
    yaml << " item3: [4.4, 5, 6.6]" << std::endl;

    auto yamlParser = std::make_shared<YamlParser>(yaml.str());

    // act
    auto list1 = yamlParser->Get(ArgumentIdentifier<std::vector<double>>{"item2"});
    auto list2 = yamlParser->Get(ArgumentIdentifier<std::vector<double>>{"item3"});

    // assert
    std::vector<double> expectedValues1 = {1.1, 2, 3.3};
    ASSERT_EQ(list1, expectedValues1);
    std::vector<double> expectedValues2 = {4.4, 5, 6.6};
    ASSERT_EQ(list2, expectedValues2);
}

TEST(YamlParserTests, ShouldGetCorrectValueForOptionalListOfDouble) {
    // arrange
    std::stringstream yaml;
    yaml << "---" << std::endl;
    yaml << " item1: 22" << std::endl;

    auto yamlParser = std::make_shared<YamlParser>(yaml.str());

    // act
    auto list = yamlParser->Get(ArgumentIdentifier<std::vector<int>>{.inputName = "item2", .optional = true});

    // assert
    std::vector<int> expectedValues = {};
    ASSERT_EQ(list, expectedValues);
}

TEST(YamlParserTests, ShouldGetMapOfStrings) {
    // arrange
    std::stringstream yaml;
    yaml << "---" << std::endl;
    yaml << " item1: 22" << std::endl;
    yaml << " item2:" << std::endl;
    yaml << "   string1: 1  " << std::endl;
    yaml << "   string2: 2 " << std::endl;
    yaml << "   string3: 3 " << std::endl;

    auto yamlParser = std::make_shared<YamlParser>(yaml.str());

    // act
    auto map = yamlParser->Get(ArgumentIdentifier<std::map<std::string, std::string>>{"item2"});

    // assert
    std::map<std::string, std::string> expectedValues = {{"string1", "1"}, {"string2", "2"}, {"string3", "3"}};
    ASSERT_EQ(map, expectedValues);
}

TEST(YamlParserTests, ShouldGetCorrectValueForOptionalMap) {
    // arrange
    std::stringstream yaml;
    yaml << "---" << std::endl;
    yaml << " item1: 22" << std::endl;

    auto yamlParser = std::make_shared<YamlParser>(yaml.str());

    // act
    auto map = yamlParser->Get(ArgumentIdentifier<std::map<std::string, std::string>>{.inputName = "item2", .optional = true});

    // assert
    std::map<std::string, std::string> expectedValues = {};
    ASSERT_EQ(map, expectedValues);
}

TEST(YamlParserTests, ShouldGetListOfFactories) {
    // arrange
    std::stringstream yaml;
    yaml << "---" << std::endl;
    yaml << " item1: 22" << std::endl;
    yaml << " item2:" << std::endl;
    yaml << "   -           " << std::endl;
    yaml << "      child1: 1 " << std::endl;
    yaml << "      child2: 2" << std::endl;
    yaml << "   - !ablate::info::green " << std::endl;
    yaml << "      child1: 3" << std::endl;
    yaml << "      child2: 4" << std::endl;

    auto yamlParser = std::make_shared<YamlParser>(yaml.str());

    // act
    auto list = yamlParser->GetFactorySequence("item2");

    // assert
    ASSERT_EQ(list[0]->Get(ArgumentIdentifier<int>{"child1"}), 1);
    ASSERT_EQ(list[0]->Get(ArgumentIdentifier<int>{"child2"}), 2);
    ASSERT_EQ(list[0]->GetClassType(), "");
    ASSERT_EQ(list[1]->Get(ArgumentIdentifier<int>{"child1"}), 3);
    ASSERT_EQ(list[1]->Get(ArgumentIdentifier<int>{"child2"}), 4);
    ASSERT_EQ(list[1]->GetClassType(), "ablate::info::green");
}

TEST(YamlParserTests, ShouldGetListOfKeys) {
    // arrange
    std::stringstream yaml;
    yaml << "---" << std::endl;
    yaml << " item1: 22" << std::endl;
    yaml << " item2:" << std::endl;
    yaml << "      child1: 1 " << std::endl;
    yaml << "      child2: 2" << std::endl;

    auto yamlParser = std::make_shared<YamlParser>(yaml.str());

    // act
    auto keysRoot = yamlParser->GetKeys();
    auto childKeys = yamlParser->GetFactory("item2")->GetKeys();

    // assert
    ASSERT_EQ(keysRoot.size(), 2);
    ASSERT_EQ(keysRoot.count("item1"), 1);
    ASSERT_EQ(keysRoot.count("item2"), 1);

    ASSERT_EQ(childKeys.size(), 2);
    ASSERT_EQ(childKeys.count("child1"), 1);
    ASSERT_EQ(childKeys.count("child2"), 1);
}

TEST(YamlParserTests, ShouldGetListAsString) {
    // arrange
    std::stringstream yaml;
    yaml << "---" << std::endl;
    yaml << " item1: 22" << std::endl;
    yaml << " item2:" << std::endl;
    yaml << "   - 1.1  " << std::endl;
    yaml << "   - 2 " << std::endl;
    yaml << "   - 3.3 " << std::endl;
    yaml << " item3: [4.4, 5, 6.6]" << std::endl;

    auto yamlParser = std::make_shared<YamlParser>(yaml.str());

    // act
    auto list1 = yamlParser->Get(ArgumentIdentifier<std::string>{"item2"});
    auto list2 = yamlParser->Get(ArgumentIdentifier<std::string>{"item3"});

    // assert
    std::string expectedValues1 = "1.1 2 3.3 ";
    ASSERT_EQ(list1, expectedValues1);
    std::string expectedValues2 = "4.4 5 6.6 ";
    ASSERT_EQ(list2, expectedValues2);
}

TEST(YamlParserTests, ShouldLocateLocalFile) {
    // arrange
    fs::path tmpFile = fs::temp_directory_path() / "tempFile.txt";
    {
        std::ofstream ofs(tmpFile);
        ofs << " tempFile" << std::endl;
        ofs.close();
    }

    fs::path tempYaml = fs::temp_directory_path();
    tempYaml /= "tempFile.yaml";
    {
        std::ofstream ofs(tempYaml);
        ofs << "---" << std::endl;
        ofs << " fileName: " << tmpFile << std::endl;
        ofs.close();
    }

    auto yamlParser = std::make_shared<YamlParser>(tempYaml);

    // act
    auto computedFilePath = yamlParser->Get(ArgumentIdentifier<std::filesystem::path>{"fileName"});

    // assert
    ASSERT_TRUE(std::filesystem::exists(computedFilePath));
    ASSERT_EQ(computedFilePath, tmpFile);

    // cleanup
    fs::remove(tmpFile);
    fs::remove(tempYaml);
}

TEST(YamlParserTests, ShouldReturnDefaultValueWhenOptional) {
    // arrange
    std::stringstream yaml;
    yaml << "---" << std::endl;
    yaml << " item1: 22" << std::endl;
    yaml << " item2:" << std::endl;

    auto yamlParser = std::make_shared<YamlParser>(yaml.str());

    // act
    auto computedFilePath = yamlParser->Get(ArgumentIdentifier<std::filesystem::path>{.inputName = "fileName", .optional = true});

    // assert
    ASSERT_EQ(std::string(), computedFilePath);
}

TEST(YamlParserTests, ShouldThrowErrorWhenNotOptional) {
    // arrange
    std::stringstream yaml;
    yaml << "---" << std::endl;
    yaml << " item1: 22" << std::endl;
    yaml << " item2:" << std::endl;

    auto yamlParser = std::make_shared<YamlParser>(yaml.str());

    // act
    // assert
    ASSERT_ANY_THROW(yamlParser->Get(ArgumentIdentifier<std::filesystem::path>{.inputName = "fileName", .optional = false}));
}

TEST(YamlParserTests, ShouldCheckContainsForObject) {
    // arrange
    std::stringstream yaml;
    yaml << "---" << std::endl;
    yaml << " item: " << std::endl;
    yaml << "   item1: blue " << std::endl;
    yaml << "   item2: blue " << std::endl;
    yaml << " list1: " << std::endl;
    yaml << "   - !green" << std::endl;
    yaml << "   - subList2: " << std::endl;
    yaml << "     subItem1: blue " << std::endl;
    yaml << "     subItem2: " << std::endl;

    // act
    auto yamlParser = std::make_shared<YamlParser>(yaml.str());
    auto yamlParserChild = yamlParser->GetFactory("item");
    auto yamlParserChildList = yamlParser->GetFactorySequence("list1");

    // assert
    ASSERT_TRUE(yamlParserChild->Contains("item1"));
    ASSERT_FALSE(yamlParserChild->Contains("item3"));
    ASSERT_FALSE(yamlParserChildList[0]->Contains("item3"));
    ASSERT_TRUE(yamlParserChildList[1]->Contains("subItem1"));
}

TEST(YamlParserTests, ShouldPrintToStream) {
    // arrange
    std::stringstream yaml;
    yaml << "---" << std::endl;
    yaml << "item1: 22" << std::endl;
    yaml << "item2:" << std::endl;
    yaml << "  item3: 3" << std::endl;
    yaml << "  item4: 5" << std::endl;

    auto yamlParser = std::make_shared<YamlParser>(yaml.str());

    std::stringstream outStream;

    // act
    yamlParser->Print(outStream);

    // assert
    ASSERT_EQ(outStream.str(), yaml.str());
}

TEST(YamlParserTests, ShouldAllowOverWrittenValues) {
    // arrange
    std::stringstream yaml;
    yaml << "---" << std::endl;
    yaml << "item1: 22" << std::endl;
    yaml << "item2:" << std::endl;
    yaml << "  item3: 3" << std::endl;
    yaml << "  item4: [1]" << std::endl;
    yaml << "  item5: " << std::endl;
    yaml << "    item6: {} " << std::endl;
    yaml << "item9: " << std::endl;
    yaml << "  - list1: 1" << std::endl;
    yaml << "  - list2:" << std::endl;
    yaml << "    item10: 10" << std::endl;

    auto params = std::map<std::string, std::string>{
        {"item1", "44"},
        {"item2::item4", "[3, 2]"},
        {"item2::item5::item6::item7", "77"},
        {"item9::[1]::item10", "100"},
    };

    // act
    auto yamlParser = std::make_shared<YamlParser>(yaml.str(), nullptr, params);

    // assert
    ASSERT_EQ("44", yamlParser->GetByName<std::string>("item1"));
    std::vector<double> expectedVector{3, 2};
    ASSERT_EQ(expectedVector, yamlParser->GetFactory("item2")->GetByName<std::vector<double>>("item4"));
    ASSERT_EQ("77", yamlParser->GetFactory("item2")->GetFactory("item5")->GetFactory("item6")->GetByName<std::string>("item7"));
    ASSERT_EQ("100", yamlParser->GetFactorySequence("item9")[1]->GetByName<std::string>("item10"));

    // the output should be updated
    std::stringstream yamlUpdated;
    yamlUpdated << "---" << std::endl;
    yamlUpdated << "item1: 44" << std::endl;
    yamlUpdated << "item2:" << std::endl;
    yamlUpdated << "  item3: 3" << std::endl;
    yamlUpdated << "  item4: [3, 2]" << std::endl;
    yamlUpdated << "  item5:" << std::endl;
    yamlUpdated << "    item6: {item7: 77}" << std::endl;
    yamlUpdated << "item9:" << std::endl;
    yamlUpdated << "  - list1: 1" << std::endl;
    yamlUpdated << "  - list2: ~" << std::endl;
    yamlUpdated << "    item10: 100" << std::endl;

    std::stringstream outStream;
    yamlParser->Print(outStream);
    ASSERT_EQ(outStream.str(), yamlUpdated.str());
}

TEST(YamlParserTests, ShouldSupportAliasesAndAnchorsForScalars) {
    // arrange
    std::stringstream yaml;
    yaml << "---" << std::endl;
    yaml << "item1: &anchor1  22" << std::endl;
    yaml << "item2: *anchor1" << std::endl;

    auto yamlParser = std::make_shared<YamlParser>(yaml.str());

    // act
    // assert
    ASSERT_EQ(22, yamlParser->Get(ArgumentIdentifier<int>{.inputName = "item1"}));
    ASSERT_EQ(22, yamlParser->Get(ArgumentIdentifier<int>{.inputName = "item2"}));
}

TEST(YamlParserTests, ShouldSupportAliasesAndAnchorsForMaps) {
    // arrange
    std::stringstream yaml;
    yaml << "---" << std::endl;
    yaml << "item1: &anchor1" << std::endl;
    yaml << "  item10: 3" << std::endl;
    yaml << "  item11: 55" << std::endl;
    yaml << "item2: *anchor1" << std::endl;

    auto yamlParser = std::make_shared<YamlParser>(yaml.str());

    // act
    auto factory1 = yamlParser->GetFactory("item2");
    auto factory2 = yamlParser->GetFactory("item2");

    // assert
    ASSERT_EQ(3, factory1->Get(ArgumentIdentifier<int>{.inputName = "item10"}));
    ASSERT_EQ(55, factory1->Get(ArgumentIdentifier<int>{.inputName = "item11"}));
    ASSERT_EQ(3, factory2->Get(ArgumentIdentifier<int>{.inputName = "item10"}));
    ASSERT_EQ(55, factory2->Get(ArgumentIdentifier<int>{.inputName = "item11"}));
}

TEST(YamlParserTests, ShouldReturnFactoryForScalarValues) {
    // arrange
    std::stringstream yaml;
    yaml << "---" << std::endl;
    yaml << " item: !classType123 22.3 " << std::endl;
    std::string emptyString = {};

    auto yamlParser = std::make_shared<YamlParser>(yaml.str());

    // act
    auto factory1 = yamlParser->GetFactory("item");

    // assert
    ASSERT_EQ("22.3", factory1->Get(ArgumentIdentifier<std::string>{.inputName = {}}));
    ASSERT_EQ(22.3, factory1->Get(ArgumentIdentifier<double>{.inputName = {}}));
    ASSERT_EQ("22.3", factory1->Get(ArgumentIdentifier<std::string>{.inputName = {}, .optional = true}));
    ASSERT_EQ(22.3, factory1->Get(ArgumentIdentifier<double>{.inputName = {}, .optional = true}));
    ASSERT_EQ("classType123", factory1->GetClassType());
}

TEST(YamlParserTests, ShouldNotErrorForEmptyScalarWhenOptional) {
    // arrange
    std::stringstream yaml;
    yaml << "---" << std::endl;
    yaml << " item: !classType123 22.3" << std::endl;
    std::string emptyString = {};

    auto yamlParser = std::make_shared<YamlParser>(yaml.str());

    // act
    auto factory1 = yamlParser->GetFactory("item");

    // assert
    ASSERT_EQ("", factory1->Get(ArgumentIdentifier<std::string>{.inputName = "subItem1", .optional = true}));
    ASSERT_EQ(0, factory1->Get(ArgumentIdentifier<int>{.inputName = "subItem1", .optional = true}));
    ASSERT_THROW(factory1->Get(ArgumentIdentifier<std::string>{.inputName = "subItem1", .optional = false}), std::invalid_argument);
    ASSERT_THROW(factory1->Get(ArgumentIdentifier<int>{.inputName = "subItem1", .optional = false}), std::invalid_argument);
    ASSERT_EQ("classType123", factory1->GetClassType());
}

TEST(YamlParserTests, ShouldReturnSelfScalarWhenInputNameIsEmpty) {
    // arrange
    std::stringstream yaml;
    yaml << "---" << std::endl;
    yaml << " item: !classType123 22.3" << std::endl;
    std::string emptyString = {};

    auto yamlParser = std::make_shared<YamlParser>(yaml.str());

    // act
    auto factory1 = yamlParser->GetFactory("item");

    // assert
    ASSERT_EQ("22.3", factory1->Get(ArgumentIdentifier<std::string>{.inputName = ""}));
    ASSERT_EQ("classType123", factory1->GetClassType());
}

TEST(YamlParserTests, ShouldReturnSelfFactoryWhenInputNameIsEmpty) {
    // arrange
    std::stringstream yaml;
    yaml << "---" << std::endl;
    yaml << " item: !classType123" << std::endl;
    yaml << "   subItem1: 1.0" << std::endl;
    yaml << "   subItem2: 2.0" << std::endl;

    std::string emptyString = {};

    auto yamlParser = std::make_shared<YamlParser>(yaml.str());

    // act
    auto factory1 = yamlParser->GetFactory("item");
    auto selfFactory = factory1->GetFactory("");

    // assert
    ASSERT_EQ(1.0, selfFactory->Get(ArgumentIdentifier<double>{.inputName = "subItem1"}));
    ASSERT_EQ(2.0, selfFactory->Get(ArgumentIdentifier<double>{.inputName = "subItem2"}));
    ASSERT_EQ("", selfFactory->GetClassType());
}

TEST(YamlParserTests, ShouldCompareFactories) {
    // arrange
    std::stringstream yaml;
    yaml << "---" << std::endl;
    yaml << " item1: " << std::endl;
    yaml << "   subItem1: 1.0" << std::endl;
    yaml << " item2: " << std::endl;
    yaml << "   subItem1: 1.0" << std::endl;
    yaml << " item3: &anchor1" << std::endl;
    yaml << "   subItem1: 2.0" << std::endl;
    yaml << " item4: *anchor1" << std::endl;

    auto yamlParser = std::make_shared<YamlParser>(yaml.str());

    // act
    auto factory1 = yamlParser->GetFactory("item1");
    auto factory2 = yamlParser->GetFactory("item2");
    auto factory3 = yamlParser->GetFactory("item3");
    auto factory4 = yamlParser->GetFactory("item4");

    // assert
    ASSERT_TRUE(*factory1 == *factory1);
    ASSERT_FALSE(*factory1 == *factory2);
    ASSERT_FALSE(*factory1 == *factory3);
    ASSERT_FALSE(*factory1 == *factory4);

    ASSERT_FALSE(*factory2 == *factory1);
    ASSERT_TRUE(*factory2 == *factory2);
    ASSERT_FALSE(*factory2 == *factory3);
    ASSERT_FALSE(*factory2 == *factory4);

    ASSERT_FALSE(*factory3 == *factory1);
    ASSERT_FALSE(*factory3 == *factory2);
    ASSERT_TRUE(*factory3 == *factory3);
    ASSERT_TRUE(*factory3 == *factory4);

    ASSERT_FALSE(*factory4 == *factory1);
    ASSERT_FALSE(*factory4 == *factory2);
    ASSERT_TRUE(*factory4 == *factory3);
    ASSERT_TRUE(*factory4 == *factory4);
}

class YamlMockClass1 {};

TEST(YamlParserTests, ShouldReuseInstances) {
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

    // assert
    ASSERT_TRUE(instance1);
    ASSERT_FALSE(instance1 == instance2);
    ASSERT_FALSE(instance1 == instance3);
    ASSERT_FALSE(instance1 == instance4);
    ASSERT_FALSE(instance1 == instance5);
    ASSERT_FALSE(instance1 == instance6);
    ASSERT_FALSE(instance1 == instance7);

    ASSERT_FALSE(instance2 == instance1);
    ASSERT_TRUE(instance2);
    ASSERT_FALSE(instance2 == instance3);
    ASSERT_FALSE(instance2 == instance4);
    ASSERT_FALSE(instance2 == instance5);
    ASSERT_FALSE(instance2 == instance6);
    ASSERT_FALSE(instance2 == instance7);

    ASSERT_FALSE(instance3 == instance1);
    ASSERT_FALSE(instance3 == instance2);
    ASSERT_TRUE(instance3);
    ASSERT_TRUE(instance3 == instance4);
    ASSERT_FALSE(instance3 == instance5);
    ASSERT_TRUE(instance3 == instance6);
    ASSERT_FALSE(instance3 == instance7);

    ASSERT_FALSE(instance4 == instance1);
    ASSERT_FALSE(instance4 == instance2);
    ASSERT_TRUE(instance4 == instance3);
    ASSERT_TRUE(instance4);
    ASSERT_FALSE(instance4 == instance5);
    ASSERT_TRUE(instance4 == instance6);
    ASSERT_FALSE(instance4 == instance7);

    ASSERT_FALSE(instance5 == instance1);
    ASSERT_FALSE(instance5 == instance2);
    ASSERT_FALSE(instance5 == instance3);
    ASSERT_FALSE(instance5 == instance4);
    ASSERT_TRUE(instance5);
    ASSERT_FALSE(instance5 == instance6);
    ASSERT_TRUE(instance5 == instance7);

    ASSERT_FALSE(instance6 == instance1);
    ASSERT_FALSE(instance6 == instance2);
    ASSERT_TRUE(instance6 == instance3);
    ASSERT_TRUE(instance6 == instance4);
    ASSERT_FALSE(instance6 == instance5);
    ASSERT_TRUE(instance6);
    ASSERT_FALSE(instance6 == instance7);

    ASSERT_FALSE(instance7 == instance1);
    ASSERT_FALSE(instance7 == instance2);
    ASSERT_FALSE(instance7 == instance3);
    ASSERT_FALSE(instance7 == instance4);
    ASSERT_TRUE(instance7 == instance5);
    ASSERT_FALSE(instance7 == instance6);
    ASSERT_TRUE(instance7);
}

class YamlMockClass2 {
   public:
    int testInt;

    YamlMockClass2(int testInt) : testInt(testInt) {}
};

TEST(YamlParserTests, ShouldNotReportUsedChildAsUnusedWhenUsingAnchors) {
    cppParser::Registrar<YamlMockClass2>::Register<YamlMockClass2>(true, std::string("YamlMockClass1"), "this is a simple mock class", ArgumentIdentifier<int>{.inputName = "testInt"});

    std::stringstream yaml;
    yaml << "---" << std::endl;
    yaml << " item1: &anchor" << std::endl;
    yaml << "   testInt: 1" << std::endl;
    yaml << " item2: *anchor" << std::endl;

    auto yamlParser = std::make_shared<YamlParser>(yaml.str());

    // act
    auto instance1 = yamlParser->GetByName<YamlMockClass2>("item1");
    auto instance2 = yamlParser->GetByName<YamlMockClass2>("item2");

    // assert
    auto unusedValues = yamlParser->GetUnusedValues();
    ASSERT_EQ(unusedValues.size(), 0);
}

TEST(YamlParserTests, ShouldReportSingleInstanceOfUnusedChildrenWhenUsingAnchors) {
    cppParser::Registrar<YamlMockClass2>::Register<YamlMockClass2>(true, std::string("YamlMockClass1"), "this is a simple mock class", ArgumentIdentifier<int>{.inputName = "testInt"});

    std::stringstream yaml;
    yaml << "---" << std::endl;
    yaml << " item1: &anchor" << std::endl;
    yaml << "   testInt: 1" << std::endl;
    yaml << "   testInt2: 1" << std::endl;
    yaml << " item2: *anchor" << std::endl;
    yaml << " item3: *anchor" << std::endl;

    auto yamlParser = std::make_shared<YamlParser>(yaml.str());

    // act
    auto instance1 = yamlParser->GetByName<YamlMockClass2>("item1");
    auto instance2 = yamlParser->GetByName<YamlMockClass2>("item2");

    // assert
    auto unusedValues = yamlParser->GetUnusedValues();
    ASSERT_EQ(unusedValues.size(), 2);
    ASSERT_EQ("root/item3", unusedValues[0]);
    ASSERT_EQ("root/item1/testInt2", unusedValues[1]);
}

}  // namespace cppParserTesting