using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using System.IO;
using EIBMobile.Conf;
using System.Xml;
using System.Xml.Serialization;
using EIBMobile.Connection;

namespace EIBMobile
{
    public static class ConsoleAPI
    {
        public static object _lock_obj = new object();
        
        public static EIBUsersConf GetUsersList()
        {
            try
            {
                EIBUsersConf users = null;
                StreamReader sr = null;
                sr = GetEIBResponse(ConsoleDefinitions.EIBServerUsersConf.EIB_SERVER_GET_USERS_CONF_CMD, string.Empty);
                XmlSerializer ser = new XmlSerializer(typeof(EIBUsersConf));
                users = (EIBUsersConf)ser.Deserialize(sr);
                return users;
            }
            catch (Exception)
            {
                return null;
            }
        }

        public static bool SetUsersList(EIBUsersConf users)
        {
            //write object into xml form
            MemoryStream ms = new MemoryStream();
            XmlSerializer ser = new XmlSerializer(typeof(EIBUsersConf));
            ser.Serialize(ms, users);
            //get the xml string
            string content = ASCIIEncoding.ASCII.GetString(ms.ToArray(),0,ms.ToArray().Length);
            StreamReader sr = GetEIBResponse(ConsoleDefinitions.EIBServerUsersConf.EIB_SERVER_SET_USERS_CONF_CMD, content);
            if (sr != null && sr.ReadToEnd().Length == 0)
            {
                return true;
            }
            return false;
        }

        public static EIBInterfaceConf GetEIBInterfaceConf()
        {
            try
            {
                EIBInterfaceConf conf_obj = null;
                StreamReader sr = null;
                sr = GetEIBResponse(ConsoleDefinitions.EIBInterfaceConf.EIB_SERVER_GET_INTERFACE_CONF_CMD, string.Empty);
                XmlSerializer ser = new XmlSerializer(typeof(EIBInterfaceConf));
                conf_obj = (EIBInterfaceConf)ser.Deserialize(sr);
                sr.Close();
                return conf_obj;
            }
            catch (Exception)
            {
                return null;
            }
        }

        public static string Disconnect()
        {
            string res = string.Empty;
            StreamReader sr = null;
            sr = GetEIBResponse(ConsoleDefinitions.GlobalConf.EIB_CLIENT_DISCONNECT, string.Empty);
            if (sr != null)
            {
                res = sr.ReadToEnd();
            }
            return res;
        }

