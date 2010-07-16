using System;
using System.Linq;
using System.Collections.Generic;
using System.Text;

namespace EIBMobile.Connection
{
    public class ConnectionParams
    {
        private static ConnectionParams _instance = null;
        private ConnectionParams()
        {
        }

        public static ConnectionParams Instance
        {
            get
            {
                if (_instance == null)
                {
                    _instance = new ConnectionParams();
                }
                return _instance;
            }
        }
        public string Domain { get; set; }
        public string HttpAddress { get; set; }
        public string SessionID { get; set; }
        public LoginStatus Status { get; set; }
    }

    public enum LoginStatus
    {
        Connected,
        Disconncted,
        ServerDown,
        ConnectionDenied
    }
}
