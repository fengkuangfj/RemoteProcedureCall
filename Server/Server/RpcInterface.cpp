#include "stdafx.h"


RPC_SERVER_STATUS
	TestInterface( 
	/* [string][in] */ unsigned char *puch
	)
{
	RPC_SERVER_STATUS RpcServerStatus = RPC_SERVER_STATUS_FAILED;


	__try
	{
		printf("%s \n", puch);

		RpcServerStatus = RPC_SERVER_STATUS_SUCCESS;
	}
	__finally
	{
		;
	}

	return RpcServerStatus;
}

RPC_SERVER_STATUS
	StopRpcServer(
	void
	)
{
	RPC_SERVER_STATUS	RpcServerStatus = RPC_SERVER_STATUS_FAILED;

	RPC_STATUS			RpcStatus		= RPC_S_OK;


	__try
	{
		RpcStatus = RpcMgmtStopServerListening(NULL);
		if (RPC_S_OK != RpcStatus)
			__leave;

		RpcStatus = RpcServerUnregisterIf(NULL, NULL, FALSE);
		if (RPC_S_OK != RpcStatus)
			__leave;

		RpcServerStatus = RPC_SERVER_STATUS_SUCCESS;
	}
	__finally
	{
		;
	}

	return RpcServerStatus;
}