        public static void Connect(string address,int port,string user_name,string password)
        {
            //build the login url
            string url = "http://";
            url += address;
            url += ":";
            url += port.ToString() + "/";
            url += ConsoleDefinitions.GlobalConf.EIB_CLIENT_AUTHENTICATE;

            Uri uri = new Uri(url);
            HttpWebRequest request = (HttpWebRequest)WebRequest.Create(uri);

            request.Headers.Add(ConsoleDefinitions.GlobalConf.USER_NAME_HEADER, user_name);
            request.Headers.Add(ConsoleDefinitions.GlobalConf.PASSWORD_HEADER, password);
            
            HttpWebResponse reply = null;
            StreamReader sr = null;
            string result = string.Empty;
            try
            {
                reply = (HttpWebResponse)request.GetResponse();
                sr = new StreamReader(reply.GetResponseStream());
                result = sr.ReadToEnd();
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

                throw new Exception("WEB Excption...");
            }

            //validate reply
            if (result == ConsoleDefinitions.GlobalConf.EIB_CLIENT_AUTHENTICATE_SUCCESS)
            {
                string cookie = reply.Headers["Set-Cookie"];
                if (cookie != null && cookie.StartsWith(ConsoleDefinitions.GlobalConf.EIB_SESSION_ID_COOKIE_NAME))
                {
                    Cookie c = new Cookie(cookie);
                    ConnectionParams.Instance.SessionID = c.Value;
                    ConnectionParams.Instance.HttpAddress = "http://" + address + ":" + port.ToString() + "/";
                    ConnectionParams.Instance.Domain = address;
                    ConnectionParams.Instance.Status = LoginStatus.Connected;
                    reply.Close();
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

        public static EIBInterfaceConf StartEIBInterface()
        {
            try
            {
                EIBInterfaceConf conf_obj = null;
                StreamReader sr = null;
                sr = GetEIBResponse(ConsoleDefinitions.EIBInterfaceConf.EIB_SERVER_INTERFACE_START_CMD, string.Empty);
                XmlSerializer ser = new XmlSerializer(typeof(EIBInterfaceConf));
                conf_obj = (EIBInterfaceConf)ser.Deserialize(sr);
                return conf_obj;
            }
            catch (Exception)
            {
                return null;
            }
        }

        public static EIBInterfaceConf StopEIBInterface()
        {
            try
            {
                EIBInterfaceConf conf_obj = null;
                StreamReader sr = null;
                sr = GetEIBResponse(ConsoleDefinitions.EIBInterfaceConf.EIB_SERVER_INTERFACE_STOP_CMD, string.Empty);
                XmlSerializer ser = new XmlSerializer(typeof(EIBInterfaceConf));
                conf_obj = (EIBInterfaceConf)ser.Deserialize(sr);
                return conf_obj;
            }
            catch (Exception)
            {
                return null;
            }
        }

        public static bool GetKeepAliveStatus()
        {
            bool res = false;
            StreamReader sr = null;
            sr = GetEIBResponse(ConsoleDefinitions.GlobalConf.EIB_CONSOLE_KEEP_ALIVE_CMD, string.Empty);
            if (sr != null && sr.ReadToEnd() == "OK")
            {
                res = true;
            }
            return res;
        }

        public static EIBBusMonAddressListConf GetAddressList()
        {
            try
            {
                EIBBusMonAddressListConf conf_obj = null;
                StreamReader sr = null;
                sr = GetEIBResponse(ConsoleDefinitions.EIBBusMonConf.EIB_BUS_MON_GET_ADDRESSES_CMD, string.Empty);
                XmlSerializer ser = new XmlSerializer(typeof(EIBBusMonAddressListConf));
                conf_obj = (EIBBusMonAddressListConf)ser.Deserialize(sr);
                return conf_obj;
            }
            catch (Exception)
            {
                return null;
            }
        }

        private static StreamReader GetEIBResponse(string url, string content)
        {
            StreamReader sr = null;
            lock (ConsoleAPI._lock_obj)
            {
                if (ConnectionParams.Instance.Status != LoginStatus.Connected)
                {
                    return null;
                }

                Uri uri = new Uri(ConnectionParams.Instance.HttpAddress + url);
                HttpWebRequest request = (HttpWebRequest)WebRequest.Create(uri);
                //set session id if exists

                request.Headers.Add("Cookie", ConsoleDefinitions.GlobalConf.EIB_SESSION_ID_COOKIE_NAME + '=' + ConnectionParams.Instance.SessionID);
                
                if (content.Length > 0)
                {
                    request.Method = "POST";
                    request.AllowWriteStreamBuffering = true;
                    request.ContentType = "text/xml";
                    request.SendChunked = false;
                    request.ProtocolVersion = new Version("1.0");
                    byte[] data = Encoding.UTF8.GetBytes(content);
                    request.ContentLength = data.Length;
                    try
                    {
                        Stream ds = request.GetRequestStream();
                        ds.Write(data, 0, data.Length);
                        ds.Flush();
                        ds.Close();
                    }
                    catch (WebException e)
                    {
                        return null;
                    }
                }
                else
                {
                    request.Method = "GET";
                    request.ProtocolVersion = new Version("1.0");
                }

                //srround with try/catch
                try
                {
                    HttpWebResponse reply = (HttpWebResponse)request.GetResponse();
                    Cookie cookie = new Cookie(reply.Headers["Set-Cookie"]);
                    if (cookie != null)
                    {
                        ConnectionParams.Instance.SessionID = cookie.Value;
                    }
                    else
                    {
                        ConnectionParams.Instance.SessionID = string.Empty;
                        ConnectionParams.Instance.Status = LoginStatus.Disconncted;
                    }
                    
                    sr = new StreamReader(reply.GetResponseStream());
                }
                catch (WebException ex)
                {
                    switch (ex.Status)
                    {
                        case WebExceptionStatus.ConnectFailure:
                            ConnectionParams.Instance.Status = LoginStatus.Disconncted;
                            ConnectionParams.Instance.SessionID = string.Empty;
                            return null;
                    }
                    
                    //403 Forbidden error
                    HttpWebResponse resp = (HttpWebResponse)ex.Response;
                    switch (resp.StatusCode)
                    {
                        case HttpStatusCode.Forbidden:
                            ConnectionParams.Instance.Status = LoginStatus.Disconncted;
                            ConnectionParams.Instance.SessionID = string.Empty;
                            break;
                    }
                }
            }
            return sr;
        }
    }

    
}
