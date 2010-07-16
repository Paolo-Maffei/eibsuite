using System;
using System.Collections.Generic;
using System.Text;
using System.Xml.Serialization;
using EIBConsole.Controls;

namespace EIBConsole.Conf
{
    public enum DeviceMode
    {
        MODE_ROUTING,
        MODE_TUNNELING,
        MODE_UNKNOWN
    }
    
    [XmlRoot("Root")]
    public class EIBInterfaceConf
    {
        [XmlElement(ConsoleDefinitions.EIBInterfaceConf.EIB_INTERFACE_ADDRESS_XML)]
        public string DeviceAddress { get; set; }
        [XmlElement(ConsoleDefinitions.EIBInterfaceConf.EIB_INTERFACE_PORT_XML)]
        public int DevicePort { get; set; }
        [XmlElement(ConsoleDefinitions.EIBInterfaceConf.EIB_INTERFACE_DEVICE_MODE_XML)]
        public DeviceMode DeviceMode { get; set; }
        [XmlElement(ConsoleDefinitions.EIBInterfaceConf.EIB_INTERFACE_AUTO_DETECT_XML)]
        public bool AutoDetect { get; set; }
        [XmlElement(ConsoleDefinitions.EIBInterfaceConf.EIB_INTERFACE_RUNNING_STATUS_XML)]
        public bool Running { get; set; }
        [XmlElement(ConsoleDefinitions.EIBInterfaceConf.EIB_INTERFACE_LAST_TIME_PACKET_SENT_XML)]
        public string LastPacketSentTime { get; set; }
        [XmlElement(ConsoleDefinitions.EIBInterfaceConf.EIB_INTERFACE_LAST_TIME_PACKET_RECEIVED_XML)]
        public string LastPacketReceivedTime { get; set; }
        [XmlElement(ConsoleDefinitions.EIBInterfaceConf.EIB_INTERFACE_TOTAL_PACKETS_RECEIVED_XML)]
        public int TotalPacketsReceived { get; set; }
        [XmlElement(ConsoleDefinitions.EIBInterfaceConf.EIB_INTERFACE_TOTAL_PACKETS_SENT_XML)]
        public int TotalPacketsSent { get; set; }

    }
}
