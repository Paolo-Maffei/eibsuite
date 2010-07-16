using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using System.IO;
using EIBConsole.Conf;
using System.Xml;
using System.Xml.Serialization;

namespace EIBConsole
{
    public class ConsoleAPI
    {
        private string _address;
        private int _port;
        private string _session_id;
      
        private HttpWebRequest _request;
        private HttpWebResponse _reply;

        public ConsoleAPI()
        {
            _session_id = string.Empty;
        }

        public LoginStatus Login(string server_ip, int server_port, string user_name, string password)
        {
            _address = server_ip;
            _port = server_port;

            string port_str = _port.ToString();
            
            //build the login url
            string url = "http://" + _address + ":" + port_str + "/" ;
            url += ConsoleDefinitions.EIB_CLIENT_AUTHENTICATE;

            _request = (HttpWebRequest)WebRequest.Create(url);

            _request.Headers.Add(ConsoleDefinitions.USER_NAME_HEADER, user_name);
            _request.Headers.Add(ConsoleDefinitions.PASSWORD_HEADER, password);
            //allocate cookie containter instance 
            _request.CookieContainer = new CookieContainer();

            try
            {
                _reply = (HttpWebResponse)_request.GetResponse();
            }
            catch (WebException e)
            {
                if (e.Response == null)
                {
                    return LoginStatus.ServerDown;
                }
                
                switch (((HttpWebResponse)e.Response).StatusCode)
                {
                    case HttpStatusCode.Forbidden: return LoginStatus.Denied;
                }
            }

            StreamReader sr = new StreamReader(_reply.GetResponseStream());
            string result = sr.ReadToEnd();

            //validate reply
            if (result == ConsoleDefinitions.EIB_CLIENT_AUTHENTICATE_SUCCESS)
            {
                if (_reply.Cookies[0].Name == ConsoleDefinitions.EIB_SESSION_ID_COOKIE_NAME)
                {
                    _session_id = _reply.Cookies[0].Value;
                }
                return LoginStatus.Success;
            }
            if (result == ConsoleDefinitions.EIB_CLIENT_AUTHENTICATE_FAILED)
            {
                return LoginStatus.Denied;
            }
            throw new Exception("Incorrect reply to login request");
        }

        public ClientsList GetClients()
        {
            ClientsList clnts = null;
            StreamReader sr = null;
            sr = GetEIBResponse(ConsoleDefinitions.EIB_SERVER_CLIENTS_STATUS_CMD);
            XmlSerializer ser = new XmlSerializer(typeof(ClientsList));
            clnts = (ClientsList)ser.Deserialize(sr);
            
            return clnts;
        }

        public EIBServerStatus GetServerStatus()
        {
            EIBServerStatus stat = null;
            StreamReader sr = null;
            sr = GetEIBResponse(ConsoleDefinitions.EIB_SERVER_CONF_CMD);
            XmlSerializer ser = new XmlSerializer(typeof(EIBServerStatus));
            stat = (EIBServerStatus)ser.Deserialize(sr);
            
            return stat;
        }

        public bool RestartServer()
        {
            GetEIBResponse(ConsoleDefinitions.EIB_SERVER_RELOAD_CMD);
            return true;
        }

        private StreamReader GetEIBResponse(string url)
        {
            string port_str = _port.ToString();
            _request = (HttpWebRequest)WebRequest.Create("http://" + _address + ":" + port_str + "/" + url);
            //set session id if exists
            _request.CookieContainer = new CookieContainer();
            Cookie c = new Cookie(ConsoleDefinitions.EIB_SESSION_ID_COOKIE_NAME,_session_id);
            c.Domain = _address;
            _request.CookieContainer.Add(c);
            
            //srround with try/catch
            _reply = (HttpWebResponse)_request.GetResponse();
            if (_reply.Cookies[0].Name == ConsoleDefinitions.EIB_SESSION_ID_COOKIE_NAME)
            {
                _session_id = _reply.Cookies[0].Value;
            }

            StreamReader sr = new StreamReader(_reply.GetResponseStream());
            return sr;
        }
    }

    public enum LoginStatus
    {
        Success,
        Denied,
        ServerDown,
    }
}
