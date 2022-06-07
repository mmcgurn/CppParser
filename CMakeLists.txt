cmake_minimum_required(VERSION 3.14)

# Create the new project
project(CppParser VERSION 0.2.0)

# Set the standards
set(CMAKE_CXX_STANDARD 17)

# Create the project
add_library(cppParserLibrary)

# FetContent will allow downloading of other libraries
include(FetchContent)

# Load the the yamlLibrary
# turn off yaml-cpp options
set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "" FORCE)
set(YAML_CPP_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
        yaml-cpp
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
        GIT_TAG yaml-cpp-0.7.0
)
FetchContent_MakeAvailable(yaml-cpp)

# Add required warnings for projects
function(default_target_compile_options target)
    if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
        target_compile_options(${target} PUBLIC $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>: -Werror -Wall -Wwrite-strings -Wno-strict-aliasing -Wno-unknown-pragmas -Wsign-compare -fstack-protector -fno-stack-check >)
    endif()
endfunction()
default_target_compile_options(cppParserLibrary)

# Load in the source code
add_subdirectory(src)
target_link_libraries(cppParserLibrary PUBLIC yaml-cpp)

# Add a library that can be used for testing
add_library(cppParserTestLibrary INTERFACE)
target_include_directories(cppParserTestLibrary INTERFACE ${CMAKE_CURRENT_LIST_DIR}/tests/include)

# Check if we should enable testing options
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)
    include(CTest)

    # Include the format command only if we are the main project
    include(clangFormatter.cmake)

    # Setup and configure testing
    if(BUILD_TESTING)
        enable_testing()
        include(GoogleTest)
        add_subdirectory(tests)
    endif()
endif()


