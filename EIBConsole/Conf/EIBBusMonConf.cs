using System;
using System.Collections.Generic;
using System.Text;
using System.Xml.Serialization;
using EIBConsole.Controls;

namespace EIBConsole.Conf
{
    [XmlRoot("Root")]
    public class EIBBusMonAddressListConf
    {
        [XmlArray(ConsoleDefinitions.EIBBusMonConf.EIB_BUS_MON_ADDRESSES_LIST_XML)]
        [XmlArrayItem(ConsoleDefinitions.EIBBusMonConf.EIB_BUS_MON_ADDRESS_XML)]
        public List<EIBBusMonAddressConf> List { get; set; }
    }

    [XmlRoot(ConsoleDefinitions.EIBBusMonConf.EIB_BUS_MON_ADDRESS_STR_XML)]
    public class EIBBusMonAddressConf
    {
        [XmlElement(ConsoleDefinitions.EIBBusMonConf.EIB_BUS_MON_ADDRESS_STR_XML)]
        public string Address { get; set; }
        [XmlElement(ConsoleDefinitions.EIBBusMonConf.EIB_BUS_MON_IS_ADDRESS_LOGICAL_XML)]
        public bool IsGroup { get; set; }
        [XmlElement(ConsoleDefinitions.EIBBusMonConf.EIB_BUS_MON_ADDR_LAST_RECVED_TIME_XML)]
        public string Time { get; set; }
        [XmlElement(ConsoleDefinitions.EIBBusMonConf.EIB_BUS_MON_LAST_ADDR_VALUE_XML)]
        public string Value { get; set; }
        [XmlElement(ConsoleDefinitions.EIBBusMonConf.EIB_BUS_MON_ADDRESSES_COUNT_XML)]
        public int Count { get; set; }
    }
}
