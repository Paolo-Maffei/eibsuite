using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using EIBConsole.Conf;

namespace EIBConsole.Controls
{
    public partial class EIBBusMonitorControl : UserControl, IRefreshable
    {
        EIBBusMonAddressListConf _conf;

        public EIBBusMonitorControl()
        {
            InitializeComponent();
        }

        private void EIBBusMonitorControl_Load(object sender, EventArgs e)
        {
            this.Dock = DockStyle.Fill;
            this.treeView1.Dock = DockStyle.Fill;
            this.treeView1.AfterSelect += new TreeViewEventHandler(treeView1_AfterSelect);
            this.btnReplayLastPkt.Click += new EventHandler(btnReplayLastPkt_Click);
        }

        void btnReplayLastPkt_Click(object sender, EventArgs e)
        {
            EIBAddressTreeNodeConf n = this.treeView1.SelectedNode as EIBAddressTreeNodeConf;
            if (n == null || n.Details == null)
            {
                //Error
                return;
            }
            Cursor.Current = Cursors.WaitCursor;
            if (!ConsoleAPI.SendAddrCmd(n.Details, ConsoleAPI.BlockingMode.WAIT_FOR_CONFIRM))
            {
                //Error
            }
            Cursor.Current = Cursors.Default;
        }

        void treeView1_AfterSelect(object sender, TreeViewEventArgs e)
        {
            if (e.Node.Parent == null)
            {
                //root node --> do nothing
                ShowEmptyAddressDetails();
                return;
            }
            EIBAddressTreeNodeConf addrNode = e.Node as EIBAddressTreeNodeConf;
            if (addrNode == null)
            {
                //YGYG: wierd! error ?
                ShowEmptyAddressDetails();
                return;
            }

            this.lblLastTimeSeenVal.Text = addrNode.Details.Time;
            this.lblLastValueVal.Text = addrNode.Details.Value;
            this.lblTotalNumSeenVal.Text = addrNode.Details.Count.ToString();
            this.btnReplayLastPkt.Enabled = true;
        }

        private void ShowEmptyAddressDetails()
        {
            this.lblLastTimeSeenVal.Text = "Never";
            this.lblLastValueVal.Text = "0x0";
            this.lblTotalNumSeenVal.Text = "0";
        }

        private void UpdateView()
        {
            this.treeView1.Nodes["Root"].Nodes.Clear();

            if (_conf != null)
            {
                foreach (EIBBusMonAddressConf addr in _conf.List)
                {
                    EIBAddressTreeNodeConf node = new EIBAddressTreeNodeConf(addr.Address, 1, 1, addr);
                    this.treeView1.Nodes["Root"].Nodes.Add(node);
                }
            }

            this.treeView1.ExpandAll();
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            if (this.treeView1.SelectedNode == null || this.treeView1.SelectedNode.Parent == null)
            {
                this.btnReplayLastPkt.Enabled = false;
            }
            else
            {
                this.btnReplayLastPkt.Enabled = true;
            }
            base.OnPaint(e);
        }

        #region IRefreshable Members

        void IRefreshable.Refresh()
        {
            _conf = ConsoleAPI.GetAddressList();
            UpdateView();
        }

        void IRefreshable.OnDisconnect()
        {
            this.treeView1.Nodes["Root"].Nodes.Clear();
            this.btnReplayLastPkt.Enabled = false;
        }

        void IRefreshable.OnConnect()
        {
            throw new NotImplementedException();
        }

        #endregion
    }
}
