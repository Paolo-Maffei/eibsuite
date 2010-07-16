using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.Text;
using System.Net.Sockets;
using System.Net;
using System.IO;
using System.Xml.Serialization;

namespace EIBEmulator
{
    static class EIBMessages
    {
        //ERRORS
        public const byte E_NO_ERROR = 0x0;
        public const byte E_NO_MORE_CONNECTIONS = 0x24;
        
        /*
        * ************* eibnetip services ***
        */
        public const byte EIBNETIP_CORE = 0x02;
        public const byte EIBNETIP_DEVMGMT = 0x03;
        public const byte EIBNETIP_TUNNELING = 0x04;
        public const byte EIBNETIP_ROUTING = 0x05;
        public const byte EIBNETIP_REMLOG = 0x06;
        public const byte EIBNETIP_REMCONF = 0x07;
        public const byte EIBNETIP_OBJSRV = 0x08;

        
        public const byte EIB_NET_IP_HEADER_SIZE = 0x06;
        public const byte EIB_NET_IP_VERSION = 0x10;

        public const ushort EIB_MC_SEARCH_REQUEST = 0x0201;
        public const ushort EIB_MC_SEARCH_RESPONSE = 0x0202;
        public const ushort EIB_MC_CONNECT_REQUEST = 0x0205;
        public const ushort EIB_MC_CONNECT_RESPONSE = 0x0206;
        public const ushort EIB_MC_CONNECTION_STATE_REQUEST = 0x0207;
        public const ushort EIB_MC_CONNECTION_STATE_RESPONSE = 0x0208;
        public const ushort EIB_MC_DISCONNECT_REQUEST = 0x0209;
        public const ushort EIB_MC_DISCONNECT_RESPONSE = 0x020A;
        public const ushort EIB_MC_TUNNEL_REQUEST = 0x0420;
        public const ushort EIB_MC_ROUTING_INDICATION = 0x0530;
        public const ushort EIB_MC_TUNNEL_ACK = 0x0421;

        public const byte DEFAULT_CHANNEL_ID = 0x4A;
        public const byte L_DATA_CON = 0x2E;
        public const byte L_DATA_REQ = 0x11;
        public const byte MC_LDATA_IND = 0x29;
    }

    public enum DEVICE_MODE
    {
        MODE_TUNNELING,
        MODE_ROUTING
    }

    public class EIBHeader
    {
        public byte header_version;
        public byte header_length;
        public ushort mc;
        public ushort total_length;

        public EIBHeader(ushort message_code)
        {
            header_length = EIBMessages.EIB_NET_IP_HEADER_SIZE;
            header_version = EIBMessages.EIB_NET_IP_VERSION;
            mc = message_code;
            total_length = EIBMessages.EIB_NET_IP_HEADER_SIZE;
        }

        public EIBHeader(Stream s)
        {
            header_length = (byte)s.ReadByte();
            header_version = (byte)s.ReadByte();

            mc = (ushort)(s.ReadByte() << 8);
            mc |= (byte)s.ReadByte();

            total_length = (ushort)(s.ReadByte() << 8);
            total_length |= (byte)s.ReadByte();
        }

        public byte[] ToByteArray()
        {
            byte[] result = new byte[header_length];
            result[0] = header_length;
            result[1] = header_version;
            result[2] = (byte)(mc >> 8);
            result[3] = (byte)(mc & 0xFF);
            result[4] = (byte)(total_length >> 8);
            result[5] = (byte)(total_length & 0xFF);
            return result;
        }
    }

    public class EIBAddress
    {
        public bool _is_logical;
        public ushort _low;
        public ushort _high;
        GROUP_LEVEL_FORMAT _group_type;

        public EIBAddress()
            : this("0/0/0")
        {
        }

