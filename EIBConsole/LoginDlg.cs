using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Net;
using System.IO;
using System.Windows.Forms;
using IPAddressControlLib;
using System.Threading;

namespace EIBConsole
{
    public partial class LoginDlg : Form
    {
        public LoginDlg()
        {
            InitializeComponent();
        }

        public bool ValidateInput()
        {
            return true;
        }

        public string ServerAddress
        {
            get { return tbServerAddress.Text; }
        }

        public int ServerPort
        {
            get 
            {
                try
                {
                    int result = int.Parse(tbServerPort.Text);
                    return result;
                }
                catch (Exception)
                {
                    return -1;
                }
            }
        }

        public string UserName
        {
            get { return tbUserName.Text; }
        }

        public string Password
        {
            get { return tbPassword.Text; }
        }

        public void Login()
        {
            //validate the input strings here...
            if (!ValidateInput())
            {
                //show error message
                //set flag
                ConnectionParams.Instance.Status = LoginStatus.Disconncted;
            }

            //build the login url
            string url = "http://";
            url += ServerAddress;
            url += ":";
            url += ServerPort.ToString() + "/";
            url += ConsoleDefinitions.GlobalConf.EIB_CLIENT_AUTHENTICATE;

            HttpWebRequest request = (HttpWebRequest)WebRequest.Create(url);
            //EIB server should respond in 3 seconds...
            request.Timeout = 3000;
            request.Headers.Add(ConsoleDefinitions.GlobalConf.USER_NAME_HEADER, UserName);
            request.Headers.Add(ConsoleDefinitions.GlobalConf.PASSWORD_HEADER, Password);
            //allocate cookie containter instance 
            request.CookieContainer = new CookieContainer();

            HttpWebResponse reply = null;
            try
            {
                reply = (HttpWebResponse)request.GetResponse();
            }
            catch (WebException e)
            {
                if (e.Response == null)
                {
                    ConnectionParams.Instance.Status = LoginStatus.ServerDown;
                    return;
                }

                switch (((HttpWebResponse)e.Response).StatusCode)
                {
                    case HttpStatusCode.Forbidden:
                        ConnectionParams.Instance.Status = LoginStatus.ConnectionDenied;
                        return;
                }
            }

            StreamReader sr = new StreamReader(reply.GetResponseStream());
            string result = sr.ReadToEnd();

            //validate reply
            if (result == ConsoleDefinitions.GlobalConf.EIB_CLIENT_AUTHENTICATE_SUCCESS)
            {
                if (reply.Cookies[ConsoleDefinitions.GlobalConf.EIB_SESSION_ID_COOKIE_NAME] != null)
                {
                    ConnectionParams.Instance.SessionID = reply.Cookies[ConsoleDefinitions.GlobalConf.EIB_SESSION_ID_COOKIE_NAME].Value;
                    ConnectionParams.Instance.HttpAddress = "http://" + ServerAddress + ":" + ServerPort.ToString() + "/";
                    ConnectionParams.Instance.Domain = ServerAddress;
                    ConnectionParams.Instance.Status = LoginStatus.Connected;
                }
                else
                {
                    ConnectionParams.Instance.Status = LoginStatus.Disconncted;
                    ConnectionParams.Instance.HttpAddress = String.Empty;
                    ConnectionParams.Instance.SessionID = String.Empty;
                }
                
            }
            else if (result == ConsoleDefinitions.GlobalConf.EIB_CLIENT_AUTHENTICATE_FAILED)
            {
                ConnectionParams.Instance.Status = LoginStatus.ConnectionDenied;
                ConnectionParams.Instance.HttpAddress = String.Empty;
                ConnectionParams.Instance.SessionID = String.Empty;
            }
            else
            {
                throw new Exception("Incorrect reply to login request");
            }
        }

        public void LogOut()
        {
            string res = ConsoleAPI.Disconnect();
        }
    }

    public class ConnectionParams
    {
        private static ConnectionParams _instance = null;
        private ConnectionParams()
        {
            //initial status is "Disconnected".
            Status = LoginStatus.Disconncted;
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
