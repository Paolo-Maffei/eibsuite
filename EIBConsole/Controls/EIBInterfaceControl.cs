using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Xml.Serialization;
using EIBConsole.Conf;

namespace EIBConsole.Controls
{
    
    
    public partial class EIBInterfaceControl : UserControl, IRefreshable
    {
        private EIBInterfaceConf _conf;
        
        public EIBInterfaceControl()
        {
            InitializeComponent();
            this.btnStartInterface.Enabled = false;
            this.btnStopInterface.Enabled = false;
        }

        private void rbModeRouting_CheckedChanged(object sender, EventArgs e)
        {
            if (this.rbModeRouting.Checked)
            {
                this.tbDeviceAddress.Enabled = false;
                this.cbAutoDetect.Enabled = false;
            }

            if (this.rbModeTunneling.Checked)
            {
                this.tbDeviceAddress.Enabled = true;
                this.cbAutoDetect.Enabled = true;
            }
        }

        public void UpdateView()
        {
            if (_conf != null)
            {
                this.rbModeRouting.Checked = _conf.DeviceMode == DeviceMode.MODE_ROUTING;
                this.rbModeTunneling.Checked = _conf.DeviceMode == DeviceMode.MODE_TUNNELING;
                this.cbAutoDetect.Checked = _conf.AutoDetect;
                this.tbDeviceAddress.Text = _conf.DeviceAddress;
                this.EibPortNUD.Value = _conf.DevicePort;
                this.lblLasttimeSent.Text = _conf.LastPacketSentTime;
                this.lblLasttimeReceived.Text = _conf.LastPacketReceivedTime;

                this.lblTotalReceived.Text = _conf.TotalPacketsReceived.ToString();
                this.lblTotalSent.Text = _conf.TotalPacketsSent.ToString();

                this.pbStatus.Image = this.imageList1.Images[_conf.Running ? "device_running" : "device_stopped"];
                this.label1.Text = _conf.Running ? "Running" : "Not Running";

                this.btnStopInterface.Enabled = _conf.Running;
                this.btnStartInterface.Enabled = !_conf.Running;
            }
            else
            {
                OnDisconnect();
            }
        }

        private void EIBInterfaceControl_Load(object sender, EventArgs e)
        {
            this.Dock = DockStyle.Fill;
            this.pbStatus.Image = this.imageList1.Images["device_unknown"];
            this.label1.Text = "Unknown";
        }

        #region IRefreshable Members

        void IRefreshable.Refresh()
        {
            _conf = ConsoleAPI.GetEIBInterfaceConf();
            UpdateView();
        }

        public void OnDisconnect()
        {
            this.rbModeRouting.Checked = false;
            this.rbModeTunneling.Checked = false;
            this.cbAutoDetect.Checked = false;
            this.tbDeviceAddress.Text = "0.0.0.0";
            this.EibPortNUD.Value = 0;
            this.lblLasttimeSent.Text = "Never";
            this.lblLasttimeReceived.Text = "Never";

            this.lblTotalReceived.Text = "0";
            this.lblTotalSent.Text = "0";

            this.pbStatus.Image = this.imageList1.Images["device_unknown"];
            this.label1.Text = "Unknown";

            this.btnStopInterface.Enabled = false;
            this.btnStartInterface.Enabled = false;
        }

        public void OnConnect()
        {
            throw new NotImplementedException();
        }

        #endregion

        private void btnStartInterface_Click(object sender, EventArgs e)
        {
            Cursor.Current = Cursors.WaitCursor;

            _conf = ConsoleAPI.StartEIBInterface();
            UpdateView();

            Cursor.Current = Cursors.Default;
        }

        private void btnStopInterface_Click(object sender, EventArgs e)
        {
            Cursor.Current = Cursors.WaitCursor;
            _conf = ConsoleAPI.StopEIBInterface();
            UpdateView();
            Cursor.Current = Cursors.Default;
        }

    }
}