        public EIBAddress(string address)
        {
            if (address.IndexOf('/') != -1)
            {
                _is_logical = true;
                string[] tokens = address.Split(new char[] { '/' });
                if (tokens.Length == 3)
                {
                    ushort high = ushort.Parse(tokens[0]);
                    ushort low = ushort.Parse(tokens[1]);
                    _high = (ushort)((high << 3) | (low & 0x0007));
                    _low = ushort.Parse(tokens[2]);
                    _group_type = GROUP_LEVEL_FORMAT.GROUP_3_LEVEL_FORMAT;
                }
                else if (tokens.Length == 2)
                {
                    ushort high = ushort.Parse(tokens[0]);
                    ushort low = ushort.Parse(tokens[1]);
                    _high = (ushort)(high << 3 | low >> 8);
                    _low = (ushort)(low & 0x00FF);
                    _group_type = GROUP_LEVEL_FORMAT.GROUP_2_LEVEL_FORMAT;
                }
                else
                {
                    throw new Exception("EIB Address error. Wrong group address syntax");
                }
            }
            else if (address.IndexOf('.') != -1)
            {
                _is_logical = false;
                string[] tokens = address.Split(new char[] { '.' });
                if (tokens.Length == 3)
                {
                    ushort high = ushort.Parse(tokens[0]);
                    ushort low = ushort.Parse(tokens[1]);
                    _high = (ushort)((high << 4) | (low & 0x000F));
                    _low = ushort.Parse(tokens[2]);
                    _group_type = GROUP_LEVEL_FORMAT.GROUP_3_LEVEL_FORMAT;
                }
                else
                {
                    throw new Exception("EIB Address error. Wrong group address syntax");
                }
            }
            else
            {
                throw new Exception("EIB Address error. Need . or / separator");
            }
        }

        public EIBAddress(ushort address, bool is_logical)
        {
            _is_logical = is_logical;
            _high = (ushort)(address & 0xFF00);
            _low = (ushort)(address & 0x00FF);
            _group_type = GROUP_LEVEL_FORMAT.GROUP_3_LEVEL_FORMAT;
        }

        public EIBAddress(byte[] address, bool is_logical)
        {
            _is_logical = is_logical;
            _high = address[0];
            _low = address[1];
            _group_type = GROUP_LEVEL_FORMAT.GROUP_3_LEVEL_FORMAT;
        }

        public byte[] ToByteArray()
        {
            byte[] res = new byte[2];
            res[0] = (byte)_high;
            res[1] = (byte)_low;
            return res;
        }

        public override string ToString()
        {
            string address;
            if (_is_logical)
            {
                if (_group_type == GROUP_LEVEL_FORMAT.GROUP_2_LEVEL_FORMAT)
                {
                    address = MainGroup.ToString();
                    address += "/";
                    address += SubGroup11.ToString();
                }
                else
                {
                    address = MainGroup.ToString();
                    address += "/";
                    address += MiddleGroup.ToString();
                    address += "/";
                    address += SubGroup8.ToString();
                }
                return address;
            }
            address = Zone.ToString();
            address += ".";
            address += Line.ToString();
            address += ".";
            address += Device.ToString();
            return address;
        }

        private byte Device
        {
            get
            {
                if (_is_logical)
                {
                    throw new Exception("EIBAddress: Operation not appropriate for a group address");
                }
                return (byte)(_low & 0x00FF);
            }
        }

        private byte Line
        {
            get
            {
                if (_is_logical)
                {
		            throw new Exception("EIBAddress: Operation not appropriate for a group address");
	            }
                return (byte) (_high & 0x000F);
            }
        }

        private byte Zone
        {
            get
            {
                if (_is_logical)
                {
                    throw new Exception("EIBAddress: Operation not appropriate for a group address");
                }
                return (byte)((_high & 0x00F0) >> 4);
            }
        }

        private byte SubGroup8
        {
            get
            {
                if (!_is_logical)
                {
                    throw new Exception("EIBAddress: Operation not appropriate for a group address");
	            }
                return (byte) (_low & 0x00FF);
            }
        }

