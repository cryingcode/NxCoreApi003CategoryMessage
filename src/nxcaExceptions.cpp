// file:  nxcaExceptions.cpp

#include "nxcaExceptions.hpp"

std::string to_string(NxCAException code)
{
    switch (code)
    {
    case NxCAException::NxCA_EXCEPTION_USER_CANCEL:
        return "User called cancel.";
    case NxCAException::NxCA_EXCEPTION_USER_RESTART:
        return "User called restart.";
    case NxCAException::NxCA_EXCEPTION_CLOCK_CRC:
        return "A secondary CRC has failed, data is corrupt and the tape should be re-downloaded. Contact Nanex support (support@nanex.net) for assistance replacing tapes.";
    case NxCAException::NxCA_EXCEPTION_MSG_BIT_SIZE:
        return "Too many or too few bits were decoded, data is corrupt and the tape should be re-downloaded. Contact Nanex support (support@nanex.net) for assistance replacing tapes.";
    case NxCAException::NxCA_EXCEPTION_MSG_CHK_SUM:
        return "A tertiary checksum has failed, data is corrupt and the tape should be re-downloaded. Contact Nanex support (support@nanex.net) for assistance replacing tapes.";
    case NxCAException::NxCA_EXCEPTION_DECODE_FIELDS:
        return "Category fields failed a check, data is corrupt and the tape should be re-downloaded. Contact Nanex support (support@nanex.net) for assistance replacing tapes.";
    case NxCAException::NxCA_EXCEPTION_DECODE_CAT_DEF:
        return "Category definitions failed a check, data is corrupt and the tape should be re-downloaded. Contact Nanex support (support@nanex.net) for assistance replacing tapes.";
    case NxCAException::NxCA_EXCEPTION_DECODE_CAT_DATA:
        return "Category data failed a check, data is corrupt and the tape should be re-downloaded. Contact Nanex support (support@nanex.net) for assistance replacing tapes.";
    case NxCAException::NxCA_EXCEPTION_LISTED_EXG:
        return "A listed exchange was referenced that was not defined. Your permissions may need to be adjusted, contact Nanex support (support@nanex.net) for assistance changing your permissions.";
    case NxCAException::NxCA_EXCEPTION_OPT_SYMBOL:
        return "A option symbol was referenced that was not defined. Your permissions may need to be adjusted, contact Nanex support (support@nanex.net) for assistance changing your permissions.";
    case NxCAException::NxCA_EXCEPTION_SYMBOL:
        return "A symbol was referenced that was not defined. Your permissions may need to be adjusted, contact Nanex support (support@nanex.net) for assistance changing your permissions.";
    case NxCAException::NxCA_EXCEPTION_CS32:
        return "Internal decoder failure. Data is corrupt and the tape should be re-downloaded. Contact nanex support (support@nanex.net) for assistance replacing tapes.";
    case NxCAException::NxCA_EXCEPTION_PERM_ID_OOB:
        return "Permission ID out of bounds. Your permissions may need to be adjusted, contact Nanex support (support@nanex.net) for assistance changing your permissions.";
    case NxCAException::NxCA_EXCEPTION_BLOCK_CRC:
        return "A 64kB block of data has failed its CRC check. Data is corrupt and the tape should be re-downloaded. Contact nanex support (support@nanex.net) for assistance replacing tapes.";
    case NxCAException::NxCA_EXCEPTION_MSG_HEADER:
        return "Message header failed check, data is corrupt and the tape should be re-downloaded. Contact Nanex support (support@nanex.net) for assistance replacing tapes.";
    case NxCAException::NxCA_EXCEPTION_TAPE_HEADER:
        return "Very start of the tape failed check, data is corrupt and the tape should be re-downloaded. Contact Nanex support (support@nanex.net) for assistance replacing tapes.";
    case NxCAException::NxCA_EXCEPTION_END_OF_TAPE:
        return "End of tape has been reached.";
    case NxCAException::NxCA_EXCEPTION_UNSUPPORTED_API:
        return "The API version does not support the tape or NxCore Access Stream. Download the Latest API. Introduced in NxCore API 3.0.123 and 3.2.21.";
    case NxCAException::NxCA_EXCEPTION_LAST:
        return "Internal counter, not currently used. Can be safely ignored.";
    default:
        return "";
    }
}