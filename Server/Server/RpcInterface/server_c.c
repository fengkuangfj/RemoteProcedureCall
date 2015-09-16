#ifdef _M_X64
#ifdef _DEBUG
#include "Debug\x64\Server_c.c"
#else
#include "Release\x64\Server_c.c"
#endif
#else
#ifdef _DEBUG
#include "Debug\Server_c.c"
#else
#include "Release\Server_c.c"
#endif
#endif
