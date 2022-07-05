#include <memory>
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mockFactory.hpp"
#include "registrar.hpp"

using ::testing::AtLeast;

namespace cppParserTesting {

using namespace cppParser;

class MockInterface {
   public:
    virtual void Test(){};
    virtual ~MockInterface() = default;
};

class MockListing : public cppParser::Listing {
   public:
    MOCK_METHOD(void, RecordListing, (ClassEntry entry));
};

class MockClass1 : public MockInterface {
   public:
    MockClass1(std::shared_ptr<Factory> factory){};
};

TEST(RegistrarTests, ShouldRegisterClassAndRecordInLog) {
    // arrange
    auto mockListing = std::make_shared<MockListing>();
    EXPECT_CALL(*mockListing, RecordListing(Listing::ClassEntry{.interface = Demangler::Demangle<MockInterface>(), .className = "mockClass1", .description = "this is a simple mock class"}))
        .Times(::testing::Exactly(1));

    Listing::ReplaceListing(mockListing);

    // act
    Registrar<MockInterface>::RegisterWithFactoryConstructor<MockClass1>(false, "mockClass1", "this is a simple mock class");

    // assert
    auto createMethod = Creator<MockInterface>::GetCreateMethod("mockClass1");
    ASSERT_TRUE(createMethod != nullptr);

    // cleanup
    Listing::ReplaceListing(nullptr);
}

class MockClass2 : public MockInterface {
   public:
    MockClass2(std::string, int, std::shared_ptr<MockInterface>){};
};

TEST(RegistrarTests, ShouldRegisterClassWithArgumentIdentifiersAndRecordInLog) {
    // arrange
    auto mockListing = std::make_shared<MockListing>();
    EXPECT_CALL(*mockListing,
                RecordListing(Listing::ClassEntry{
                    .interface = Demangler::Demangle<MockInterface>(),
                    .className = "MockClass2",
                    .description = "this is a simple mock class",
                    .arguments = {Listing::ArgumentEntry{.name = "dog", .interface = Demangler::Demangle<std::string>(), .description = "this is a string"},
                                  Listing::ArgumentEntry{.name = "cat", .interface = Demangler::Demangle<int>(), .description = "this is a int"},
                                  Listing::ArgumentEntry{.name = "bird", .interface = Demangler::Demangle<MockInterface>(), .description = "this is a shared pointer to an interface"}}}))
        .Times(::testing::Exactly(1));

    Listing::ReplaceListing(mockListing);

    // act
    cppParser::Registrar<MockInterface>::Register<MockClass2>(false,
                                                              "MockClass2",
                                                              "this is a simple mock class",
                                                              cppParser::ArgumentIdentifier<std::string>{.inputName = "dog", .description = "this is a string"},
                                                              cppParser::ArgumentIdentifier<int>{.inputName = "cat", .description = "this is a int"},
                                                              cppParser::ArgumentIdentifier<MockInterface>{.inputName = "bird", .description = "this is a shared pointer to an interface"});

    // assert
    auto createMethod = Creator<MockInterface>::GetCreateMethod("MockClass2");
    ASSERT_TRUE(createMethod != nullptr);

    // cleanup
    Listing::ReplaceListing(nullptr);
}

TEST(RegistrarTests, ShouldRegisterClassWithArgumentIdentifiersAndOptAndRecordInLog) {
    // arrange
    auto mockListing = std::make_shared<MockListing>();
    EXPECT_CALL(
        *mockListing,
        RecordListing(Listing::ClassEntry{
            .interface = Demangler::Demangle<MockInterface>(),
            .className = "MockClass2a",
            .description = "this is a simple mock class",
            .arguments = {Listing::ArgumentEntry{.name = "dog", .interface = Demangler::Demangle<std::string>(), .description = "this is a string", .optional = true},
                          Listing::ArgumentEntry{.name = "cat", .interface = Demangler::Demangle<int>(), .description = "this is a int", .optional = true},
                          Listing::ArgumentEntry{.name = "bird", .interface = Demangler::Demangle<MockInterface>(), .description = "this is a shared pointer to an interface", .optional = true}}}))
        .Times(::testing::Exactly(1));

    Listing::ReplaceListing(mockListing);

    // act
    cppParser::Registrar<MockInterface>::Register<MockClass2>(false,
                                                              "MockClass2a",
                                                              "this is a simple mock class",
                                                              cppParser::ArgumentIdentifier<std::string>{"dog", "this is a string", true},
                                                              cppParser::ArgumentIdentifier<int>{"cat", "this is a int", true},
                                                              cppParser::ArgumentIdentifier<MockInterface>{"bird", "this is a shared pointer to an interface", true});

    // assert
    auto createMethod = Creator<MockInterface>::GetCreateMethod("MockClass2a");
    ASSERT_TRUE(createMethod != nullptr);

    // cleanup
    Listing::ReplaceListing(nullptr);
}

class MockInterface4 {
   public:
    virtual ~MockInterface4() = default;
    virtual void Test(){};
};

class MockClass4 : public MockInterface4 {
   public:
    MockClass4(std::string, int, std::shared_ptr<MockInterface4>){};
};

TEST(RegistrarTests, ShouldRegisterDefaultClassWithArgumentIdentifiersAndRecordInLog) {
    // arrange
    auto mockListing = std::make_shared<MockListing>();
    EXPECT_CALL(*mockListing,
                RecordListing(Listing::ClassEntry{
                    .interface = Demangler::Demangle<MockInterface4>(),
                    .className = "MockClass4",
                    .description = "this is a simple mock class",
                    .arguments = {Listing::ArgumentEntry{.name = "dog", .interface = Demangler::Demangle<std::string>(), .description = "this is a string"},
                                  Listing::ArgumentEntry{.name = "cat", .interface = Demangler::Demangle<int>(), .description = "this is a int"},
                                  Listing::ArgumentEntry{.name = "bird", .interface = Demangler::Demangle<MockInterface4>(), .description = "this is a shared pointer to an interface"}},
                    .defaultConstructor = true}))
        .Times(::testing::Exactly(1));

    Listing::ReplaceListing(mockListing);

    // act
    cppParser::Registrar<MockInterface4>::Register<MockClass4>(true,
                                                               "MockClass4",
                                                               "this is a simple mock class",
                                                               cppParser::ArgumentIdentifier<std::string>{"dog", "this is a string"},
                                                               cppParser::ArgumentIdentifier<int>{"cat", "this is a int"},
                                                               cppParser::ArgumentIdentifier<MockInterface4>{"bird", "this is a shared pointer to an interface"});

    // assert
    auto createMethod = Creator<MockInterface4>::GetCreateMethod("MockClass4");
    ASSERT_TRUE(createMethod != nullptr);

    // cleanup
    Listing::ReplaceListing(nullptr);
}

class MockInterface5 {
   public:
    virtual ~MockInterface5() = default;
    virtual void Test(){};
};

class MockClass5a : public MockInterface5 {
   public:
    MockClass5a(std::string, int, std::shared_ptr<MockInterface5>){};
};

class MockClass5b : public MockInterface5 {
   public:
    MockClass5b(std::string, int, std::shared_ptr<MockInterface5>){};
};

TEST(RegistrarTests, ShouldNotAllowDoubleDefaultRegistar) {
    // arrange
    cppParser::Registrar<MockInterface5>::Register<MockClass5a>(true,
                                                                "MockClass5a",
                                                                "this is a simple mock class",
                                                                cppParser::ArgumentIdentifier<std::string>{"dog", "this is a string"},
                                                                cppParser::ArgumentIdentifier<int>{"cat", "this is a int"},
                                                                cppParser::ArgumentIdentifier<MockInterface5>{"bird", "this is a shared pointer to an interface"});

    // act
    // assert
    ASSERT_THROW(cppParser::Registrar<MockInterface5>::Register<MockClass5b>(true,
                                                                             "MockClass5b",
                                                                             "this is a simple mock class",
                                                                             cppParser::ArgumentIdentifier<std::string>{"dog", "this is a string"},
                                                                             cppParser::ArgumentIdentifier<int>{"cat", "this is a int"},
                                                                             cppParser::ArgumentIdentifier<MockInterface5>{"bird", "this is a shared pointer to an interface"}),
                 std::invalid_argument);
}

TEST(RegistrarTests, ShouldResolveAndCreate) {
    // arrange
    auto mockFactory = std::make_shared<MockFactory>();
    const std::string expectedClassType = "mockClass1";

    EXPECT_CALL(*mockFactory, GetClassType()).Times(::testing::Exactly(1)).WillOnce(::testing::ReturnRef(expectedClassType));

    Registrar<MockInterface>::RegisterWithFactoryConstructor<MockClass1>(false, "mockClass1", "this is a simple mock class");

    // act
    auto instance = cppParser::Creator<MockInterface>::GetCreateMethod(mockFactory->GetClassType())(mockFactory);

    // assert
    ASSERT_TRUE(instance != nullptr) << " should create an instance of the interface";
    ASSERT_TRUE(std::dynamic_pointer_cast<MockClass1>(instance) != nullptr) << " should be an instance of MockClass1";
}

TEST(RegistrarTests, ShouldCreateDefaultAndUseWhenNotSpecified) {
    // arrange
    auto mockFactory = std::make_shared<MockFactory>();
    const std::string expectedClassType = "";

    EXPECT_CALL(*mockFactory, GetClassType()).Times(::testing::Exactly(1)).WillOnce(::testing::ReturnRef(expectedClassType));

    Registrar<MockInterface>::RegisterWithFactoryConstructor<MockClass1>(true, "mockClass54", "this is a simple mock class");

    // act
    auto result = cppParser::Creator<MockInterface>::GetCreateMethod(mockFactory->GetClassType())(mockFactory);

    // assert
    ASSERT_TRUE(result != nullptr);
}

class MockClass6 : public MockInterface {
   public:
    const int a;
    const int b;

    MockClass6(int a, int b) : a(a), b(b){};
};

static std::shared_ptr<MockClass6> MakeMockClass6Function(std::shared_ptr<Factory> factory) {
    auto c = factory->GetByName<int>("c");
    return std::make_shared<MockClass6>(c * 2, c * 3);
}

TEST(RegistrarTests, ShouldRegisterFunctionForClassAndRecordInLog) {
    // arrange
    auto mockListing = std::make_shared<MockListing>();
    EXPECT_CALL(*mockListing, RecordListing(Listing::ClassEntry{.interface = Demangler::Demangle<MockInterface>(), .className = "mockClass6", .description = "this is a simple mock class"}))
        .Times(::testing::Exactly(1));

    Listing::ReplaceListing(mockListing);

    // act
    Registrar<MockInterface>::RegisterWithFactoryFunction<MockClass6>(false, "mockClass6", "this is a simple mock class", MakeMockClass6Function);

    // assert
    auto createMethod = Creator<MockInterface>::GetCreateMethod("mockClass6");
    ASSERT_TRUE(createMethod != nullptr);

    // cleanup
    Listing::ReplaceListing(nullptr);
}

}  // namespace cppParserTesting