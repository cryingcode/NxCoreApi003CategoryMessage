// executableUtils.hpp
#pragma once

#include <filesystem>
#include <string>

[[nodiscard]] std::filesystem::path get_executable_path();
[[nodiscard]] std::string get_executable_filename();