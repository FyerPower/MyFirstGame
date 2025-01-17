#pragma once

// ###############################################
// #tag Includes
// ###############################################

#include "bump_allocator.hpp"
#include "logger.hpp"

// This is to get the edit timestampt of files
#include <sys/stat.h>

// This is to get memset
#include <string.h>

// ###############################################
// #tag Functions
// ###############################################

long long get_timestamp(const char* filePath)
{
    struct stat file_stat = {}; // Construct the file_stat variable
    stat(filePath, &file_stat); // Get the stats of a file and store it into `file_stat`
    return file_stat.st_mtime;  // Return the modified timestamp of the file
};

long get_file_size(const char* filePath)
{
    struct stat file_stat = {}; // Construct the file_stat variable
    stat(filePath, &file_stat); // Get the stats of a file and store it into `file_stat`
    return file_stat.st_size;   // Return the size of the file.  Caveat: This only works for regular files, not symlinks /
                                // shared memory objects / or typed memory objects. If those are needed, you will need to
                                // open the file, fseek to the end, ftell to get the size, fseek back to start, and close.
};

bool file_exists(const char* filePath)
{
    FP_ASSERT(filePath, "No filePath supplied!");

    // Attempt to open the file (read binary)
    FILE* file;
    if (fopen_s(&file, filePath, "rb") != 0) {
        // If no file was found, return false that the file does not exist.
        return false;
    }

    // Otherwise.. close the file and return true.
    fclose(file);
    return true;
};

char* read_file(const char* filePath, int* fileSize, char* buffer)
{
    // Ensure variables are sent
    FP_ASSERT(filePath, "No filePath specified.");
    FP_ASSERT(fileSize, "No fileSize specified.");
    FP_ASSERT(buffer, "No buffer specified.");

    // Set the fileSize to zero (calculated later)
    *fileSize = 0;

    // Attempt to open the file (read binary)
    FILE* file;
    if (fopen_s(&file, filePath, "rb") != 0) {
        // If no file was found, log an error and return a nullptr
        return nullptr;
    }

    // Calculate the fileSize of the file opened
    fseek(file, 0, SEEK_END);
    *fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Ensure the entire buffer is cleared by setting it to 0;
    memset(buffer, 0, *fileSize + 1);

    // Read the entire file and store it into buffer
    fread(buffer, sizeof(char), *fileSize, file);

    // Close the File
    fclose(file);

    // Return the buffer
    return buffer;
};

char* read_file(const char* filePath, int* fileSize, BumpAllocator* bumpAllocator)
{
    char* file = nullptr;
    long fileSize2 = get_file_size(filePath);

    if (fileSize2) {
        char* buffer = bump_alloc(bumpAllocator, fileSize2 + 1);
        file = read_file(filePath, fileSize, buffer);
    }
    return file;
};

void write_file(const char* filePath, char* buffer, int fileSize)
{
    // Ensure variables are sent
    FP_ASSERT(filePath, "No filePath specified.");
    FP_ASSERT(buffer, "No buffer specified.");
    FP_ASSERT(fileSize, "No fileSize specified.");

    // Attempt to open the file (write binary)
    FILE* file;
    if (fopen_s(&file, filePath, "wb") != 0) {
        // If no file was found, log an error and return
        FP_ERROR("Failed opening file: %s", filePath);
        return;
    }

    // Write the entire buffer to the file
    fwrite(buffer, sizeof(char), fileSize, file);

    // Close the File
    fclose(file);
};

bool copy_file(const char* filePath, const char* outputPath, char* buffer)
{
    // Ensure variables are sent
    FP_ASSERT(filePath, "No filePath specified.");
    FP_ASSERT(outputPath, "No outputPath specified.");
    FP_ASSERT(buffer, "No buffer specified.");

    // initialize fileSize to 0 and then read the file getting its content and size.
    int fileSize = 0;
    char* data = read_file(filePath, &fileSize, buffer);

    // open the destination file (creates if it doesn't exist)
    FILE* outputFile;
    if (fopen_s(&outputFile, outputPath, "wb") != 0) {
        FP_ERROR("Failed opening File: %s", outputPath);
        return false;
    }

    // write the contents of the original file into the outputFile
    auto result = fwrite(data, sizeof(char), fileSize, outputFile);
    if (!result) {
        FP_ERROR("Failed writing File: %s", outputPath);
        fclose(outputFile);
        return false;
    }

    // close file
    fclose(outputFile);

    // return success
    return true;
};

bool copy_file(const char* filePath, const char* outputPath, BumpAllocator* bumpAllocator)
{
    // Get the filesize of the original file
    long fileSize = get_file_size(filePath);

    if (fileSize) {
        // allocate space for the file contents
        char* buffer = bump_alloc(bumpAllocator, fileSize + 1);

        // copy file
        return copy_file(filePath, outputPath, buffer);
    }

    // if no filesize was found, return
    return false;
};