        private byte MiddleGroup
        {
            get
            {
                if (!_is_logical)
                {
                    throw new Exception("EIBAddress: Operation not appropriate for a group address");
	            }       
                return (byte) (_high & 0x0007);
            }
        }

        private ushort SubGroup11
        {
            get
            {
                if (!_is_logical)
                {
                    throw new Exception("EIBAddress: Operation not appropriate for a group address");
                }
                return (ushort)((_high & 0x0007) << 8 | _low);
            }
        }

        private byte MainGroup
        {
            get
            {
                if (!_is_logical)
                {
                    throw new Exception("EIBAddress: Operation not appropriate for a group address");
	            }
                return (byte) (_high >> 3);
            }
        }

        public static EIBAddress GetRandomAddress()
        {
            Random r = new Random();
            byte[] arr = new byte[2];
            r.NextBytes(arr);
            EIBAddress addr = new EIBAddress(arr,true);
            return addr;
        }
    }

    public class ConnectRequest : EIBHeader
    {
        public HPAI control_endpoint;
        public HPAI data_endpoint;
        public CRI cri;

        public ConnectRequest(Stream s) : base(s)
        {
            control_endpoint = new HPAI(s);
            data_endpoint = new HPAI(s);
            cri = new CRI(s);
        }
    }

    public class RoutingIndication : EIBHeader
    {
        public CemiFrame _frame;

        public RoutingIndication(Stream s)
            : base(s)
        {
            _frame = new CemiFrame(s);
        }

        public RoutingIndication(CemiFrame frame)
            : base(EIBMessages.EIB_MC_ROUTING_INDICATION)
        {
            _frame = frame;
            total_length += (ushort)_frame.Length;
        }

        public new byte[] ToByteArray()
        {
            MemoryStream ms = new MemoryStream();
            ms.Write(base.ToByteArray(), 0, base.ToByteArray().Length);
            ms.Write(_frame.ToByteArray(), 0, _frame.ToByteArray().Length);
            return ms.ToArray();
        }
    }

    public class ConnectRepsonse : EIBHeader
    {
        public byte channel_id;
        public byte status;
        public HPAI data_endpoint;
        public CRI cri;

        public ConnectRepsonse(EndPoint data_endpoint)
            : base(EIBMessages.EIB_MC_CONNECT_RESPONSE)
        {
            this.data_endpoint = new HPAI(data_endpoint);
            this.cri = new CRI();
            status = EIBMessages.E_NO_ERROR;
            total_length += (ushort)(2 + HPAI.length + cri.length);
        }

        public new byte[] ToByteArray()
        {
            MemoryStream ms = new MemoryStream();
            //header
            ms.Write(base.ToByteArray(), 0, base.ToByteArray().Length);
            //channel id
            ms.WriteByte(EIBMessages.DEFAULT_CHANNEL_ID);
            //status
            ms.WriteByte(status);
            //data endpoint
            ms.Write(data_endpoint.ToByteArray(), 0, data_endpoint.ToByteArray().Length);
            //cri
            ms.Write(cri.ToByteArray(), 0, cri.ToByteArray().Length);

            return ms.ToArray();
        }
    }

    public class ConnectionStateRequest : EIBHeader
    {
        public byte channel_id;
        public byte reserved;
        public HPAI control_endpoint;

        public ConnectionStateRequest(Stream s)
            : base(s)
        {
            if (s.ReadByte() != EIBMessages.DEFAULT_CHANNEL_ID || s.ReadByte() != 0)
            {
                throw new NotImplementedException();
            }
            control_endpoint = new HPAI(s);
        }
    }

    public class ConnectionStateResponse : EIBHeader
    {
        public byte channel_id;
        public byte status;

        public ConnectionStateResponse()
            : base(EIBMessages.EIB_MC_CONNECTION_STATE_RESPONSE)
        {
            channel_id = EIBMessages.DEFAULT_CHANNEL_ID;
            status = 0;
        }

