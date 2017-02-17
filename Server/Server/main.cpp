// Server.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


RpcServerInterface_v1_0_epv_t g_MgrEpv = {0};


int _tmain(int argc, _TCHAR* argv[])
{
	CPublicServer	PublicServer;

	TCHAR			tchProtseq[MAX_PATH]	= {0};
	TCHAR			tchEndpoint[MAX_PATH]	= {0};


	do 
	{
		COperationSystemVersion::GetInstance();

		_tcscat_s(tchProtseq, _countof(tchProtseq), RPC_PROT_SEQ);

		_tcscat_s(tchEndpoint, _countof(tchEndpoint), RPC_END_POINT);

		g_MgrEpv.RpcStopServer = RpcStopServer;
		g_MgrEpv.RpcTest = RpcTest;
		g_MgrEpv.RpcUseCallback = RpcUseCallback;

		if (!PublicServer.Init(
			RpcServerInterface_v1_0_s_ifspec,
			NULL,
			&g_MgrEpv,
			RPC_IF_SEC_NO_CACHE,
			0,
			0,
			RpcIfCallbackFn,
			tchProtseq,
			tchEndpoint,
			NULL,
			8,
			0,
			CPublicServer::RpcMgmtAuthorizationFn
			))
			break;
	} while (FALSE);

	_getch();

	PublicServer.Unload(FALSE);
	COperationSystemVersion::ReleaseInstance();

	return 0;
}
