#include "stdafx.h"


RPC_SERVER_STATUS
RpcTest(
	/* [string][in] */ LPTSTR lpCh
)
{
	RPC_SERVER_STATUS RpcServerStatus = RPC_SERVER_STATUS_FAILED;


	__try
	{
		if (!lpCh)
			__leave;

		printf("[%s] %S [current thread]%d \n", __FUNCTION__, lpCh, GetCurrentThreadId());

		RpcServerStatus = RPC_SERVER_STATUS_SUCCESS;
	}
	__finally
	{
		;
	}

	return RpcServerStatus;
}

RPC_SERVER_STATUS
RpcUseCallback(
	void
)
{
	RPC_SERVER_STATUS RpcServerStatus = RPC_SERVER_STATUS_FAILED;


	__try
	{
		if (!TestRpcCallback())
			__leave;

		RpcServerStatus = RPC_SERVER_STATUS_SUCCESS;
	}
	__finally
	{
		;
	}

	return RpcServerStatus;
}

RPC_SERVER_STATUS
RpcStopServer(
	void
)
{
	RPC_SERVER_STATUS	RpcServerStatus = RPC_SERVER_STATUS_FAILED;


	do
	{
		if (!CRpcServer::GetInstance()->Unload(TRUE))
			break;

		RpcServerStatus = RPC_SERVER_STATUS_SUCCESS;
	} while (FALSE);

	return RpcServerStatus;
}
