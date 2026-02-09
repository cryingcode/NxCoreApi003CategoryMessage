// nxcoreCallback.hpp
#pragma once

#include "NxCoreAPI_Wrapper_C++.h"  // defines NxCoreClass, NxCoreSystem, NxCoreMessage, NxMSG_*, NxCALLBACKRETURN_*

// Forward declaration — actual processing function is in another module
int processNxCoreStatusMessage(const NxCoreSystem* pNxCoreSys);

// Main NxCore callback handler
// Returns NxCALLBACKRETURN_CONTINUE (0) to keep processing
//         NxCALLBACKRETURN_STOP     (1) to abort processing
[[nodiscard]] int OnNxCoreCallback(
    const NxCoreSystem*  pNxCoreSys,
    const NxCoreMessage* pNxCoreMsg) noexcept;