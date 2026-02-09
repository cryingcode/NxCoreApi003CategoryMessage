// nxcore_global.cpp

#include "srclib/nxcore/NxCoreAPI_Wrapper_C++.h"

// Global NxCore wrapper instance
// In larger applications, prefer dependency injection / RAII wrapper
NxCoreClass NxCore; // actual definition here – no extern