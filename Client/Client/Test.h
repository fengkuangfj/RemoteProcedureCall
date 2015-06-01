#pragma once

#ifndef _TEST_TAG_
#define _TEST_TAG_	'TSET'
#endif


BOOL
	TestRpcTest();

BOOL
	TestRpcUseCallback();

static
	DWORD
	WINAPI
	TestRpcUseCallbackThread(
	LPVOID lpThreadParameter
	);

BOOL
	TestRpcStopServer(
	__in		BOOL				bDirectlyStop,
	__in_opt	RPC_BINDING_HANDLE	RpcBindingHandle
	);
