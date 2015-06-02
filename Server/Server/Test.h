#pragma once

#ifndef _TEST_TAG_
#define _TEST_TAG_	'TSET'
#endif


BOOL
	TestRpcCallback();

RPC_STATUS
	RPC_ENTRY
	RpcIfCallbackFn(
	__in RPC_IF_HANDLE		InterfaceUuid,
	__in void			*	Context
	);
