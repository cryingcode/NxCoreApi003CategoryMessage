// executableUtils.cpp
#include "executableUtils.hpp"

#include <filesystem>
#include <string>
#include <stdexcept>

namespace fs = std::filesystem;

[[nodiscard]] std::filesystem::path get_executable_path()
{
    // On Linux → /proc/self/exe is a symlink to the actual binary
    static const fs::path proc_self_exe{"/proc/self/exe"};

    std::error_code ec;
    auto canonical_path = fs::canonical(proc_self_exe, ec);

    if (ec)
    {
        throw std::runtime_error(
            "Failed to canonicalize /proc/self/exe: " + ec.message());
    }

    return canonical_path;
}

[[nodiscard]] std::string get_executable_filename()
{
    // Most common use-case is just the filename → we cache the full path
    static const auto cached_path = get_executable_path();

    // .filename() → returns path (may be empty)
    // .string()   → std::string (empty if no filename component)
    return cached_path.filename().string();
}