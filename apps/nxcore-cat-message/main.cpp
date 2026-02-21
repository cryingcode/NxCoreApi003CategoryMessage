// main.cpp

// =============================================================================
// SampleApp3.cpp
// Category Message – Modernized C++17 version
// Derived from SampleApp3 (Jeffrey Donovan, Jeremy Truelove, 2017)
//
// Demonstrates:
//   - Handling the NxCore Category Message
// =============================================================================

#include <iostream>
#include <string>
#include <string_view>
#include <filesystem>
#include <cstdlib>
#include <stdexcept>
#include <dlfcn.h> // for dlerror() — already used indirectly via NxCore

#include "executableUtils.hpp"
#include "nxcoreCallback.hpp"

#include "srclib/nxcore/NxCoreAPI_Wrapper_C++.h"

extern NxCoreClass NxCore;

namespace
{

    // Flags we want to use (excluding most quote-related data to reduce volume)
    constexpr unsigned int DEFAULT_PROCESS_FLAGS =
        NxCF_EXCLUDE_QUOTES |
        NxCF_EXCLUDE_QUOTES2 |
        NxCF_EXCLUDE_OPRA;

    void print_usage(std::string_view program_name)
    {
        std::cout << "Usage:\n"
                  << "  " << program_name << " <path-to-libnx.so> [path-to-tape-file]\n\n"
                  << "Examples:\n"
                  << "  " << program_name << " ./libnx.so /data/tapes/20250102.nxc\n"
                  << "  " << program_name << " ./libnx.so            # process live feed\n";
    }

    [[nodiscard]] std::string get_program_name(int argc, char *argv[])
    {
        if (argc <= 0 || !argv || !argv[0])
        {
            return "nxcore-cat-message"; // fallback name
        }

        try
        {
            return get_executable_filename();
        }
        catch (const std::exception &e)
        {
            std::cerr << "Warning: cannot determine executable name: " << e.what() << '\n';
            return std::filesystem::path(argv[0]).filename().string();
        }
    }

} // namespace

int main(int argc, char *argv[])
{
    const auto exe_name = get_program_name(argc, argv);

    if (argc < 2)
    {
        std::cout << "NxCore Category Message (modernized C++17 version)\n\n";
        print_usage(exe_name);
        return EXIT_SUCCESS;
    }

    const std::string lib_path = argv[1];
    const std::string tape_path = (argc >= 3) ? argv[2] : "";

    std::cout << exe_name << " starting...\n";

    // ── Load NxCore library ─────────────────────────────────────────────────
    if (!NxCore.LoadNxCore(lib_path.c_str()))
    {
        std::cerr << "Failed to load NxCore library: " << dlerror() << '\n';
        return EXIT_FAILURE;
    }

    std::cout << "NxCore library loaded successfully.\n";

    // ── Process tape or live feed ───────────────────────────────────────────
    std::cout << "Processing " << (tape_path.empty() ? "live feed" : "tape: " + tape_path) << '\n';

    const int return_value = NxCore.ProcessTape(
        tape_path.c_str(), // tape filename or "" for live
        nullptr,           // no user data
        DEFAULT_PROCESS_FLAGS,
        0, // reserved / future
        reinterpret_cast<NxCoreCallback>(OnNxCoreCallback));

    // ── Handle return code ──────────────────────────────────────────────────
    NxCore.ProcessReturnValue(return_value);

    // ── Cleanup ─────────────────────────────────────────────────────────────
    NxCore.UnloadNxCore();

    std::cout << exe_name << " finished.\n";
    return EXIT_SUCCESS;
}