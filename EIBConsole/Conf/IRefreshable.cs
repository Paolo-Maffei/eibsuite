using System;
using System.Collections.Generic;
using System.Text;

namespace EIBConsole.Conf
{
    public delegate void StatusChangedEventHandler(object sender, string status);

    public interface IRefreshable
    {
        void Refresh();
        void OnDisconnect();
        void OnConnect();
    }
}
