#pragma once

#ifndef _TEST_TAG_
#define _TEST_TAG_	'TSET'
#endif


BOOL
	TestRpcTest();

BOOL
	TestRpcStopServer(
	__in		BOOL				bDirectlyStop,
	__in_opt	RPC_BINDING_HANDLE	RpcBindingHandle
	);
