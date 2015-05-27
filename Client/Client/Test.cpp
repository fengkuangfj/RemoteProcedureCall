#include "stdafx.h"


BOOL
	Test()
{
	BOOL			bRet			= FALSE;

	unsigned long	ExceptionCode	= 0;


	RpcTryExcept
	{
		RpcTest((unsigned char *)"hehehe");

		RpcStop();

		bRet = TRUE;
	}
	RpcExcept(ExceptionCode = RpcExceptionCode(), EXCEPTION_EXECUTE_HANDLER)
	{
		printf("RpcExcept (%d) \n", ExceptionCode);
	}
	RpcEndExcept;

	return bRet;
}