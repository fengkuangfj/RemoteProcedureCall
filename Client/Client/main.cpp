// Client.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	CPublicClient	PublicClient;

	TCHAR			tchProtSeq[MAX_PATH]	= {0};
	TCHAR			tchEndPoint[MAX_PATH]	= {0};


	do 
	{
		_tcscat_s(tchProtSeq, _countof(tchProtSeq), RPC_PROT_SEQ);
		_tcscat_s(tchEndPoint, _countof(tchEndPoint), RPC_END_POINT);

		if (!PublicClient.Init(
			NULL,
			tchProtSeq,
			NULL,
			tchEndPoint,
			NULL,
			&RpcServerInterface_Binding
			))
			break;

		if (!TestRpcTest())
			break;

		if (!TestRpcUseCallback())
			break;
	} while (FALSE);

	_getch();

	TestRpcStopServer(TRUE, RpcServerInterface_Binding);

	PublicClient.Unload(&RpcServerInterface_Binding);

	return 0;
}
