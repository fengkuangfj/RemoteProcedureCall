#ifdef _M_X64
#ifdef _DEBUG
#include "Debug\x64\Server_s.c"
#else
#include "Release\x64\Server_s.c"
#endif
#else
#ifdef _DEBUG
#include "Debug\Server_s.c"
#else
#include "Release\Server_s.c"
#endif
#endif
