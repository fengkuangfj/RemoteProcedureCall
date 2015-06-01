#include "stdafx.h"


/* [callback] */
RPC_SERVER_STATUS
	RpcTestCallBack( 
	/* [string][in] */ unsigned char *pch
	)
{
	RPC_SERVER_STATUS RpcServerStatus = RPC_SERVER_STATUS_FAILED;


	__try
	{
		if (!pch)
			__leave;

		printf("[%s] %s \n", __FUNCTION__, pch);

		RpcServerStatus = RPC_SERVER_STATUS_SUCCESS;
	}
	__finally
	{
		;
	}

	return RpcServerStatus;
}