#ifndef __GENERIC_TEMPLATE_H__
#define __GENERIC_TEMPLATE_H__

#ifdef __cplusplus
extern "C" {
#endif

	/* All the return codes that can be returned by OpenConnection() function*/
	#define CONN_SUCCESS 0
	#define ERR_INVALID_CREDENTIALS -1
	#define ERR_NO_EIB_SERVER_PRESENT -2
	#define ERR_ALREADY_CONNECTED -3
	#define ERR_NETWORK_ERROR -4

	/*3 function below already implemented and user can use them*/
	/* return 0 on success. otherwise connection failed. */
	int OpenConnection();
	void CloseConnection();
	int SendKnxMessage(unsigned short to, char* data, int data_len);

	/* All of the function below must be implemented/modified*/
	const char* GetEIBUserName();
	const char* GetPassword();
	const char* GetEncryptKey();
	const char* GetEIBServerAddress();
	int GetEIBServerPort();
	
	#ifdef WIN32
	int GetLocalNetworkIf();
	#else
	const char* GetLocalNetworkIf();
	#endif
	void OnReceiveKNXMessage(unsigned short dstAddr, char* data, int data_len);

#ifdef __cplusplus
}
#endif

#endif