        public new byte[] ToByteArray()
        {
            MemoryStream s = new MemoryStream();
            s.Write(base.ToByteArray(), 0, base.ToByteArray().Length);
            s.WriteByte(channel_id);
            s.WriteByte(status);

            return s.ToArray();
        }
    }

    public class DisconnectRequest : EIBHeader
    {
        public byte channel_id;
        public byte resereved;
        public HPAI control_endpoint;

        public DisconnectRequest(HPAI ctrlEP) : base(EIBMessages.EIB_MC_DISCONNECT_REQUEST)
        {
            channel_id = EIBMessages.DEFAULT_CHANNEL_ID;
            control_endpoint = ctrlEP;
        }
        
        public DisconnectRequest(Stream s) : base(s)
        {
            channel_id = (byte)s.ReadByte();
            resereved = (byte)s.ReadByte();

            if (channel_id != EIBMessages.DEFAULT_CHANNEL_ID || resereved != 0)
            {
                throw new NotImplementedException();
            }

            control_endpoint = new HPAI(s);
        }

        public new byte[] ToByteArray()
        {
            MemoryStream ms = new MemoryStream();
            ms.Write(base.ToByteArray(), 0, base.ToByteArray().Length);
            ms.WriteByte(channel_id);
            //reserved
            ms.WriteByte(0);
            ms.Write(control_endpoint.ToByteArray(), 0, control_endpoint.ToByteArray().Length);

            return ms.ToArray();
        }
    }

    public class DisconnectResponse : EIBHeader
    {
        public byte channel_id;
        public byte status;

        public DisconnectResponse() : base(EIBMessages.EIB_MC_DISCONNECT_RESPONSE)
        {
            channel_id = EIBMessages.DEFAULT_CHANNEL_ID;
            status = 0;
        }

        public new byte[] ToByteArray()
        {
            MemoryStream ms = new MemoryStream();
            ms.Write(base.ToByteArray(), 0, base.ToByteArray().Length);
            ms.WriteByte(channel_id);
            ms.WriteByte(status);

            return ms.ToArray();
        }
    }

    public class TunnelAck : EIBHeader
    {
        public CommonConnectionHeader _cch;

        public TunnelAck(byte sequence) : base(EIBMessages.EIB_MC_TUNNEL_ACK)
        {
            _cch = new CommonConnectionHeader(sequence);
        }

        public TunnelAck(Stream s) : base(s)
        {
            _cch = new CommonConnectionHeader(s);
        }

        public new byte[] ToByteArray()
        {
            MemoryStream ms = new MemoryStream();
            ms.Write(base.ToByteArray(),0,base.ToByteArray().Length);
            ms.Write(_cch.ToByteArray(), 0, _cch.ToByteArray().Length);
            return ms.ToArray();
        }
    }

    public class SearchRequest : EIBHeader
    {
        public HPAI _discovery_endpoint;
        public SearchRequest(Stream s)
            : base(s)
        {
            _discovery_endpoint = new HPAI(s);
        }

        public SearchRequest(EndPoint discovery)
            : base(EIBMessages.EIB_MC_SEARCH_REQUEST)
        {
            _discovery_endpoint = new HPAI(discovery);
        }

        public new byte[] ToByteArray()
        {
            MemoryStream ms = new MemoryStream();
            ms.Write(base.ToByteArray(), 0, base.ToByteArray().Length);
            ms.Write(_discovery_endpoint.ToByteArray(), 0, _discovery_endpoint.ToByteArray().Length);

            return ms.ToArray();
        }
    }

    public class SearchResponse : EIBHeader
    {
        public HPAI _control_endpoint;

        public SearchResponse(EndPoint control)
            : base(EIBMessages.EIB_MC_SEARCH_RESPONSE)
        {
            _control_endpoint = new HPAI(control);
        }

