target_sources(cppParserLibrary
        PUBLIC
        argumentIdentifier.hpp
        enumWrapper.hpp
        factory.hpp
        listing.hpp
        listing.cpp
        registrar.hpp
        yamlParser.hpp
        yamlParser.cpp
        demangler.hpp
        demangler.cpp
        )

target_include_directories(cppParserLibrary PUBLIC ${CMAKE_CURRENT_LIST_DIR})
