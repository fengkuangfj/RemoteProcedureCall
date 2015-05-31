#include "PublicServer.h"


unsigned int		CPublicServer::ms_bDontWait					= 0;
UUID			*	CPublicServer::ms_pMgrTypeUuid				= NULL;
BOOL				CPublicServer::ms_bRegistedAuthorizationFn	= FALSE;


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
	__in		LPTSTR							lpProtseq,
	__in		unsigned int					uMaxCalls,
	__in		LPTSTR							lpEndpoint,
	__in_opt	void						*	pSecurityDescriptor,
	__in		RPC_IF_HANDLE					RpcIfHandle,
	__in		unsigned int					uMinimumCallThreads,
	__in_opt	UUID						*	pMgrTypeUuid,
	__in_opt	RPC_MGR_EPV					*	pMgrEpv,
	__in		unsigned int					uDontWait,
	__in_opt	RPC_MGMT_AUTHORIZATION_FN		RpcMgmtAuthorizationFn
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

		if (RpcMgmtAuthorizationFn)
		{
			RpcStatus = RpcMgmtSetAuthorizationFn(RpcMgmtAuthorizationFn);
			if (RPC_S_OK != RpcStatus)
				__leave;

			ms_bRegistedAuthorizationFn = TRUE;
		}

		RpcStatus = RpcServerUseProtseqEp(
			(RPC_WSTR)lpProtseq,
			uMaxCalls,
			(RPC_WSTR)lpEndpoint,
			pSecurityDescriptor
			);    
		if (RPC_S_OK != RpcStatus)
			__leave;

		ms_pMgrTypeUuid = pMgrTypeUuid;
		RpcStatus = RpcServerRegisterIf(RpcIfHandle, ms_pMgrTypeUuid, pMgrEpv);
		if (RPC_S_OK != RpcStatus)
			__leave;

		if (uMinimumCallThreads >= uMaxCalls)
			__leave;

		OsVer = OsVersion.GetOSVer();
		if (OS_VER_WINDOWS_XP == OsVer)
			uMaxCalls = RPC_C_LISTEN_MAX_CALLS_DEFAULT;

		ms_bDontWait = uDontWait;

		if (ms_bDontWait)
		{
			RpcStatus = RpcServerListen(uMinimumCallThreads, uMaxCalls, ms_bDontWait); 
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

BOOL
	CPublicServer::Unload(
	__in BOOL bFromRpcInterface
	)
{
	BOOL		bRet		= FALSE;

	RPC_STATUS	RpcStatus	= RPC_S_OK;


	__try
	{
		if (ms_bRegistedAuthorizationFn)
		{
			RpcStatus = RpcMgmtSetAuthorizationFn(NULL);
			if (RPC_S_OK != RpcStatus)
				__leave;
		}

		RpcStatus = RpcMgmtStopServerListening(NULL);
		if (RPC_S_OK != RpcStatus)
			__leave;

		if (ms_bDontWait && !bFromRpcInterface)
		{
			RpcStatus = RpcMgmtWaitServerListen();
			if (RPC_S_OK != RpcStatus)
				__leave;
		}

		RpcStatus = RpcServerUnregisterIf(NULL, ms_pMgrTypeUuid, bFromRpcInterface ? FALSE : TRUE);
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

int
	CPublicServer::RpcMgmtAuthorizationFn(
	__in	RPC_BINDING_HANDLE		ClientBinding,
	__in	unsigned long			RequestedMgmtOperation,
	__out	RPC_STATUS __RPC_FAR *	Status
	)
{
	int nRet = FALSE;


	__try
	{
		if (!Status)
			__leave;

		if (!ClientBinding)
		{
			*Status = RPC_S_INVALID_ARG;
			__leave;
		}

		switch (RequestedMgmtOperation)
		{
		case RPC_C_MGMT_INQ_IF_IDS:
		case RPC_C_MGMT_INQ_PRINC_NAME:
		case RPC_C_MGMT_INQ_STATS:
		case RPC_C_MGMT_IS_SERVER_LISTEN:
		case RPC_C_MGMT_STOP_SERVER_LISTEN:
			{
				nRet = TRUE;
				break;
			}
		default:
			{
				*Status = RPC_S_OK;
				__leave;
			}
		}
	}
	__finally
	{
		;
	}

	return nRet;
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
