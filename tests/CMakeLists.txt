# Download google test
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG master
)

FetchContent_MakeAvailable(googletest)

# Define a test exe
add_executable(cppParserTests
        factoryTests.cpp mockFactory.hpp registrarTests.cpp yamlParserTests.cpp)
target_link_libraries(cppParserTests PUBLIC gtest gmock gtest_main cppParserLibrary)
add_library(cppParserTestLibrary INTERFACE)
target_include_directories(cppParserTestLibrary INTERFACE tests)

gtest_discover_tests(cppParserTests
        # set a working directory so your project root so that you can find test data via paths relative to the project root
        WORKING_DIRECTORY ${PROJECT_DIR}
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_DIR}"
        )

# Allow public access to the header files in the directory
target_include_directories(cppParserTests PUBLIC ${CMAKE_CURRENT_LIST_DIR})
