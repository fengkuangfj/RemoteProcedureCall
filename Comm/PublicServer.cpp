#include "PublicServer.h"


unsigned int		CPublicServer::ms_bDontWait					= 0;
UUID			*	CPublicServer::ms_pMgrTypeUuid				= NULL;
BOOL				CPublicServer::ms_bRegistedAuthorizationFn	= FALSE;
BOOL				CPublicServer::ms_bUnloaded					= FALSE;


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
	__in		RPC_IF_HANDLE					RpcIfHandle,
	__in_opt	UUID						*	pMgrTypeUuid,
	__in_opt	RPC_MGR_EPV					*	pMgrEpv,
	__in		unsigned int					uFlags,
	__in_opt	unsigned int					uMaxCalls,
	__in_opt	unsigned int					uMaxRpcSize,
	__in_opt	RPC_IF_CALLBACK_FN			*	pIfCallbackFn,
	__in		LPTSTR							lpProtseq,
	__in		LPTSTR							lpEndpoint,
	__in_opt	void						*	pSecurityDescriptor,
	__in		unsigned int					uMinimumCallThreads,
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
		if (!RpcIfHandle ||
			!uFlags ||
			!lpProtseq ||
			!lpEndpoint ||
			!uMinimumCallThreads)
			__leave;

		// 1��Register the interface
		ms_pMgrTypeUuid = pMgrTypeUuid;

		if (!uMaxRpcSize)
			uMaxRpcSize = -1;

		if (!pIfCallbackFn)
			pIfCallbackFn = RpcIfCallbackFn;

		RpcStatus = RpcServerRegisterIf2(
			RpcIfHandle,
			ms_pMgrTypeUuid,
			pMgrEpv,
			uFlags,
			uMaxCalls,
			uMaxRpcSize,
			pIfCallbackFn
			);
		if (RPC_S_OK != RpcStatus)
			__leave;

		// 2��Create binding information
		// 3��Register the endpoints
		RpcStatus = RpcServerUseProtseqEp(
			(RPC_WSTR)lpProtseq,
			uMaxCalls,
			(RPC_WSTR)lpEndpoint,
			pSecurityDescriptor
			);
		if (RPC_S_OK != RpcStatus)
			__leave;

		// ������Ȩ�ص�����
		if (RpcMgmtAuthorizationFn)
		{
			RpcStatus = RpcMgmtSetAuthorizationFn(RpcMgmtAuthorizationFn);
			if (RPC_S_OK != RpcStatus)
				__leave;

			ms_bRegistedAuthorizationFn = TRUE;
		}

		// 4��Listen for client calls
		OsVer = OsVersion.GetOSVer();
		if (OS_VER_WINDOWS_XP == OsVer || !uMaxCalls)
			uMaxCalls = RPC_C_LISTEN_MAX_CALLS_DEFAULT;

		if (uMinimumCallThreads >= uMaxCalls)
			__leave;

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
		if (!ms_bUnloaded)
		{
			if (ms_bRegistedAuthorizationFn)
			{
				RpcStatus = RpcMgmtSetAuthorizationFn(NULL);
				if (RPC_S_OK != RpcStatus)
					__leave;
			}

			RpcStatus = RpcMgmtStopServerListening(NULL);
			if (RPC_S_OK != RpcStatus && RPC_S_NOT_LISTENING != RpcStatus)
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

			ms_bUnloaded = TRUE;
		}

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

RPC_STATUS
	CPublicServer::RpcIfCallbackFn(
	__in RPC_IF_HANDLE		InterfaceUuid,
	__in void			*	Context
	)
{
	RPC_STATUS					RpcStatus						= RPC_S_OK;

	RPC_IF_HANDLE				ClientBindingHandle				= NULL;
	unsigned long				uClientPid						= 0;
	RPC_CALL_ATTRIBUTES_V2		RpcCallAttributes				= {0};
	CHAR						chServerPrincipalName[MAX_PATH]	= {0};
	CHAR						chClientPrincipalName[MAX_PATH]	= {0};
	RPC_CALL_LOCAL_ADDRESS_V1	CallLocalAddress				= {0};
	CHAR						chBuffer[MAX_PATH]				= {0};


	__try
	{
		if (!Context)
			__leave;

		ClientBindingHandle = (RPC_IF_HANDLE)Context;

		RpcStatus = I_RpcBindingInqLocalClientPID(ClientBindingHandle, &uClientPid);
		if (RPC_S_OK != RpcStatus)
			__leave;

		RpcCallAttributes.Version = RPC_CALL_ATTRIBUTES_VERSION;

		RpcCallAttributes.Flags |= RPC_QUERY_SERVER_PRINCIPAL_NAME;
		RpcCallAttributes.ServerPrincipalName = (unsigned short *)chServerPrincipalName;
		RpcCallAttributes.ServerPrincipalNameBufferLength = sizeof(chServerPrincipalName);

		RpcCallAttributes.Flags |= RPC_QUERY_CLIENT_PRINCIPAL_NAME;
		RpcCallAttributes.ClientPrincipalName = (unsigned short *)chClientPrincipalName;
		RpcCallAttributes.ClientPrincipalNameBufferLength = sizeof(chClientPrincipalName);

		RpcCallAttributes.Flags |= RPC_QUERY_CLIENT_PID;

		RpcCallAttributes.Flags |= RPC_QUERY_IS_CLIENT_LOCAL;

		RpcCallAttributes.Flags |= RPC_QUERY_NO_AUTH_REQUIRED;

		RpcStatus = RpcServerInqCallAttributes(ClientBindingHandle, &RpcCallAttributes);
		if (RPC_S_OK != RpcStatus)
			__leave;

		printf("[%s] %d \n", __FUNCTION__, uClientPid);
	}
	__finally
	{
		;
	}

	return RpcStatus;
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
