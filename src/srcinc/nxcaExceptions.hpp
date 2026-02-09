// nxcaExceptions.hpp
#pragma once

#include <string>
#include <string_view>   // C++17

enum class NxCAException : int
{
    Success = 0,                        // ← optional: add explicit success (very common pattern)

    UserCancel      = 1,
    UserRestart     = 2,
    ClockCrc        = 3,
    MsgBitSize      = 4,
    MsgChkSum       = 5,
    DecodeFields    = 6,
    DecodeCatDef    = 7,
    DecodeCatData   = 8,
    ListedExg       = 9,
    OptSymbol       = 10,
    Symbol          = 11,
    Cs32            = 12,
    PermIdOob       = 13,
    BlockCrc        = 14,
    MsgHeader       = 15,
    TapeHeader      = 16,
    EndOfTape       = 17,
    UnsupportedApi  = 18,

    // Last          // ← usually not needed in public API
};

[[nodiscard]] std::string_view to_string_view(NxCAException e) noexcept;
[[nodiscard]] std::string      to_string     (NxCAException e);

// Optional: if you really need int → enum conversion
[[nodiscard]] constexpr NxCAException to_nxca_exception(int code) noexcept
{
    if (code >= 1 && code <= 18) [[likely]]
        return static_cast<NxCAException>(code);

    return NxCAException::UnsupportedApi; // or some sentinel / throw in non-constexpr context
}