        public new byte[] ToByteArray()
        {
            MemoryStream ms = new MemoryStream();
            ms.Write(base.ToByteArray(), 0, base.ToByteArray().Length);
            ms.Write(_control_endpoint.ToByteArray(),0,_control_endpoint.ToByteArray().Length);
            return ms.ToArray();
        }
    }

    public class TunnelRequest : EIBHeader
    {
        public CommonConnectionHeader _cch;
        public CemiFrame _frame;

        public TunnelRequest(CemiFrame frame, byte sequence) : base(EIBMessages.EIB_MC_TUNNEL_REQUEST)
        {
            _frame = frame;
            _cch = new CommonConnectionHeader(sequence);
            total_length += (ushort)(_cch.length + _frame.Length);
        }

        public TunnelRequest(Stream s) : base(s)
        {
            _cch = new CommonConnectionHeader(s);
            _frame = new CemiFrame(s);
        }

        public new byte[] ToByteArray()
        {
            MemoryStream ms = new MemoryStream();

            ms.Write(base.ToByteArray(),0,base.ToByteArray().Length);
            ms.Write(_cch.ToByteArray(), 0, _cch.ToByteArray().Length);
            ms.Write(_frame.ToByteArray(), 0, _frame.ToByteArray().Length);

            return ms.ToArray();
        }
    }

    public class HPAI
    {
        public const byte length = 8;
        public const byte protocol_code = 1;
        public EndPoint endpoint;

        public HPAI(Stream s)
        {
            if (s.ReadByte() != length || s.ReadByte() != protocol_code)
            {
                throw new NotImplementedException();
            }

            string ip = string.Empty;
            ip += (byte)s.ReadByte();
            ip += '.';
            ip += (byte)s.ReadByte();
            ip += '.';
            ip += (byte)s.ReadByte();
            ip += '.';
            ip += (byte)s.ReadByte();

            ushort port = (ushort)(s.ReadByte() << 8);
            port |= (byte)s.ReadByte();

            endpoint = new IPEndPoint(IPAddress.Parse(ip), port);
        }

        public HPAI(EndPoint endpoint)
        {
            this.endpoint = endpoint;
        }

        public byte[] ToByteArray()
        {
            if (endpoint == null)
            {
                throw new Exception("Endpoint not set...");
            }
            
            byte[] result = new byte[length];
            result[0] = length;
            result[1] = protocol_code;
            
            //ip
            byte[] ip = ((IPEndPoint)endpoint).Address.GetAddressBytes();
            ip.CopyTo(result,2);
            //port
            ushort port = (ushort)((IPEndPoint)endpoint).Port;
            result[6] = (byte)(port >> 8);
            result[7] = (byte)(port & 0xFF);
            return result;
        }
    }

    public class CRI
    {
        public byte length;
        public byte conn_type_code;
        public byte host_prot_data;
        public byte depend_data;

        public CRI(Stream s)
        {
            length = (byte)s.ReadByte();
            conn_type_code = (byte)s.ReadByte();
            host_prot_data = (byte)s.ReadByte();
            depend_data = (byte)s.ReadByte();

            //Validation
            if (length != 0x04 || conn_type_code != 0x04 ||
                host_prot_data != 0x02 || depend_data != 0)
            {
                throw new NotImplementedException();
            }

        }

        public CRI()
        {
            length = 0x04;
            conn_type_code = 0x04;
            host_prot_data = 0x02;
            depend_data = 0x0;
        }

        public byte[] ToByteArray()
        {
            byte[] result = new byte[length];
            result[0] = length;
            result[1] = conn_type_code;
            result[2] = host_prot_data;
            result[3] = depend_data;
            return result;
        }
    }

    public enum GROUP_LEVEL_FORMAT
    {
        GROUP_2_LEVEL_FORMAT,
        GROUP_3_LEVEL_FORMAT
    }

