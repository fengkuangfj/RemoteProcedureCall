#pragma once


#include <Rpc.h>

#include "../../OsVersion/OsVersion/OsVersion.h"


#pragma comment(lib, "Rpcrt4.lib")


#ifndef _PUBLIC_SERVER_TAG_
#define _PUBLIC_SERVER_TAG_	'VSBP'
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


class CPublicServer
{
public:
	CPublicServer();

	~CPublicServer();

	BOOL
		Init(
		__in		LPTSTR			lpProtseq,
		__in		unsigned int	uMaxCalls,
		__in		LPTSTR			lpEndpoint,
		__in_opt	void          *	pSecurityDescriptor,
		__in		RPC_IF_HANDLE	RpcIfHandle,
		__in		unsigned int	uMinimumCallThreads,
		__in_opt	UUID          *	pMgrTypeUuid,
		__in_opt	RPC_MGR_EPV   *	pMgrEpv,
		__in_opt	unsigned int	uDontWait
		);

	BOOL
		Unload(
		__in BOOL bFromRpcInterface
		);

private:
	typedef struct _RPC_SERVER_LISTEN_INFO
	{
		unsigned int uMinimumCallThreads;
		unsigned int uMaxCalls;
	} RPC_SERVER_LISTEN_INFO, *PRPC_SERVER_LISTEN_INFO, *LPRPC_SERVER_LISTEN_INFO;

	static unsigned int		ms_bDontWait;
	static UUID			*	ms_pMgrTypeUuid;

	static
		DWORD
		WINAPI
		RpcServerListenThread(
		LPVOID lpThreadParameter
		);
};
