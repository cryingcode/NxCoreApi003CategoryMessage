#pragma once

#include <string>

enum class NxCAException : int
{
    NxCA_EXCEPTION_USER_CANCEL = 1,      // User called cancel.
    NxCA_EXCEPTION_USER_RESTART = 2,     // User called restart.
    NxCA_EXCEPTION_CLOCK_CRC = 3,        // Clock CRC error.
    NxCA_EXCEPTION_MSG_BIT_SIZE = 4,     // Message bit size error.
    NxCA_EXCEPTION_MSG_CHK_SUM = 5,      // Message checksum error.
    NxCA_EXCEPTION_DECODE_FIELDS = 6,    // Decode fields error.
    NxCA_EXCEPTION_DECODE_CAT_DEF = 7,   // Decode category definitions error.
    NxCA_EXCEPTION_DECODE_CAT_DATA = 8,  // Decode category data error.
    NxCA_EXCEPTION_LISTED_EXG = 9,       // Listed exchange not defined.
    NxCA_EXCEPTION_OPT_SYMBOL = 10,      // Option symbol not defined.
    NxCA_EXCEPTION_SYMBOL = 11,          // Symbol not defined.
    NxCA_EXCEPTION_CS32 = 12,            // Internal decoder failure.
    NxCA_EXCEPTION_PERM_ID_OOB = 13,     // Permission ID out of bounds.
    NxCA_EXCEPTION_BLOCK_CRC = 14,       // 64kB block CRC error.
    NxCA_EXCEPTION_MSG_HEADER = 15,      // Message header error.
    NxCA_EXCEPTION_TAPE_HEADER = 16,     // Tape header error.
    NxCA_EXCEPTION_END_OF_TAPE = 17,     // End of tape reached.
    NxCA_EXCEPTION_UNSUPPORTED_API = 18, // Unsupported API call.
    NxCA_EXCEPTION_LAST = 19,            // Internal counter, not currently used. Can be safely ignored.
};

std::string to_string(NxCAException code);

inline std::string to_string(int code)
{
    return to_string(static_cast<NxCAException>(code));
}