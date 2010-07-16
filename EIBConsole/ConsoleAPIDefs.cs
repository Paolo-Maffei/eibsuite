namespace EIBConsole
{
    public static class ConsoleDefinitions
    {     
        //Login defintions
        public const string EIB_CLIENT_AUTHENTICATE = "AUTHENTICATE_USER";
        public const string EIB_CLIENT_AUTHENTICATE_SUCCESS = "USER_AUTHENTICATE_SUCCESS";
        public const string EIB_CLIENT_AUTHENTICATE_FAILED = "USER_AUTHENTICATE_FAILED";

        public const string USER_NAME_HEADER = "User-Name";
        public const string PASSWORD_HEADER = "Password";
        public const string EIB_SESSION_ID_COOKIE_NAME = "EIBSESSIONID";

        //get/set server configuration
        public const string EIB_SERVER_CONF_CMD = "EIB_SERVER_CONF_CMD";
        public const string EIB_INITIAL_KEY_XML = "EIB_INITIAL_KEY";
        public const string EIB_IP_ADDRESS_XML = "EIB_IP_ADDRESS";
        public const string EIB_SERVER_POLL_TIME_XML = "EIB_SERVER_POLL_TIME";
        public const string LISTENING_PORT_XML = "LISTENING_PORT";
        public const string EIB_PORT_XML = "EIB_PORT";
        public const string EIB_DELAY_TIME_POLL_XML = "EIB_DELAY_TIME_POLL";
        public const string MAX_CONCURRENT_CLIENTS_XML = "MAX_CONCURRENT_CLIENTS";
        public const string HEART_BEAT_TIMEOUT_XML = "HEART_BEAT_TIMEOUT";
        public const string CONSOLE_MANAGER_PORT_XML = "CONSOLE_MANAGER_PORT";
        public const string LOG_LEVEL_XML = "LOG_LEVEL";

        //retsart server
        public const string EIB_SERVER_RELOAD_CMD = "EIB_SERVER_RELOAD_CMD";

        //clients list
        public const string EIB_SERVER_CLIENTS_STATUS_CMD = "EIB_SERVER_CLIENTS_STATUS_CMD";
        public const string CLIENTS_LIST_XML = "CLIENTS_LIST";
        public const string CLIENT_XML = "CLIENT";
        public const string CLIENT_IP_ADDRESS_XML = "CLIENT_IP_ADDRESS";
        public const string CLIENT_NAME_XML = "CLIENT_NAME";
        public const string CLIENT_PASSWORD_XML = "CLIENT_PASSWORD";
        public const string CLIENT_IS_CONNECTED_XML = "CLIENT_IS_CONNECTED";
        public const string CLIENT_SESSION_ID_XML = "CLIENT_SESSION_ID";
        public const string CLIENT_PRIVILIGES_XML = "CLIENT_PRIVILIGES";
    }
}