// Server.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


extern RPC_IF_HANDLE RpcServerInterface_v1_0_s_ifspec;


int _tmain(int argc, _TCHAR* argv[])
{
	CPublicServer	PublicServer;

	TCHAR			tchProtseq[MAX_PATH]	= {0};
	TCHAR			tchEndpoint[MAX_PATH]	= {0};
	RPC_IF_HANDLE	RpcIfHandle				= NULL;
	UUID          *	pMgrTypeUuid			= NULL;
	RPC_MGR_EPV   *	pMgrEpv					= NULL;


	do 
	{
		_tcscat_s(tchProtseq, _countof(tchProtseq), _T("ncalrpc"));
		_tcscat_s(tchEndpoint, _countof(tchEndpoint), _T("{8dd50205-3108-498f-96e8-dbc4ec074cf9}"));

		RpcIfHandle = RpcServerInterface_v1_0_s_ifspec;

		PublicServer.Init(
			tchProtseq,
			20, 
			tchEndpoint,
			RpcIfHandle,
			19,
			pMgrTypeUuid,
			pMgrEpv,
			0
			);

		_getch();
	} while (FALSE);
		
	return 0;
}
