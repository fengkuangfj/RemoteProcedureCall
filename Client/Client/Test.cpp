#include "stdafx.h"


BOOL
	TestRpcTest()
{
	BOOL				bRet			= FALSE;

	unsigned long		ExceptionCode	= 0;
	RPC_SERVER_STATUS	RpcServerStatus = RPC_SERVER_STATUS_FAILED;


	RpcTryExcept
	{
		RpcServerStatus = RpcTest((unsigned char *)"hehehe");
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


BOOL
	TestRpcStopServer()
{
	BOOL				bRet			= FALSE;

	unsigned long		ExceptionCode	= 0;
	RPC_SERVER_STATUS	RpcServerStatus = RPC_SERVER_STATUS_FAILED;


	RpcTryExcept
	{
		RpcServerStatus = RpcStopServer();
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
