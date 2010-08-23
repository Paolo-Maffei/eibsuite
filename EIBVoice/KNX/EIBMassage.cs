using System;
using System.Collections.Generic;
using System.Text;
using System.Xml.Serialization;

namespace EIBVoice.KNX
{
    [XmlRoot("EIBTelegram")]
    public class EIBTelegram
    {
        //[XmlElement("MessageControl")]
        //public byte ControlField = 0x29;
        [XmlIgnore()]
        public byte AddIL = 0x00;
        [XmlIgnore()]
        public byte Control1 = 0xbc;
        [XmlIgnore()]
        public byte Control2 = 0xd0;
        [XmlIgnore()]
        public EIBAddress SrcAddress = new EIBAddress();
        [XmlIgnore()]
        public EIBAddress DestAddress = new EIBAddress();
        [XmlIgnore()]
        public byte Length = 0x29;
        [XmlElement("TCPI")]
        public byte TCPI = 0x00;
        [XmlElement("APCI")]
        public byte APCI = 0x80;

        public byte[] Data = null;

        /*
        [XmlElement("SourceAddress")]
        public EIBAddress SourceAddress
        {
            get { return SrcAddress; }
            set { SrcAddress = value;}
        }
        */

        [XmlElement("DestinationAddress")]
        public String DestinationAddress
        {
            get { return DestAddress.ToString(); }
            set { DestAddress = new EIBAddress(value);}
        }

        
        public EIBTelegram()
        {
        }

        public byte[] ToByteArray()
        {
            byte[] arr = new byte[Data != null ? 11 + Data.Length : 11];

            arr[0] = 0x29;
            arr[1] = AddIL;
            arr[2] = Control1;
            arr[3] = Control2;
            SrcAddress.ToByteArray().CopyTo(arr, 4);
            DestAddress.ToByteArray().CopyTo(arr, 6);
            arr[8] = Data != null ? (byte)(1 + Data.Length) : (byte)1;
            arr[9] = TCPI;
            arr[10] = APCI;
            
            if (Data != null)
            {
                Data.CopyTo(arr, 10);
            }

            return arr;
        }
    }
}
