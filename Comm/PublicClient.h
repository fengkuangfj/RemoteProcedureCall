#pragma once


#include <Rpc.h>


#pragma comment(lib, "Rpcrt4.lib")


#ifndef _PUBLIC_CLIENT_TAG_
#define _PUBLIC_CLIENT_TAG_	'LCBP'
#endif


void
	__RPC_FAR* __RPC_USER
	midl_user_allocate(
	size_t len
	);

void
	__RPC_USER
	midl_user_free(
	void __RPC_FAR *ptr
	);


class CPublicClient
{
public:
	CPublicClient();

	~CPublicClient();

	BOOL
		Init(
		__in	LPTSTR					lpObjUuid,
		__in	LPTSTR					lpProtSeq,
		__in	LPTSTR					lpNetworkAddr,
		__in	LPTSTR					lpEndPoint,
		__in	LPTSTR					lpOptions,
		__out	LPTSTR				*	plpStringBinding,
		__out	RPC_BINDING_HANDLE	*	pRpcBindingHandle
		);

private:
};
