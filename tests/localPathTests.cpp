#include <fstream>
#include <memory>
#include <sstream>
#include "gtest/gtest.h"
#include "localPath.hpp"

TEST(LocalPathTests, ShouldLocateFileInSearchPaths) {
    // arrange
    std::filesystem::path directory = std::filesystem::temp_directory_path() / "tmpDir";
    std::filesystem::create_directories(directory);
    std::filesystem::path tmpFile = directory / "tempFile.txt";
    {
        std::ofstream ofs(tmpFile);
        ofs << " tempFile" << std::endl;
        ofs.close();
    }

    cppParser::LocalPath localPathLocator("tempFile.txt");

    // act
    auto computedFilePath = localPathLocator.Locate({directory});

    // assert
    ASSERT_TRUE(std::filesystem::exists(computedFilePath));
    ASSERT_EQ(computedFilePath, std::filesystem::canonical(tmpFile));

    // cleanup
    std::filesystem::remove(tmpFile);
    std::filesystem::remove_all(directory);
}

TEST(LocalPathTests, FunctionShouldLocateFileInSearchPaths) {
    // arrange
    std::filesystem::path directory = std::filesystem::temp_directory_path() / "tmpDir";
    std::filesystem::create_directories(directory);
    std::filesystem::path tmpFile = directory / "tempFile.txt";
    {
        std::ofstream ofs(tmpFile);
        ofs << " tempFile" << std::endl;
        ofs.close();
    }

    cppParser::LocalPath localPathLocator("tempFile.txt");

    // act
    auto computedFilePath = localPathLocator.Locate({directory});

    // assert
    ASSERT_TRUE(std::filesystem::exists(computedFilePath));
    ASSERT_EQ(computedFilePath, std::filesystem::canonical(tmpFile));

    // cleanup
    std::filesystem::remove(tmpFile);
    std::filesystem::remove_all(directory);
}

TEST(LocalPathTests, StaticShouldLocateFileInSearchPaths) {
    // arrange
    std::filesystem::path directory = std::filesystem::temp_directory_path() / "tmpDir";
    std::filesystem::create_directories(directory);
    std::filesystem::path tmpFile = directory / "tempFile.txt";
    {
        std::ofstream ofs(tmpFile);
        ofs << " tempFile" << std::endl;
        ofs.close();
    }

    cppParser::LocalPath localPathLocator("tempFile.txt");

    // act
    auto computedFilePath = localPathLocator.Locate({directory});

    // assert
    ASSERT_TRUE(std::filesystem::exists(computedFilePath));
    ASSERT_EQ(computedFilePath, std::filesystem::canonical(tmpFile));

    // cleanup
    std::filesystem::remove(tmpFile);
    std::filesystem::remove_all(directory);
}

TEST(LocalPathTests, ShouldLocateRelativeFileInSearchPathsAndReturnCanonicalPath) {
    // arrange
    std::filesystem::path directory = std::filesystem::temp_directory_path() / "tmpDir";
    std::filesystem::remove_all(directory);
    std::filesystem::create_directories(directory);
    std::filesystem::path otherDirectory = std::filesystem::temp_directory_path() / "otherDir";
    std::filesystem::remove_all(otherDirectory);
    std::filesystem::create_directories(otherDirectory);

    std::filesystem::path tmpFile = otherDirectory / "tempFile.txt";
    {
        std::ofstream ofs(tmpFile);
        ofs << " tempFile" << std::endl;
        ofs.close();
    }

    cppParser::LocalPath localPathLocator("../otherDir/tempFile.txt");

    // act
    auto computedFilePath = localPathLocator.Locate({directory});

    // assert
    ASSERT_TRUE(std::filesystem::exists(computedFilePath));
    ASSERT_EQ(computedFilePath, std::filesystem::canonical(tmpFile));

    // cleanup
    std::filesystem::remove(tmpFile);
    std::filesystem::remove_all(directory);
    std::filesystem::remove_all(otherDirectory);
}

TEST(LocalPathTests, FunctionShouldLocateRelativeFileInSearchPathsAndReturnCanonicalPath) {
    // arrange
    std::filesystem::path directory = std::filesystem::temp_directory_path() / "tmpDir";
    std::filesystem::remove_all(directory);
    std::filesystem::create_directories(directory);
    std::filesystem::path otherDirectory = std::filesystem::temp_directory_path() / "otherDir";
    std::filesystem::remove_all(otherDirectory);
    std::filesystem::create_directories(otherDirectory);

    std::filesystem::path tmpFile = otherDirectory / "tempFile.txt";
    {
        std::ofstream ofs(tmpFile);
        ofs << " tempFile" << std::endl;
        ofs.close();
    }

    cppParser::LocalPath localPathLocator("../otherDir/tempFile.txt");

    // act
    auto computedFilePath = localPathLocator.Locate({directory});

    // assert
    ASSERT_TRUE(std::filesystem::exists(computedFilePath));
    ASSERT_EQ(computedFilePath, std::filesystem::canonical(tmpFile));

    // cleanup
    std::filesystem::remove(tmpFile);
    std::filesystem::remove_all(directory);
    std::filesystem::remove_all(otherDirectory);
}

TEST(LocalPathTests, StaticShouldLocateRelativeFileInSearchPathsAndReturnCanonicalPath) {
    // arrange
    std::filesystem::path directory = std::filesystem::temp_directory_path() / "tmpDir";
    std::filesystem::remove_all(directory);
    std::filesystem::create_directories(directory);
    std::filesystem::path otherDirectory = std::filesystem::temp_directory_path() / "otherDir";
    std::filesystem::remove_all(otherDirectory);
    std::filesystem::create_directories(otherDirectory);

    std::filesystem::path tmpFile = otherDirectory / "tempFile.txt";
    {
        std::ofstream ofs(tmpFile);
        ofs << " tempFile" << std::endl;
        ofs.close();
    }

    cppParser::LocalPath localPathLocator("../otherDir/tempFile.txt");

    // act
    auto computedFilePath = localPathLocator.Locate({directory});

    // assert
    ASSERT_TRUE(std::filesystem::exists(computedFilePath));
    ASSERT_EQ(computedFilePath, std::filesystem::canonical(tmpFile));

    // cleanup
    std::filesystem::remove(tmpFile);
    std::filesystem::remove_all(directory);
    std::filesystem::remove_all(otherDirectory);
}
