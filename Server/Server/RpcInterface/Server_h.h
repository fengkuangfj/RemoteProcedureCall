#ifdef _M_X64
#ifdef _DEBUG
#include "Debug\x64\Server_h.h"
#else
#include "Release\x64\Server_h.h"
#endif
#else
#ifdef _DEBUG
#include "Debug\Server_h.h"
#else
#include "Release\Server_h.h"
#endif
#endif
