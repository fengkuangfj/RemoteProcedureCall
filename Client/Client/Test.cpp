#include "stdafx.h"


BOOL
	TestRpcTest()
{
	BOOL				bRet			= FALSE;

	unsigned long		ExceptionCode	= RPC_S_OK;
	RPC_SERVER_STATUS	RpcServerStatus = RPC_SERVER_STATUS_FAILED;


	RpcTryExcept
	{
		RpcServerStatus = RpcTest((unsigned char *)"form client");
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
	TestRpcStopServer(
	__in		BOOL				bDirectlyStop,
	__in_opt	RPC_BINDING_HANDLE	RpcBindingHandle
	)
{
	BOOL				bRet			= FALSE;

	unsigned long		ExceptionCode	= RPC_S_OK;
	RPC_SERVER_STATUS	RpcServerStatus = RPC_SERVER_STATUS_FAILED;
	RPC_STATUS			RpcStatus		= RPC_S_OK;


	__try
	{

		if (bDirectlyStop)
		{
			if (!RpcBindingHandle)
				__leave;

			RpcStatus = RpcMgmtStopServerListening(RpcBindingHandle);
			if (RPC_S_OK != RpcStatus)
				__leave;

			bRet = TRUE;
		}
		else
		{
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
		}
	}
	__finally
	{
		;
	}

	return bRet;
}
