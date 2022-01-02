#pragma once

#include "defs.h"
#include <ctime>
#include <chrono>
#if defined(USE_WINDOWS)
#    include <Windows.h>
#elif __linux__
#    include <sys/stat.h>
#endif
/**
 * @brief Creates a timestap either from file or just current time
 * If it fails to read the timestamp of a file, it set the time stamp to current time
 * 
 * @warning It uses std::time to get current time, which is not standardized to be 1970-01-01....
 * However, it works on Windows and Unix https://stackoverflow.com/questions/6012663/get-unix-timestamp-with-c 
 * With C++20 this will be standardized
 */
struct Timestamp
{
    tm timestamp;
    Timestamp();
    Timestamp(const std::string& filepath);
};

Timestamp::Timestamp()
{
    std::time_t now = std::time(nullptr);
    timestamp = *std::localtime(&now);
}

Timestamp::Timestamp(const std::string& filepath)
{
    //Set default
    std::time_t now = std::time(nullptr);
    timestamp = *std::localtime(&now);
#if defined(USE_WINDOWS)

#elif __linux__
    struct stat buf;
    if (stat(filepath.c_str(), &buf) != 0)
    {
        return;
    }
    auto timet = static_cast<time_t>(buf.st_mtim.tv_sec);
    timestamp = *std::localtime(&timet);
#endif
}
