#include "stdafx.h"


/* [callback] */
RPC_SERVER_STATUS
	RpcCallback(
	/* [string][in] */ LPTSTR lpCh
	)
{
	RPC_SERVER_STATUS RpcServerStatus = RPC_SERVER_STATUS_FAILED;


	__try
	{
		if (!lpCh)
			__leave;

		printf("[%s] %S \n", __FUNCTION__, lpCh);

		RpcServerStatus = RPC_SERVER_STATUS_SUCCESS;
	}
	__finally
	{
		;
	}

	return RpcServerStatus;
}
