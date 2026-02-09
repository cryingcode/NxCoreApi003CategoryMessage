// nxcaExceptions.cpp
#include "nxcaExceptions.hpp"

#include <string_view>

using namespace std::literals;

[[nodiscard]] std::string_view to_string_view(NxCAException e) noexcept
{
    switch (e)
    {
    case NxCAException::UserCancel:
        return "User called cancel."sv;
    case NxCAException::UserRestart:
        return "User called restart."sv;
    case NxCAException::ClockCrc:
        return "Clock CRC error – data corrupt. Re-download tape or contact support@nanex.net"sv;
    case NxCAException::MsgBitSize:
        return "Message bit size error – data corrupt. Re-download tape or contact support@nanex.net"sv;
    case NxCAException::MsgChkSum:
        return "Message checksum error – data corrupt. Re-download tape or contact support@nanex.net"sv;
    case NxCAException::DecodeFields:
        return "Decode fields error – data corrupt. Re-download tape or contact support@nanex.net"sv;
    case NxCAException::DecodeCatDef:
        return "Decode category definitions error – data corrupt. Re-download tape or contact support@nanex.net"sv;
    case NxCAException::DecodeCatData:
        return "Decode category data error – data corrupt. Re-download tape or contact support@nanex.net"sv;
    case NxCAException::ListedExg:
        return "Listed exchange not defined – check permissions. Contact support@nanex.net"sv;
    case NxCAException::OptSymbol:
        return "Option symbol not defined – check permissions. Contact support@nanex.net"sv;
    case NxCAException::Symbol:
        return "Symbol not defined – check permissions. Contact support@nanex.net"sv;
    case NxCAException::Cs32:
        return "Internal decoder failure (CS32) – data corrupt. Re-download tape or contact support@nanex.net"sv;
    case NxCAException::PermIdOob:
        return "Permission ID out of bounds – check permissions. Contact support@nanex.net"sv;
    case NxCAException::BlockCrc:
        return "64kB block CRC error – data corrupt. Re-download tape or contact support@nanex.net"sv;
    case NxCAException::MsgHeader:
        return "Message header error – data corrupt. Re-download tape or contact support@nanex.net"sv;
    case NxCAException::TapeHeader:
        return "Tape header error – data corrupt. Re-download tape or contact support@nanex.net"sv;
    case NxCAException::EndOfTape:
        return "End of tape reached."sv;
    case NxCAException::UnsupportedApi:
        return "Unsupported API version – download latest NxCore API (3.0.123+ / 3.2.21+)"sv;

    default:
        return "Unknown NxCAException"sv;
    }
}

[[nodiscard]] std::string to_string(NxCAException e)
{
    return std::string{to_string_view(e)};
}