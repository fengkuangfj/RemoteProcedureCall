#include "PublicServer.h"


CPublicServer::CPublicServer()
{
	;
}

CPublicServer::~CPublicServer()
{
	;
}

BOOL
	CPublicServer::Init(
	__in LPTSTR				lpProtseq,
	__in unsigned int		uMaxCalls,
	__in LPTSTR				lpEndpoint,
	__in RPC_IF_HANDLE		RpcIfHandle,
	__in unsigned int		uMinimumCallThreads,
	__in UUID          *	pMgrTypeUuid,
	__in RPC_MGR_EPV   *	pMgrEpv,
	__in unsigned int		uDontWait
	)
{
	BOOL						bRet					= FALSE;

	RPC_STATUS					RpcStatus				= RPC_S_OK;
	OS_VER						OsVer					= OS_VER_UNKNOWN;
	LPRPC_SERVER_LISTEN_INFO	lpRpcServerListenInfo	= {0};
	HANDLE						hThread					= NULL;

	COsVersion					OsVersion;


	__try
	{
		if (!lpProtseq ||
			!uMaxCalls ||
			!lpEndpoint ||
			!RpcIfHandle ||
			!uMinimumCallThreads)
			__leave;

		RpcStatus = RpcServerUseProtseqEp(
			(RPC_WSTR)lpProtseq,
			uMaxCalls,
			(RPC_WSTR)lpEndpoint,
			NULL
			);    
		if (RPC_S_OK != RpcStatus)
			__leave;

		RpcStatus = RpcServerRegisterIf(RpcIfHandle, pMgrTypeUuid, pMgrEpv);
		if (RPC_S_OK != RpcStatus)
			__leave;

		if (uMinimumCallThreads >= uMaxCalls)
			__leave;

		OsVer = OsVersion.GetOSVer();
		if (OS_VER_WINDOWS_XP == OsVer)
			uMaxCalls = RPC_C_LISTEN_MAX_CALLS_DEFAULT;

		if (uDontWait)
		{
			RpcStatus = RpcServerListen(uMinimumCallThreads, uMaxCalls, uDontWait); 
			if (RPC_S_OK != RpcStatus)
				__leave;
		}
		else
		{
			lpRpcServerListenInfo = (LPRPC_SERVER_LISTEN_INFO)calloc(1, sizeof(RPC_SERVER_LISTEN_INFO));
			if (!lpRpcServerListenInfo)
				__leave;

			lpRpcServerListenInfo->uMinimumCallThreads = uMinimumCallThreads;
			lpRpcServerListenInfo->uMaxCalls = uMaxCalls;

			hThread = CreateThread(
				NULL,
				0,
				RpcServerListenThread,
				lpRpcServerListenInfo,
				0,
				NULL				
				);
			if (!hThread)
				__leave;
		}

		bRet = TRUE;
	}
	__finally
	{
		if (!bRet && lpRpcServerListenInfo)
			free(lpRpcServerListenInfo);

		if (hThread)
			CloseHandle(hThread);
	}

	return bRet;
}

DWORD
	WINAPI
	CPublicServer::RpcServerListenThread(
	LPVOID lpThreadParameter
	)
{
	LPRPC_SERVER_LISTEN_INFO	lpRpcServerListenInfo	= NULL;
	RPC_STATUS					RpcStatus				= RPC_S_OK;


	__try
	{
		if (!lpThreadParameter)
			__leave;

		lpRpcServerListenInfo = (LPRPC_SERVER_LISTEN_INFO)lpThreadParameter;

		RpcStatus = RpcServerListen(lpRpcServerListenInfo->uMinimumCallThreads, lpRpcServerListenInfo->uMaxCalls, 0); 
		if (RPC_S_OK != RpcStatus)
			__leave;
	}
	__finally
	{
		if (lpRpcServerListenInfo)
			free(lpRpcServerListenInfo);
	}

	return 0;
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
