using System;
using System.Collections.Generic;
using System.Text;

namespace EIBMobile.Conf
{
    public interface IRefreshable
    {
        void Refresh();
        void OnDisconnect();
        void OnConnect();
    }
}