    public class CemiFrame
    {
        public byte _mc;
        public byte _addil;
        public byte _ctrl1;
        public byte _ctrl2;
        public EIBAddress _src;
        public EIBAddress _dst;
        public byte _apci_length;
        public byte _tpci;
        public byte _apci;
        public byte[] _addil_data;

        public CemiFrame(EIBAddress src, EIBAddress dest, byte[] value, bool need_ack)
        {
            if (value == null)
            {
                throw new Exception("Value byte Array is null");
            }
            
            _mc = 0x29;
            _addil = 0;
            _ctrl1 = need_ack ? (byte)0x02 : (byte)0;
            _ctrl1 |= 0x80; //standard frame
            _ctrl2 = 0x60; // default: routing counter = 6
            if (dest._is_logical)
            {
                _ctrl2 |= 0x80;
            }
            _src = src;
            _dst = dest;
            _apci_length = (byte)value.Length;
            _tpci = 0;
            _apci = value[0];

            if (_apci_length > 1)
            {
                _addil_data = new byte[_apci_length - 1];
                for (int i = 1; i < _apci_length; ++i)
                {
                    _addil_data[i - 1] = value[i];
                }
            }
        }

        public CemiFrame(Stream s)
        {
            _mc = (byte)s.ReadByte();
            _addil = (byte)s.ReadByte();
            _ctrl1 = (byte)s.ReadByte();
            _ctrl2 = (byte)s.ReadByte();

            byte[] addr = new byte[2];
            s.Read(addr, 0, 2);
            _src = new EIBAddress(addr, false);
            s.Read(addr, 0, 2);
            _dst = new EIBAddress(addr, (_ctrl2 & 0x80) != 0);

            _apci_length = (byte)s.ReadByte();
            _tpci = (byte)s.ReadByte();
            _apci = (byte)s.ReadByte();

            if (_apci_length > 1)
            {
                _addil_data = new byte[_apci_length - 1];
                s.Read(_addil_data, 0, _apci_length - 1);
            }
        }

        public bool IsAckNeeded
        {
            get
            {
                return ((_ctrl1 & 0x02) == 0);
            }
            set
            {
                if (value)
                {
                    _ctrl1 |= 0x02;
                }
                else
                {
                    _ctrl1 &= (0xFD);
                }
            }
        }

        public int Length
        {
            get { return _apci_length + 10; }
        }

        public byte[] ToByteArray()
        {
            MemoryStream ms = new MemoryStream();
            ms.WriteByte(_mc);
            ms.WriteByte(_addil);
            ms.WriteByte(_ctrl1);
            ms.WriteByte(_ctrl2);
            ms.Write(_src.ToByteArray(), 0, _src.ToByteArray().Length);
            ms.Write(_dst.ToByteArray(), 0, _dst.ToByteArray().Length);
            ms.WriteByte(_apci_length);
            ms.WriteByte(_tpci);
            ms.WriteByte(_apci);
            if (_apci_length > 1)
            {
                ms.Write(_addil_data, 0, _addil_data.Length);
            }
            return ms.ToArray();
        }
    }

    public class CommonConnectionHeader
    {
        public byte length;
        public byte channel_id;
        public byte sequence;
        public byte service_type;

        public CommonConnectionHeader(byte _sequence)
        {
            length = 4;
            channel_id = EIBMessages.DEFAULT_CHANNEL_ID;
            sequence = _sequence;
            service_type = 0;
        }

        public CommonConnectionHeader(Stream s)
        {
            length = (byte)s.ReadByte();
            Debug.Assert(length == 4, "Common Connection header length must be 4");
            channel_id = (byte)s.ReadByte();
            sequence = (byte)s.ReadByte();
            service_type = (byte)s.ReadByte();
        }

        public byte[] ToByteArray()
        {
            byte[] res = new byte[4];
            res[0] = length;
            res[1] = channel_id;
            res[2] = sequence;
            res[3] = service_type;
            return res;
        }
    }
}
