#include <memory>
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mockFactory.hpp"
#include "registrar.hpp"

using ::testing::AtLeast;

namespace cppParserTesting {

using namespace cppParser;

class FactoryMockClass1 {};

TEST(FactoryTests, GetShouldReturnNullPtrWhenOptional) {
    // arrange

    cppParser::Registrar<FactoryMockClass1>::Register<FactoryMockClass1>(true, "FactoryMockClass1", "this is a simple mock class");
    auto mockFactory = std::make_shared<MockFactory>();
    EXPECT_CALL(*mockFactory, Contains(std::string("input123"))).Times(::testing::Exactly(1)).WillOnce(::testing::Return(false));

    // act
    auto argument = ArgumentIdentifier<FactoryMockClass1>{.inputName = "input123", .optional = true};
    auto result = std::dynamic_pointer_cast<Factory>(mockFactory)->Get(argument);

    // assert
    ASSERT_TRUE(result == nullptr);
}

TEST(FactoryTests, ShouldReturnEmptyListWhenOptional) {
    // arrange

    cppParser::Registrar<FactoryMockClass1>::Register<FactoryMockClass1>(true, "FactoryMockClass1", "this is a simple mock class");
    auto mockFactory = std::make_shared<MockFactory>();
    EXPECT_CALL(*mockFactory, Contains(std::string("input123"))).Times(::testing::Exactly(1)).WillOnce(::testing::Return(false));

    // act
    auto argument = ArgumentIdentifier<std::vector<FactoryMockClass1>>{.inputName = "input123", .optional = true};
    auto result = std::dynamic_pointer_cast<Factory>(mockFactory)->Get(argument);

    // assert
    ASSERT_TRUE(result.empty());
}

TEST(FactoryTests, ShouldReturnEmptyMapWhenOptional) {
    // arrange

    cppParser::Registrar<FactoryMockClass1>::Register<FactoryMockClass1>(true, "FactoryMockClass1", "this is a simple mock class");
    auto mockFactory = std::make_shared<MockFactory>();
    EXPECT_CALL(*mockFactory, Contains(std::string("input123"))).Times(::testing::Exactly(1)).WillOnce(::testing::Return(false));

    // act
    auto argument = ArgumentIdentifier<std::map<std::string, FactoryMockClass1>>{.inputName = "input123", .optional = true};
    auto result = std::dynamic_pointer_cast<Factory>(mockFactory)->Get(argument);

    // assert
    ASSERT_TRUE(result.empty());
}

TEST(FactoryTests, GetByNameShouldCallGetWithCorrectArguments) {
    // arrange
    MockFactory mockFactory;
    EXPECT_CALL(mockFactory, Get(ArgumentIdentifier<std::string>{.inputName = "input123"})).Times(::testing::Exactly(1)).WillOnce(::testing::Return("result 123"));

    // act
    auto result = mockFactory.GetByName<std::string>("input123");

    // assert
    ASSERT_EQ("result 123", result);
}

TEST(FactoryTests, GetByNameShouldReturnCorrectValue) {
    // arrange
    MockFactory mockFactory;
    EXPECT_CALL(mockFactory, Contains("input123")).Times(::testing::Exactly(1)).WillOnce(::testing::Return(true));
    EXPECT_CALL(mockFactory, Get(ArgumentIdentifier<std::string>{.inputName = "input123"})).Times(::testing::Exactly(1)).WillOnce(::testing::Return("result 123"));

    // act
    auto result = mockFactory.GetByName<std::string, std::string>("input123", "default 123");

    // assert
    ASSERT_EQ("result 123", result);
}

TEST(FactoryTests, GetByNameShouldReturnDefaultValue) {
    // arrange
    MockFactory mockFactory;
    EXPECT_CALL(mockFactory, Contains("input123")).Times(::testing::Exactly(1)).WillOnce(::testing::Return(false));

    // act
    auto result = mockFactory.GetByName<std::string>("input123", std::string("default 123"));

    // assert
    ASSERT_EQ("default 123", result);
}

class DefaultMockClass {
   public:
    std::string name;
    DefaultMockClass(std::string name) : name(name){};
};

TEST(FactoryTests, GetByNameShouldReturnDefaultValueClass) {
    // arrange
    MockFactory mockFactory;
    EXPECT_CALL(mockFactory, Contains("input123")).Times(::testing::Exactly(1)).WillOnce(::testing::Return(false));

    // act
    auto result = mockFactory.GetByName<DefaultMockClass>("input123", std::make_shared<DefaultMockClass>("default 123"));

    // assert
    ASSERT_EQ("default 123", result->name);
}

TEST(FactoryTests, GetByNameShouldReturnDefaultValueWithList) {
    // arrange
    MockFactory mockFactory;
    EXPECT_CALL(mockFactory, Contains("input123")).Times(::testing::Exactly(1)).WillOnce(::testing::Return(false));

    // act
    auto result = mockFactory.GetByName<std::vector<DefaultMockClass>>("input123", std::vector<std::shared_ptr<DefaultMockClass>>{std::make_shared<DefaultMockClass>("default 123")});

    // assert
    ASSERT_EQ(1, result.size());
    ASSERT_EQ("default 123", result[0]->name);
}

TEST(FactoryTests, GetByNameShouldReturnDefaultValueWithEmptyList) {
    // arrange
    MockFactory mockFactory;
    EXPECT_CALL(mockFactory, Contains("input123")).Times(::testing::Exactly(1)).WillOnce(::testing::Return(false));

    // act
    auto result = mockFactory.GetByName<std::vector<DefaultMockClass>>("input123", std::vector<std::shared_ptr<DefaultMockClass>>());

    // assert
    ASSERT_EQ(0, result.size());
}

enum class TestEnum { VECTOR, COMPONENT, DEFAULT };

std::istream& operator>>(std::istream& is, TestEnum& v) {
    std::string enumString;
    is >> enumString;

    if (enumString == "vector") {
        v = TestEnum::VECTOR;
    } else if (enumString == "component") {
        v = TestEnum::COMPONENT;
    } else {
        v = TestEnum::DEFAULT;
    }
    return is;
}

TEST(FactoryTests, ShouldReturnEnumFromString) {
    // arrange

    cppParser::Registrar<FactoryMockClass1>::Register<FactoryMockClass1>(true, "FactoryMockClass1", "this is a simple mock class");
    auto mockFactory = std::make_shared<MockFactory>();
    EXPECT_CALL(*mockFactory, Get(ArgumentIdentifier<std::string>{.inputName = "input123"})).Times(::testing::Exactly(1)).WillOnce(::testing::Return("component"));

    // act
    auto argument = ArgumentIdentifier<EnumWrapper<TestEnum>>{.inputName = "input123", .optional = false};
    auto result = std::dynamic_pointer_cast<Factory>(mockFactory)->Get(argument);

    // assert
    ASSERT_EQ(result, TestEnum::COMPONENT);
}

TEST(FactoryTests, ShouldReturnDefaultEnumValueWhenOptional) {
    // arrange

    cppParser::Registrar<FactoryMockClass1>::Register<FactoryMockClass1>(true, "FactoryMockClass1", "this is a simple mock class");
    auto mockFactory = std::make_shared<MockFactory>();
    EXPECT_CALL(*mockFactory, Get(ArgumentIdentifier<std::string>{.inputName = "input123", .optional = true})).Times(::testing::Exactly(1));

    // act
    auto argument = ArgumentIdentifier<EnumWrapper<TestEnum>>{.inputName = "input123", .optional = true};
    auto result = std::dynamic_pointer_cast<Factory>(mockFactory)->Get(argument);

    // assert
    ASSERT_EQ(result, TestEnum::DEFAULT);
}

TEST(FactoryTests, ShouldReturnEnumFromStringList) {
    // arrange

    cppParser::Registrar<FactoryMockClass1>::Register<FactoryMockClass1>(true, "FactoryMockClass1", "this is a simple mock class");
    auto mockFactory = std::make_shared<MockFactory>();
    EXPECT_CALL(*mockFactory, Get(ArgumentIdentifier<std::vector<std::string>>{.inputName = "input123"}))
        .Times(::testing::Exactly(1))
        .WillOnce(::testing::Return(std::vector<std::string>{"component", "component", "vector"}));

    // act
    auto argument = ArgumentIdentifier<std::vector<EnumWrapper<TestEnum>>>{.inputName = "input123", .optional = false};
    std::vector<TestEnum> result = std::dynamic_pointer_cast<Factory>(mockFactory)->Get(argument);

    // assert
    ASSERT_EQ(result.size(), 3);
    ASSERT_EQ(result[0], TestEnum::COMPONENT);
    ASSERT_EQ(result[1], TestEnum::COMPONENT);
    ASSERT_EQ(result[2], TestEnum::VECTOR);
}

TEST(FactoryTests, ShouldGetMapOfSharedPointers) {
    // arrange
    const std::string defaultClassType = "FactoryMockClass1";
    cppParser::Registrar<FactoryMockClass1>::Register<FactoryMockClass1>(true, std::string(defaultClassType), "this is a simple mock class");
    auto mockFactory = std::make_shared<MockFactory>();

    // subChildFactory
    auto subChildFactory = std::make_shared<MockFactory>();
    EXPECT_CALL(*mockFactory, GetFactory("input123")).Times(::testing::Exactly(1)).WillOnce(::testing::Return(subChildFactory));
    EXPECT_CALL(*subChildFactory, GetKeys()).Times(::testing::Exactly(1)).WillOnce(::testing::Return(std::unordered_set<std::string>{"key1", "key2"}));

    auto factoryChild1 = std::make_shared<MockFactory>();
    EXPECT_CALL(*subChildFactory, GetFactory("key1")).Times(::testing::Exactly(1)).WillOnce(::testing::Return(factoryChild1));
    EXPECT_CALL(*factoryChild1, GetClassType()).Times(::testing::Exactly(1)).WillOnce(::testing::ReturnRef(defaultClassType));

    auto factoryChild2 = std::make_shared<MockFactory>();
    EXPECT_CALL(*subChildFactory, GetFactory("key2")).Times(::testing::Exactly(1)).WillOnce(::testing::Return(factoryChild2));
    EXPECT_CALL(*factoryChild2, GetClassType()).Times(::testing::Exactly(1)).WillOnce(::testing::ReturnRef(defaultClassType));

    // act
    auto argument = ArgumentIdentifier<std::map<std::string, FactoryMockClass1>>{.inputName = "input123", .optional = false};
    auto result = std::dynamic_pointer_cast<Factory>(mockFactory)->Get(argument);

    // assert
    ASSERT_EQ(result.size(), 2);
    ASSERT_TRUE(std::dynamic_pointer_cast<FactoryMockClass1>(result["key1"]) != nullptr);
    ASSERT_TRUE(std::dynamic_pointer_cast<FactoryMockClass1>(result["key2"]) != nullptr);
}

/**
 * Create multi level inheritance
 */
class GrandParentMockClass {
   public:
    virtual std::string GetLevel() const { return "grandparent"; }
};

class ParentMockClass : public GrandParentMockClass {
   public:
    virtual std::string GetLevel() const override { return "parent"; }
};

class ChildMockClass : public ParentMockClass {
   public:
    virtual std::string GetLevel() const override { return "child"; }
};

TEST(FactoryTests, ShouldSupportMultiLevelInheritance) {
    // arrange
    // Register grandparent as grandparent
    std::string grandparentClassName = "cppParserTesting::GrandParentMockClass";
    cppParser::Registrar<GrandParentMockClass>::Register<GrandParentMockClass>(false, std::string(grandparentClassName), "");

    // register parent as grandparent
    std::string parentClassName = "cppParserTesting::ParentMockClass";
    cppParser::Registrar<GrandParentMockClass>::Register<ParentMockClass>(false, std::string(parentClassName), "");

    // register parent as derived from grandparent
    cppParser::Registrar<GrandParentMockClass>::RegisterDerived<ParentMockClass>(false, std::string(parentClassName));

    // register child as parent
    std::string childClassName = "cppParserTesting::ChildMockClass";
    cppParser::Registrar<ParentMockClass>::Register<ChildMockClass>(false, std::string(childClassName), "");

    auto mockFactory = std::make_shared<MockFactory>();

    // setup grandparent, parent, and child factories
    auto grandparentFactory = std::make_shared<MockFactory>();
    EXPECT_CALL(*mockFactory, GetFactory("grandparentInput")).Times(::testing::AtLeast(1)).WillRepeatedly(::testing::Return(grandparentFactory));
    EXPECT_CALL(*grandparentFactory, GetClassType()).Times(::testing::AtLeast(1)).WillRepeatedly(::testing::ReturnRef(grandparentClassName));

    auto parentFactory = std::make_shared<MockFactory>();
    EXPECT_CALL(*mockFactory, GetFactory("parentFactoryInput")).Times(::testing::AnyNumber()).WillRepeatedly(::testing::Return(parentFactory));
    EXPECT_CALL(*parentFactory, GetClassType()).Times(::testing::AnyNumber()).WillRepeatedly(::testing::ReturnRef(parentClassName));

    auto childFactory = std::make_shared<MockFactory>();
    EXPECT_CALL(*mockFactory, GetFactory("childFactoryInput")).Times(::testing::AnyNumber()).WillRepeatedly(::testing::Return(childFactory));
    EXPECT_CALL(*childFactory, GetClassType()).Times(::testing::AnyNumber()).WillRepeatedly(::testing::ReturnRef(childClassName));

    // act/assert
    // Get the grandparent as a grandparent
    auto grandparentAsGrandparent = std::dynamic_pointer_cast<Factory>(mockFactory)->GetByName<GrandParentMockClass>("grandparentInput");
    ASSERT_TRUE(grandparentAsGrandparent != nullptr);
    ASSERT_EQ(grandparentAsGrandparent->GetLevel(), "grandparent");

    // Get the parent as a grandparent
    auto parentAsGrandparent = std::dynamic_pointer_cast<Factory>(mockFactory)->GetByName<GrandParentMockClass>("parentFactoryInput");
    ASSERT_TRUE(parentAsGrandparent != nullptr);
    ASSERT_EQ(parentAsGrandparent->GetLevel(), "parent");

    // Get the child as a parent
    auto childAsParent = std::dynamic_pointer_cast<Factory>(mockFactory)->GetByName<ParentMockClass>("childFactoryInput");
    ASSERT_TRUE(childAsParent != nullptr);
    ASSERT_EQ(childAsParent->GetLevel(), "child");

    // Get the child as a grandparent
    auto childAsGrandparent = std::dynamic_pointer_cast<Factory>(mockFactory)->GetByName<GrandParentMockClass>("childFactoryInput");
    ASSERT_TRUE(childAsGrandparent != nullptr);
    ASSERT_EQ(childAsGrandparent->GetLevel(), "child");
}

/**
 * Create multi level inheritance
 */
class GrandParentMockClassDefault {
   public:
    virtual std::string GetLevel() const { return "grandparent"; }
};

class ParentMockClassDefault : public GrandParentMockClassDefault {
   public:
    virtual std::string GetLevel() const override { return "parent"; }
};

class ChildMockClassDefault : public ParentMockClassDefault {
   public:
    virtual std::string GetLevel() const override { return "child"; }
};

TEST(FactoryTests, ShouldSupportMultiLevelInheritanceWithDefault) {
    // arrange
    // Register grandparent as grandparent
    std::string grandparentClassName = "cppParserTesting::GrandParentMockClass";
    cppParser::Registrar<GrandParentMockClassDefault>::Register<GrandParentMockClassDefault>(false, std::string(grandparentClassName), "");

    // register parent as grandparent
    std::string parentClassName = "cppParserTesting::ParentMockClass";
    cppParser::Registrar<GrandParentMockClassDefault>::Register<ParentMockClassDefault>(false, std::string(parentClassName), "");

    // register parent as derived from grandparent
    cppParser::Registrar<GrandParentMockClassDefault>::RegisterDerived<ParentMockClassDefault>(true, std::string(parentClassName));

    // register child as parent
    std::string childClassName = "cppParserTesting::ChildMockClass";
    cppParser::Registrar<ParentMockClassDefault>::Register<ChildMockClassDefault>(true, std::string(childClassName), "");

    auto mockFactory = std::make_shared<MockFactory>();

    // use an empty class name so that the default item is created
    std::string emptyClassName = "";

    //  parent, and child factories
    auto grandparentFactory = std::make_shared<MockFactory>();
    EXPECT_CALL(*mockFactory, GetFactory("grandparentInput")).Times(::testing::AtLeast(1)).WillRepeatedly(::testing::Return(grandparentFactory));
    EXPECT_CALL(*grandparentFactory, GetClassType()).Times(::testing::AtLeast(1)).WillRepeatedly(::testing::ReturnRef(emptyClassName));

    auto parentFactory = std::make_shared<MockFactory>();
    EXPECT_CALL(*mockFactory, GetFactory("parentFactoryInput")).Times(::testing::AnyNumber()).WillRepeatedly(::testing::Return(parentFactory));
    EXPECT_CALL(*parentFactory, GetClassType()).Times(::testing::AnyNumber()).WillRepeatedly(::testing::ReturnRef(emptyClassName));

    auto childFactory = std::make_shared<MockFactory>();
    EXPECT_CALL(*mockFactory, GetFactory("childFactoryInput")).Times(::testing::AnyNumber()).WillRepeatedly(::testing::Return(childFactory));
    EXPECT_CALL(*childFactory, GetClassType()).Times(::testing::AnyNumber()).WillRepeatedly(::testing::ReturnRef(emptyClassName));

    // act/assert
    // Get the grandparent as a grandparent, this should also default as a child
    auto grandparent = std::dynamic_pointer_cast<Factory>(mockFactory)->GetByName<GrandParentMockClassDefault>("grandparentInput");
    ASSERT_TRUE(grandparent != nullptr);
    ASSERT_EQ(grandparent->GetLevel(), "child");

    // Get the parent as a grandparent, this should also default as a child
    auto parentAsGrandparent = std::dynamic_pointer_cast<Factory>(mockFactory)->GetByName<GrandParentMockClassDefault>("parentFactoryInput");
    ASSERT_TRUE(parentAsGrandparent != nullptr);
    ASSERT_EQ(parentAsGrandparent->GetLevel(), "child");

    // Get the child as a parent, this should also default as a child
    auto childAsParent = std::dynamic_pointer_cast<Factory>(mockFactory)->GetByName<ParentMockClassDefault>("childFactoryInput");
    ASSERT_TRUE(childAsParent != nullptr);
    ASSERT_EQ(childAsParent->GetLevel(), "child");

    // Get the child as a grandparent, this should also default as a child
    auto childAsGrandparent = std::dynamic_pointer_cast<Factory>(mockFactory)->GetByName<GrandParentMockClassDefault>("childFactoryInput");
    ASSERT_TRUE(childAsGrandparent != nullptr);
    ASSERT_EQ(childAsGrandparent->GetLevel(), "child");
}

}  // namespace cppParserTesting