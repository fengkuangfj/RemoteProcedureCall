#include "stdafx.h"


extern RpcServerInterface_v1_0_epv_t g_MgrEpv;


BOOL
	TestRpcCallback()
{
	BOOL				bRet			= FALSE;

	RPC_SERVER_STATUS	RpcServerStatus = RPC_SERVER_STATUS_FAILED;
	unsigned long		ExceptionCode	= RPC_S_OK;


	RpcTryExcept
	{
		RpcServerStatus = RpcCallback(_T("from server"));
		if (RPC_SERVER_STATUS_SUCCESS != RpcServerStatus)
			__leave;

		bRet = TRUE;
	}
	RpcExcept(ExceptionCode = RpcExceptionCode(), EXCEPTION_EXECUTE_HANDLER)
	{
		printf("[%s] RpcExcept (%d) \n", __FUNCTION__, ExceptionCode);
	}
	RpcEndExcept;

	return bRet;
}

RPC_STATUS
	RPC_ENTRY
	RpcIfCallbackFn(
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
	PVOID						pInterfaceAddress				= NULL;
	OS_VERSION_USER_DEFINED		OsVer							= OS_VERSION_UNKNOWN;
	ULONG						ulOpNum							= 0;


	__try
	{
		if (!Context)
			__leave;

		ClientBindingHandle = (RPC_IF_HANDLE)Context;

		OsVer = COperationSystemVersion::GetInstance()->GetOSVersion();
		if (OS_VERSION_UNKNOWN == OsVer)
			__leave;

		if (OS_VERSION_WINDOWS_XP == OsVer ||
			OS_VERSION_WINDOWS_XP_SP1 == OsVer ||
			OS_VERSION_WINDOWS_XP_SP2 == OsVer ||
			OS_VERSION_WINDOWS_XP_SP3 == OsVer)
		{
			RpcStatus = I_RpcBindingInqLocalClientPID(ClientBindingHandle, &uClientPid);
			if (RPC_S_OK != RpcStatus)
				__leave;

			ulOpNum = *(ULONG *)((ULONG_PTR)ClientBindingHandle + 0x134);
		}
		else
		{
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

			uClientPid = (ULONG)RpcCallAttributes.ClientPID;

			ulOpNum = RpcCallAttributes.OpNum;
		}

		pInterfaceAddress = (PVOID)*((ULONG_PTR *)&g_MgrEpv + ulOpNum);
		if (pInterfaceAddress == g_MgrEpv.RpcTest)
			printf("[%s] [client process]%d [current thread]%d RpcTest \n", __FUNCTION__, uClientPid, GetCurrentThreadId());
		else if (pInterfaceAddress == g_MgrEpv.RpcUseCallback)
			printf("[%s] [client process]%d [current thread]%d RpcUseCallback \n", __FUNCTION__, uClientPid, GetCurrentThreadId());
		else if (pInterfaceAddress == g_MgrEpv.RpcStopServer)
			printf("[%s] [client process]%d [current thread]%d RpcStopServer \n", __FUNCTION__, uClientPid, GetCurrentThreadId());
	}
	__finally
	{
		;
	}

	return RpcStatus;
}
