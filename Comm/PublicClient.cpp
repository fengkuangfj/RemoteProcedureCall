#include "PublicClient.h"


CPublicClient::CPublicClient()
{
	;
}

CPublicClient::~CPublicClient()
{
	;
}

BOOL
	CPublicClient::Init(
	__in	LPTSTR					lpObjUuid,
	__in	LPTSTR					lpProtSeq,
	__in	LPTSTR					lpNetworkAddr,
	__in	LPTSTR					lpEndPoint,
	__in	LPTSTR					lpOptions,
	__out	LPTSTR				*	plpStringBinding,
	__out	RPC_BINDING_HANDLE	*	pRpcBindingHandle
	)
{
	BOOL		bRet		= FALSE;

	RPC_STATUS	RpcStatus	= RPC_S_OK;


	__try
	{
		RpcStatus = RpcStringBindingCompose(
			(RPC_WSTR)lpObjUuid,
			(RPC_WSTR)lpProtSeq,
			(RPC_WSTR)lpNetworkAddr,
			(RPC_WSTR)lpEndPoint,
			(RPC_WSTR)lpOptions,
			(RPC_WSTR *)plpStringBinding
			);
		if (RPC_S_OK != RpcStatus)
			__leave;

		RpcStatus = RpcBindingFromStringBinding((RPC_WSTR)*plpStringBinding, pRpcBindingHandle);  
		if (RPC_S_OK != RpcStatus)
			__leave;

		bRet = TRUE;
	}
	__finally
	{
		;
	}
	
	return bRet;
}

void
	__RPC_FAR* __RPC_USER
	midl_user_allocate(
	size_t len
	)
{
	return(malloc(len));
}

void
	__RPC_USER
	midl_user_free(
	void __RPC_FAR *ptr
	)
{
	free(ptr);
}
