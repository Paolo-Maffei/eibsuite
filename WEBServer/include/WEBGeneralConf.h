CONF_ENTRY(CString,InitialKey,"WEB_INITIAL_KEY","EIBKEY")
CONF_ENTRY(bool, AutoDiscoverEibServer,"AUTO_DISCOVER_EIB_SERVER", true)
CONF_ENTRY(CString,EibIPAddress,"EIB_SERVER_IP_ADDRESS","127.0.0.1")
CONF_ENTRY(int,EibPort,"EIB_SERVER_PORT",5000)
CONF_ENTRY(int,WEBServerPort,"WEB_SERVER_PORT",8080)
CONF_ENTRY(CString,ImagesFolder,"WEB_SERVER_IMAGES_FOLDER","C:\\Documents and Settings\\Yosi\\Desktop\\EIBProject\\WEBServer\\Images")
CONF_ENTRY(CString,Name,"WEB_SERVER_NAME","WEB")
CONF_ENTRY(CString,NetworkName,"WEB_NETWORK_NAME","WEB")
CONF_ENTRY(CString,Password,"WEB_SERVER_PASSWORD","WEB")
#ifdef WIN32
CONF_ENTRY(int,ListenInterface,"LISTEN_INTERFACE",1)
#else
CONF_ENTRY(CString,ListenInterface,"LISTEN_INTERFACE","eth0")
#endif
