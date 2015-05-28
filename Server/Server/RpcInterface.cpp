#include "stdafx.h"


RPC_SERVER_STATUS
	RpcTest( 
	/* [string][in] */ unsigned char *puch
	)
{
	RPC_SERVER_STATUS RpcServerStatus = RPC_SERVER_STATUS_FAILED;


	__try
	{
		if (!puch)
			__leave;

		printf("[%s] %s \n", __FUNCTION__, puch);

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

	CPublicServer		PublicServer;


	do 
	{
		if (!PublicServer.Unload(TRUE))
			break;

		RpcServerStatus = RPC_SERVER_STATUS_SUCCESS;
	} while (FALSE);

	return RpcServerStatus;
}
