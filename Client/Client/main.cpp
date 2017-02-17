// Client.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	TCHAR			tchProtSeq[MAX_PATH] = { 0 };
	TCHAR			tchEndPoint[MAX_PATH] = { 0 };


	do
	{
		printf("[%s] [current process]%d [current thread]%d \n", __FUNCTION__, GetCurrentProcessId(), GetCurrentThreadId());

		_tcscat_s(tchProtSeq, _countof(tchProtSeq), RPC_PROT_SEQ);
		_tcscat_s(tchEndPoint, _countof(tchEndPoint), RPC_END_POINT);

		if (!CRpcClient::GetInstance(
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

		if (!TestRpcTest())
			break;

		if (!TestRpcTest())
			break;

		if (!TestRpcTest())
			break;

		if (!TestRpcTest())
			break;

		if (!TestRpcUseCallback())
			break;
	} while (FALSE);

	_getch();

	TestRpcStopServer(FALSE, RpcServerInterface_Binding);

	CRpcClient::ReleaseInstance();

	return 0;
}
