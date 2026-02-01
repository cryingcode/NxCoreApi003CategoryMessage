// file:  executableUtils.cpp

#include <stdexcept>
#include "executableUtils.hpp"

std::string get_executable_path()
{
    namespace fs = std::filesystem;
    try
    {
        return fs::canonical("/proc/self/exe").string();
    }
    catch (const fs::filesystem_error &e)
    {
        throw std::runtime_error("Failed to resolve executable path: " + std::string(e.what()));
    }
}

std::string get_executable_filename()
{
    return std::filesystem::path(get_executable_path()).filename().string();
}