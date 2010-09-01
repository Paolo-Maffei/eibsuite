#ifndef __GENERIC_TEMPLATE_H__
#define __GENERIC_TEMPLATE_H__

/* All the return codes that can be returned by OpenConnection() function*/
#define CONN_SUCCESS 0
#define ERR_INVALID_CREDENTIALS -1
#define ERR_NO_EIB_SERVER_PRESENT -2
#define ERR_ALREADY_CONNECTED -3

/*3 function below already implemented and user can use them*/
/* return 0 on success. otherwise connection failed. */
int OpenConnection();
void CloseConnection();
int SendKnxMessage(unsigned short to, char* data, int data_len);

/* All of the function below must be implemented/modified*/
char* GetUserName();
char* GetPassword();
char* GetEIBServerAddress();
#ifdef WIN32
int GetNetworkIf();
#else
char* GetNetworkIf();
#endif
void OnReceiveKNXMessage(unsigned short from, unsigned short to, char* data, int data_len);

#endif
