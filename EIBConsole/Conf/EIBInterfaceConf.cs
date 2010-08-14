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

    [Flags]
    public enum SupportedServices
    {
        SERVICE_CORE = 0x01,
        SERVICE_DEV_MNGMT = 0x02,
        SERVICE_TUNNELING = 0x04,
        SERVICE_ROUTING = 0x08,
        SERVICE_REMLOG = 0x10,
        SERVICE_REMCONF = 0x20,
        SERVICE_OBJSRV = 0x40
    };

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
        [XmlElement(ConsoleDefinitions.EIBInterfaceConf.EIB_INTERFACE_DEV_DESCRIPTION_XML)]
        public EIBDeviceInfo DeviceInfo { get; set; }
    }

    [XmlRoot(ConsoleDefinitions.EIBInterfaceConf.EIB_INTERFACE_DEV_DESCRIPTION_XML)]
    public class EIBDeviceInfo
    {
        [XmlElement(ConsoleDefinitions.EIBInterfaceConf.EIB_INTERFACE_DEV_NAME_XML)]
        public string Name;
        [XmlElement(ConsoleDefinitions.EIBInterfaceConf.EIB_INTERFACE_DEV_MAC_ADDRESS_XML)]
        public string MacAddress;
        [XmlElement(ConsoleDefinitions.EIBInterfaceConf.EIB_INTERFACE_DEV_SERIAL_NUMBER_XML)]
        public string SerialNumber;
        [XmlElement(ConsoleDefinitions.EIBInterfaceConf.EIB_INTERFACE_DEV_MULTICAST_ADDRESS_XML)]
        public string MulticastAddress;
        [XmlElement(ConsoleDefinitions.EIBInterfaceConf.EIB_INTERFACE_DEV_PHY_ADDRESS_XML)]
        public string PhysicalAddress;
        [XmlElement(ConsoleDefinitions.EIBInterfaceConf.EIB_INTERFACE_DEV_SUPPORTED_SERVICES_XML)]
        public int SupportedServices;
    }
}
