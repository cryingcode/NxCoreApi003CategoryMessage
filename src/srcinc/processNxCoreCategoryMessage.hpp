// processNxCoreCategoryMessage.hpp
#pragma once

#include "NxCoreAPI_Wrapper_C++.h" // NxCoreSystem, NxCoreClass, NxMSG_*, NxCORESTATUS_*, NxCLOCK_*, etc.

#include <iosfwd> // forward declare std::ostream

// Processes NxMSG_CATEGORY callbacks.
// Returns NxCALLBACKRETURN_CONTINUE (normal) or NxCALLBACKRETURN_STOP (on fatal error).
[[nodiscard]] int processNxCoreCategoryMessage(
    const NxCoreSystem * /*pNxCoreSys*/,
    const NxCoreMessage *pNxCoreMsg,
    std::ostream &out = std::cout) noexcept;