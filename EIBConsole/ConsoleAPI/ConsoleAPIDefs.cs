namespace EIBConsole
{
    public static class ConsoleDefinitions
    {
        public static class GlobalConf
        {
            //Login definitions
            public const string EIB_CLIENT_AUTHENTICATE = "AUTHENTICATE_USER";
            public const string EIB_CLIENT_AUTHENTICATE_SUCCESS = "USER_AUTHENTICATE_SUCCESS";
            public const string EIB_CLIENT_AUTHENTICATE_FAILED = "USER_AUTHENTICATE_FAILED";
            //Logout definitions
            public const string EIB_CLIENT_DISCONNECT = "DISCONNECT_USER";
            public const string EIB_CLIENT_DISCONNECT_SUCCESS = "USER_DISCONNECT_SUCCESS";
            public const string EIB_CLIENT_DISCONNECT_FAILED = "USER_DISCONNECT_FAILED";
            //Keep-Alive definitions
            public const string EIB_CONSOLE_KEEP_ALIVE_CMD = "EIB_CONSOLE_KEEP_ALIVE_CMD";

            public const string USER_NAME_HEADER = "User-Name";
            public const string PASSWORD_HEADER = "Password";
            public const string EIB_SESSION_ID_COOKIE_NAME = "EIBSESSIONID";
        }
        //users list
        public static class EIBServerUsersConf
        {
            //commands
            public const string EIB_SERVER_GET_USERS_CONF_CMD = "EIB_SERVER_GET_USERS_CONF_CMD";
            public const string EIB_SERVER_SET_USERS_CONF_CMD = "EIB_SERVER_SET_USERS_CONF_CMD";
            //defs
            public const string EIB_SERVER_USERS_LIST_XML = "EIB_SERVER_USERS_LIST";
            public const string EIB_SERVER_USER_XML = "EIB_SERVER_USER";
            public const string EIB_SERVER_USER_IP_ADDRESS_XML = "EIB_SERVER_USER_IP_ADDRESS";
            public const string EIB_SERVER_USER_NAME_XML = "EIB_SERVER_USER_NAME";
            public const string EIB_SERVER_USER_PASSWORD_XML = "EIB_SERVER_USER_PASSWORD";
            public const string EIB_SERVER_USER_IS_CONNECTED_XML = "EIB_SERVER_USER_IS_CONNECTED";
            public const string EIB_SERVER_USER_SESSION_ID_XML = "EIB_SERVER_USER_SESSION_ID";
            public const string EIB_SERVER_USER_PRIVILIGES_XML = "EIB_SERVER_USER_PRIVILIGES";
            public const string EIB_SERVER_USER_SOURCE_ADDR_MASK_XML = "EIB_SERVER_USER_SOURCE_ADDR_MASK";
            public const string EIB_SERVER_USER_DST_ADDR_MASK_XML = "EIB_SERVER_USER_DST_ADDR_MASK";
        }
        //eib interface conf defs
        public static class EIBInterfaceConf
        {
            //commands
            public const string EIB_SERVER_GET_INTERFACE_CONF_CMD = "EIB_SERVER_GET_INTERFACE_CONF_CMD";
            public const string EIB_SERVER_SET_INTERFACE_CONF_CMD = "EIB_SERVER_SET_INTERFACE_CONF_CMD";
            public const string EIB_SERVER_INTERFACE_STOP_CMD = "EIB_SERVER_INTERFACE_STOP_CMD";
            public const string EIB_SERVER_INTERFACE_START_CMD = "EIB_SERVER_INTERFACE_START_CMD";

            //defs
            public const string EIB_INTERFACE_ADDRESS_XML = "EIB_INTERFACE_ADDRESS";
            public const string EIB_INTERFACE_PORT_XML = "EIB_INTERFACE_PORT";
            public const string EIB_INTERFACE_DEVICE_MODE_XML = "EIB_INTERFACE_MODE";
            public const string EIB_INTERFACE_AUTO_DETECT_XML = "EIB_INTERFACE_AUTO_DETECT";
            public const string EIB_INTERFACE_LAST_TIME_PACKET_SENT_XML = "EIB_INTERFACE_LAST_TIME_PACKET_SENT";
            public const string EIB_INTERFACE_LAST_TIME_PACKET_RECEIVED_XML = "EIB_INTERFACE_LAST_TIME_PACKET_RECEIVED";
            public const string EIB_INTERFACE_TOTAL_PACKETS_SENT_XML = "EIB_INTERFACE_TOTAL_PACKETS_SENT";
            public const string EIB_INTERFACE_TOTAL_PACKETS_RECEIVED_XML = "EIB_INTERFACE_TOTAL_PACKETS_RECEIVED";
            public const string EIB_INTERFACE_RUNNING_STATUS_XML = "EIB_INTERFACE_RUNNING_STATUS";
        }
        //bus monitor defs
        public static class EIBBusMonConf
        {
            //commands
            public const string EIB_BUS_MON_GET_ADDRESSES_CMD = "EIB_BUS_MON_GET_ADDRESSES_CMD";
            public const string EIB_BUS_MON_GET_ADDRESS_INFO_CMD = "EIB_BUS_MON_GET_ADDRESS_INFO_CMD";
            public const string EIB_BUS_MON_SEND_CMD_TO_ADDR_CMD = "EIB_BUS_MON_SEND_CMD_TO_ADDR_CMD";

            //defs
            public const string EIB_BUS_MON_ADDRESSES_LIST_XML = "EIB_BUS_MON_ADDRESSES_LIST";
            public const string EIB_BUS_MON_ADDRESS_XML = "EIB_BUS_MON_ADDRESS";
            public const string EIB_BUS_MON_ADDRESS_STR_XML = "EIB_BUS_MON_ADDRESS_STR";
            public const string EIB_BUS_MON_IS_ADDRESS_LOGICAL_XML = "EIB_BUS_MON_IS_ADDRESS_LOGICAL";
            public const string EIB_BUS_MON_ADDR_LAST_RECVED_TIME_XML = "EIB_BUS_MON_ADDR_LAST_RECVED_TIME";
            public const string EIB_BUS_MON_LAST_ADDR_VALUE_XML = "EIB_BUS_MON_LAST_ADDR_VALUE";
            public const string EIB_BUS_MON_ADDRESSES_COUNT_XML = "EIB_BUS_MON_ADDRESSES_COUNT";
            //send cmd defs
            public const string EIB_BUS_MON_SEND_CMD_ADDR_PARAM = "SND_DEST_ADDR";
            public const string EIB_BUS_MON_SEND_CMD_VAL_PARAM  = "SND_VAL";
            public const string EIB_BUS_MON_SEND_MODE_CMD_PARAM = "SND_MODE";
        }
    }
}