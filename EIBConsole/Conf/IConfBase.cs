using System;
using System.Collections.Generic;
using System.Text;
using System.Xml.Serialization;

namespace EIBConsole.Conf
{
    [XmlInclude(typeof(EIBUsersConf))]
    [XmlRoot("Root")]
    public interface IConfBase
    {
        
    }
}
