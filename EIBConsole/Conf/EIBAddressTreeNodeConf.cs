using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace EIBConsole.Conf
{
    public class EIBAddressTreeNodeConf : TreeNode
    {
        private EIBBusMonAddressConf _details;

        public EIBAddressTreeNodeConf(string text, int imageIndex, int selectedImageIndex, EIBBusMonAddressConf data)
            : base(text, imageIndex, selectedImageIndex)
        {
            _details = data;
        }

        public EIBBusMonAddressConf Details
        {
            get
            {
                return _details;
            }
        }
    }
}
