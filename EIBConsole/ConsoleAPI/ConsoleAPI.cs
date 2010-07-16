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
    public static class ConsoleAPI
    {
        public enum BlockingMode
        {
            NON_BLOCKING = 1,
            WAIT_FOR_ACK = 2,
            WAIT_FOR_CONFIRM = 3
        }
        
        public static object _lock_obj = new object();
        
        public static EIBUsersConf GetUsersList()
        {
            try
            {
                EIBUsersConf users = null;
                StreamReader sr = null;
                sr = GetEIBResponse(ConsoleDefinitions.EIBServerUsersConf.EIB_SERVER_GET_USERS_CONF_CMD, string.Empty,false);
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
            string content = ASCIIEncoding.ASCII.GetString(ms.ToArray());
            StreamReader sr = GetEIBResponse(ConsoleDefinitions.EIBServerUsersConf.EIB_SERVER_SET_USERS_CONF_CMD, content,false);
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
                sr = GetEIBResponse(ConsoleDefinitions.EIBInterfaceConf.EIB_SERVER_GET_INTERFACE_CONF_CMD, string.Empty,false);
                XmlSerializer ser = new XmlSerializer(typeof(EIBInterfaceConf));
                conf_obj = (EIBInterfaceConf)ser.Deserialize(sr);
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
            sr = GetEIBResponse(ConsoleDefinitions.GlobalConf.EIB_CLIENT_DISCONNECT, string.Empty,false);
            if (sr != null)
            {
                res = sr.ReadToEnd();
            }
            return res;
        }

        public static EIBInterfaceConf StartEIBInterface()
        {
            try
            {
                EIBInterfaceConf conf_obj = null;
                StreamReader sr = null;
                sr = GetEIBResponse(ConsoleDefinitions.EIBInterfaceConf.EIB_SERVER_INTERFACE_START_CMD, string.Empty,false);
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
                sr = GetEIBResponse(ConsoleDefinitions.EIBInterfaceConf.EIB_SERVER_INTERFACE_STOP_CMD, string.Empty,false);
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
            sr = GetEIBResponse(ConsoleDefinitions.GlobalConf.EIB_CONSOLE_KEEP_ALIVE_CMD, string.Empty,false);
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
                sr = GetEIBResponse(ConsoleDefinitions.EIBBusMonConf.EIB_BUS_MON_GET_ADDRESSES_CMD, string.Empty,false);
                XmlSerializer ser = new XmlSerializer(typeof(EIBBusMonAddressListConf));
                conf_obj = (EIBBusMonAddressListConf)ser.Deserialize(sr);
                return conf_obj;
            }
            catch (Exception)
            {
                return null;
            }
        }

        public static bool SendAddrCmd(EIBBusMonAddressConf node, BlockingMode mode)
        {
            string content = String.Format("{0}={1}&{2}={3}&{4}={5}",
                                            ConsoleDefinitions.EIBBusMonConf.EIB_BUS_MON_SEND_CMD_ADDR_PARAM,
                                            node.Address,
                                            ConsoleDefinitions.EIBBusMonConf.EIB_BUS_MON_SEND_CMD_VAL_PARAM,
                                            node.Value,
                                            ConsoleDefinitions.EIBBusMonConf.EIB_BUS_MON_SEND_MODE_CMD_PARAM,
                                            (int)mode);
            StreamReader sr = GetEIBResponse(ConsoleDefinitions.EIBBusMonConf.EIB_BUS_MON_SEND_CMD_TO_ADDR_CMD, content,true);
            if (sr != null && sr.ReadToEnd().Length == 0)
            {
                return true;
            }
            return false;
        }

        private static StreamReader GetEIBResponse(string url, string content, bool content_type_params)
        {
            StreamReader sr = null;
            lock (ConsoleAPI._lock_obj)
            {
                if (ConnectionParams.Instance.Status != LoginStatus.Connected)
                {
                    return null;
                }

                HttpWebRequest request = (HttpWebRequest)WebRequest.Create(ConnectionParams.Instance.HttpAddress + url);
                //set session id if exists
                request.CookieContainer = new CookieContainer();
                Cookie c = new Cookie(ConsoleDefinitions.GlobalConf.EIB_SESSION_ID_COOKIE_NAME, ConnectionParams.Instance.SessionID);
                c.Domain = ConnectionParams.Instance.Domain;
                request.CookieContainer.Add(c);

                if (content.Length > 0)
                {
                    request.Method = "POST";
                    request.ProtocolVersion = new Version("1.0");

                    if (content_type_params)
                    {
                        request.ContentType = "application/x-www-form-urlencoded";
                    }

                    byte[] data = Encoding.UTF8.GetBytes(content);
                    request.ContentLength = data.Length;
                    Stream ds = request.GetRequestStream();
                    ds.Write(data, 0, data.Length);
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
                    if (reply.Cookies[ConsoleDefinitions.GlobalConf.EIB_SESSION_ID_COOKIE_NAME] != null)
                    {
                        ConnectionParams.Instance.SessionID = reply.Cookies[ConsoleDefinitions.GlobalConf.EIB_SESSION_ID_COOKIE_NAME].Value;
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
                        case WebExceptionStatus.Timeout:
                        case WebExceptionStatus.ConnectFailure:
                        case WebExceptionStatus.ConnectionClosed:
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
