// nxcoreCallback.cpp

#include "nxcoreCallback.hpp"
#include "processNxCoreCategoryMessage.hpp"
#include "processNxCoreStatusMessage.hpp"
#include "processNxCoreSymbolSpinMessage.hpp"

// extern NxCoreClass NxCore;
// still needed if you use NxCore functions inside handlers
// Consider: passing a context object instead of relying on global in the future

[[nodiscard]] int OnNxCoreCallback(
    const NxCoreSystem *pNxCoreSys,
    const NxCoreMessage *pNxCoreMsg) noexcept
{
    // ────────────────────────────────────────────────────────────────
    // Future / optional message types — placeholders only for now
    // Add [[fallthrough]]; when intentionally falling through
    switch (pNxCoreMsg->MessageType)
    {
    case NxMSG_STATUS:
        return processNxCoreStatusMessage(pNxCoreSys, pNxCoreMsg, std::cout);
    case NxMSG_EXGQUOTE:
        [[fallthrough]];
    case NxMSG_MMQUOTE:
        [[fallthrough]];
    case NxMSG_TRADE:
        break;
    case NxMSG_CATEGORY:
        return processNxCoreCategoryMessage(pNxCoreSys, pNxCoreMsg, std::cout);
    case NxMSG_SYMBOLCHANGE:
        break;
    case NxMSG_SYMBOLSPIN:
        // Intentionally ignored for now — future expansion point
        return processNxCoreSymbolSpinMessage(pNxCoreSys, pNxCoreMsg, std::cout);

    default:
        // Defensive: unknown message type
        // In production you might log, count, or return NxCALLBACKRETURN_STOP
        // For now we just continue (silent ignore)
        break;
    }

    return NxCALLBACKRETURN_CONTINUE;
}