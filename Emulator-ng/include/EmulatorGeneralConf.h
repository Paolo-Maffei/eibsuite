CONF_ENTRY(int,EibPort,"EIB_PORT",3671)
CONF_ENTRY(int,LogLevel,"LOG_LEVEL",3)
#ifdef WIN32
CONF_ENTRY(int,ListenInterface,"LISTEN_INTERFACE",1)
#else
CONF_ENTRY(CString,ListenInterface,"LISTEN_INTERFACE","eth0")
#endif
