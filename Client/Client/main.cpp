// Client.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	CPublicClient			PublicClient;

	LPTSTR					lpObjUuid				= NULL;
	TCHAR					tchProtSeq[MAX_PATH]	= {0};
	LPTSTR					lpNetworkAddr			= NULL;
	TCHAR					tchEndPoint[MAX_PATH]	= {0};
	LPTSTR					lpOptions				= NULL;
	LPTSTR					plpStringBinding		= NULL;
	RPC_BINDING_HANDLE		pRpcBindingHandle		= NULL;


	do 
	{
		_tcscat_s(tchProtSeq, _countof(tchProtSeq), RPC_PROT_SEQ);
		_tcscat_s(tchEndPoint, _countof(tchEndPoint), RPC_END_POINT);

		if (!PublicClient.Init(
			lpObjUuid,
			tchProtSeq,
			lpNetworkAddr,
			tchEndPoint,
			lpOptions,
			&plpStringBinding,
			&RpcServerInterface_Binding
			))
			break;

		Test();
	} while (FALSE);

	_getch();

	return 0;
}
