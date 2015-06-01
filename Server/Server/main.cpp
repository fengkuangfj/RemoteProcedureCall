// Server.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


extern RPC_IF_HANDLE RpcServerInterface_v1_0_s_ifspec;


int _tmain(int argc, _TCHAR* argv[])
{
	CPublicServer	PublicServer;

	TCHAR			tchProtseq[MAX_PATH]	= {0};
	TCHAR			tchEndpoint[MAX_PATH]	= {0};


	do 
	{
		_tcscat_s(tchProtseq, _countof(tchProtseq), RPC_PROT_SEQ);
		_tcscat_s(tchEndpoint, _countof(tchEndpoint), RPC_END_POINT);

		if (!PublicServer.Init(
			RpcServerInterface_v1_0_s_ifspec,
			NULL,
			NULL,
			RPC_IF_SEC_NO_CACHE,
			0,
			0,
			CPublicServer::RpcIfCallbackFn,
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

	return 0;
}
