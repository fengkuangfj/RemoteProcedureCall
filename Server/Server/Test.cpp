#include "stdafx.h"


BOOL
	TestRpcCallback()
{
	BOOL				bRet			= FALSE;

	RPC_SERVER_STATUS	RpcServerStatus = RPC_SERVER_STATUS_FAILED;
	unsigned long		ExceptionCode	= RPC_S_OK;


	RpcTryExcept
	{
		RpcServerStatus = RpcCallback((unsigned char *)"form server");
		if (RPC_SERVER_STATUS_SUCCESS != RpcServerStatus)
			__leave;

		bRet = TRUE;
	}
	RpcExcept(ExceptionCode = RpcExceptionCode(), EXCEPTION_EXECUTE_HANDLER)
	{
		printf("[%s] RpcExcept (%d) \n", __FUNCTION__, ExceptionCode);
	}
	RpcEndExcept;

	return bRet;
}
