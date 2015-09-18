#include "stdafx.h"


BOOL
	TestRpcTest()
{
	BOOL				bRet			= FALSE;

	unsigned long		ExceptionCode	= RPC_S_OK;
	RPC_SERVER_STATUS	RpcServerStatus = RPC_SERVER_STATUS_FAILED;


	RpcTryExcept
	{
		RpcServerStatus = RpcTest(_T("form client"));
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
	TestRpcUseCallback()
{
	BOOL	bRet	= FALSE;

	HANDLE	hThread = NULL;


	__try
	{
		hThread = CreateThread(
			NULL,
			0,
			TestRpcUseCallbackThread,
			NULL,
			0,
			NULL		
			);
		if (!hThread)
			__leave;

		bRet = TRUE;
	}
	__finally
	{
		if (hThread)
			CloseHandle(hThread);
	}

	return bRet;
}

DWORD
	WINAPI
	TestRpcUseCallbackThread(
	LPVOID lpThreadParameter
	)
{
	RPC_SERVER_STATUS	RpcServerStatus = RPC_SERVER_STATUS_FAILED;
	unsigned long		ExceptionCode	= RPC_S_OK;


	RpcTryExcept
	{
		RpcServerStatus = RpcUseCallback();
		if (RPC_SERVER_STATUS_SUCCESS != RpcServerStatus)
			__leave;
	}
	RpcExcept(ExceptionCode = RpcExceptionCode(), EXCEPTION_EXECUTE_HANDLER)
	{
		printf("[%s] RpcExcept (%d) \n", __FUNCTION__, ExceptionCode);
	}
	RpcEndExcept;

	return 0;
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


	RpcTryExcept
	{
		if (bDirectlyStop)
		{
			if (!RpcBindingHandle)
				__leave;

			RpcStatus = RpcMgmtStopServerListening(RpcBindingHandle);
			if (RPC_S_OK != RpcStatus && RPC_S_SERVER_UNAVAILABLE != RpcStatus)
				__leave;
		}
		else
		{
			RpcServerStatus = RpcStopServer();
			if (RPC_SERVER_STATUS_SUCCESS != RpcServerStatus)
				__leave;
		}

		bRet = TRUE;
	}
	RpcExcept(ExceptionCode = RpcExceptionCode(), EXCEPTION_EXECUTE_HANDLER)
	{
		printf("[%s] RpcExcept (%d) \n", __FUNCTION__, ExceptionCode);
	}
	RpcEndExcept;

	return bRet;
}
