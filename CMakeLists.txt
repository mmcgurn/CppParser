cmake_minimum_required(VERSION 3.18.4)

# Create the new project
project(CppParser VERSION 0.2.2)

# Set the standards
set(CMAKE_CXX_STANDARD 17)

# Create the project
add_library(cppParserLibrary SHARED)

# make cache variables for install destinations
include(GNUInstallDirs)

# FetContent will allow downloading of other libraries
include(FetchContent)

# check for and download yaml-cpp
IF (TARGET yaml-cpp)
    message(STATUS "Found yaml-cpp target")
ELSE ()
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
ENDIF ()

# load in the build/compiler standards
FetchContent_Declare(
        chrestCompilerFlags
        GIT_REPOSITORY https://github.com/UBCHREST/chrestCompilerFlags.git
        GIT_TAG main
)
FetchContent_MakeAvailable(chrestCompilerFlags)

# Load in the source code
add_subdirectory(src)
target_link_libraries(cppParserLibrary PRIVATE yaml-cpp PRIVATE chrestCompilerFlags)

# Add a library that can be used for testing
add_library(cppParserTestLibrary INTERFACE)
target_include_directories(cppParserTestLibrary
        INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/tests/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

# Check if we should enable testing options
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)
    include(CTest)

    # Include the format command only if we are the main project
    include(clangFormatter.cmake)

    # Setup and configure testing
    if (BUILD_TESTING)
        enable_testing()
        include(GoogleTest)
        add_subdirectory(tests)
    endif ()
endif ()

# install the cppParserLibrary (and others) target and create export-set
install(TARGETS cppParserLibrary cppParserTestLibrary yaml-cpp chrestCompilerFlags
        EXPORT cppParserLibraryTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )


# install header file(s)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} FILES_MATCHING PATTERN "*.hpp")
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} FILES_MATCHING PATTERN "*.hpp")

# generate and install export file
install(EXPORT cppParserLibraryTargets
        FILE cppParserLibraryTargets.cmake
        NAMESPACE CHREST::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cppParserLibrary
        )

# include CMakePackageConfigHelpers macro
include(CMakePackageConfigHelpers)

# march over each target property to update path to includes
get_target_property(interfaceListOrg cppParserLibrary INTERFACE_SOURCES)
foreach (interfaceItem ${interfaceListOrg})
    # Replace this hard code value with relative values
    file(RELATIVE_PATH relativeHeaderPath ${PROJECT_SOURCE_DIR}/src ${interfaceItem})
    # add the lists back
    list(APPEND interfaceListUpdated "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/${relativeHeaderPath}>")
    list(APPEND interfaceListUpdated "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${relativeHeaderPath}>")
endforeach ()
set_property(TARGET cppParserLibrary PROPERTY INTERFACE_SOURCES ${interfaceListUpdated})

# generate the version file for the config file
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/cppParserLibraryConfigVersion.cmake"
        VERSION "${CMAKE_PROJECT_VERSION}"
        COMPATIBILITY AnyNewerVersion
)

# create config file
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/cppParserLibraryConfig.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cppParserLibrary
        )

# install config files
install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/cppParserLibraryConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/cppParserLibraryConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cppParserLibrary
        )

# generate the export targets for the build tree
export(EXPORT cppParserLibraryTargets
        FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/cppParserLibraryTargets.cmake"
        NAMESPACE CHREST::
        )

# set library properties
set_property(TARGET cppParserLibrary PROPERTY POSITION_INDEPENDENT_CODE ON)
set_property(TARGET yaml-cpp PROPERTY POSITION_INDEPENDENT_CODE ON)