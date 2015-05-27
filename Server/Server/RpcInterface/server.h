

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Wed May 27 19:21:23 2015
 */
/* Compiler settings for Server.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __Server_h__
#define __Server_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __RpcServerInterface_INTERFACE_DEFINED__
#define __RpcServerInterface_INTERFACE_DEFINED__

/* interface RpcServerInterface */
/* [implicit_handle][version][uuid] */ 

typedef 
enum _RPC_SERVER_STATUS
    {	RPC_SERVER_STATUS_SUCCESS	= 0,
	RPC_SERVER_STATUS_FAILED	= ( RPC_SERVER_STATUS_SUCCESS + 1 ) 
    } 	RPC_SERVER_STATUS;

typedef enum _RPC_SERVER_STATUS *PRPC_SERVER_STATUS;

typedef enum _RPC_SERVER_STATUS *LPRPC_SERVER_STATUS;

RPC_SERVER_STATUS TestInterface( 
    /* [string][in] */ unsigned char *puch);

RPC_SERVER_STATUS StopRpcServer( void);


extern handle_t RpcServerInterface_Binding;


extern RPC_IF_HANDLE RpcServerInterface_v1_0_c_ifspec;
extern RPC_IF_HANDLE RpcServerInterface_v1_0_s_ifspec;
#endif /* __RpcServerInterface_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


