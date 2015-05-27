
{
	unsigned char * pszStringBinding = NULL;
	if ( argc != 2 )
	{
		printf("Usage:%s <Hello Text>\n", argv[0]);
		return 1;
	}    

	// ��Named Pipe ��ΪRPC ��ͨ�����μ�server.c �е�RpcServerUseProtseqEp() ����
	// ��3 ������NetworkAddr ���ȡNULL����ô�������ӱ�������
	// ����Ҫȡ\\\\servername �����ĸ�ʽ��������ļ������Ϊjack����ô����\\jack
	RpcStringBindingCompose( NULL, (unsigned char*)"ncacn_np", /*(unsigned char*)"\\\\servername"*/ NULL, (unsigned char*)"\\pipe\\{8dd50205-3108-498f-96e8-dbc4ec074cf9}", NULL, &pszStringBinding );

	// �󶨽ӿڣ�����Ҫ�� Hello.acf ������һ�£���ô����HelloWorld_Binding
	RpcBindingFromStringBinding(pszStringBinding, & HelloWorld_Binding );    
}


// ����ĺ�����Ϊ������������Ҫ��д�ģ�û�еĻ���������Ӵ���
void __RPC_FAR* __RPC_USER midl_user_allocate(size_t len)
{
	return(malloc(len));
}

void __RPC_USER midl_user_free(void __RPC_FAR *ptr)
{
	free(ptr);
}