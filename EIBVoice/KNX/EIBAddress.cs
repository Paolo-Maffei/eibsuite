using System;
using System.Net;
namespace EIBVoice.KNX
{
    public enum GROUP_LEVEL_FORMAT
    {
        GROUP_2_LEVEL_FORMAT,
        GROUP_3_LEVEL_FORMAT
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

        public ushort RawAddress
        {
            get
            {
                return (ushort)(_high << 8 | _low);
                //return this as if called with htons
                //return (ushort)((byte)_high | (byte)_low);
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
                return (byte)(_high & 0x000F);
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
                return (byte)(_low & 0x00FF);
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
                return (byte)(_high & 0x0007);
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
                return (byte)(_high >> 3);
            }
        }

        public static EIBAddress GetRandomAddress()
        {
            Random r = new Random();
            byte[] arr = new byte[2];
            r.NextBytes(arr);
            EIBAddress addr = new EIBAddress(arr, true);
            return addr;
        }
    }
}