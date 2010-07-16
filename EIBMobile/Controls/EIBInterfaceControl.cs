using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using EIBMobile.Conf;

namespace EIBMobile.Controls
{
    public partial class EIBInterfaceControl : UserControl, IRefreshable
    {
        private const int DEVICE_STATUS_UNKNOWN_IMG_INDEX = 2;
        private const int DEVICE_STATUS_STOPPED_IMG_INDEX = 1;
        private const int DEVICE_STATUS_RUNNING_IMG_INDEX = 0;
        
        public EIBInterfaceControl()
        {
            InitializeComponent();
            UpdateView(null);
            
        }

        public void UpdateView(EIBInterfaceConf conf)
        {
            if (conf == null)
            {
                //management buttons
                this.btnStart.Enabled = false;
                this.btnStop.Enabled = false;
                
                //device address
                this.ipDeviceAddress.Text = "0.0.0.0";
                this.ipDeviceAddress.Enabled = false;

                //device status
                this.lblInterfaceStatus.Text = "Unknown Status";
                
                //auto detec check box
                this.cbAutoDetect.Checked = false;
                this.cbAutoDetect.Enabled = false;
                
                //device mode radio buttons
                this.rbModeRouting.Checked = false;
                this.rbModeRouting.Enabled = false;
                this.rbModeTunneling.Checked = false;
                this.rbModeTunneling.Enabled = false;

                //stats
                this.lblTotalSentVal.Text = "0";
                this.lblTotalRecvdVal.Text = "0";
                
                //status image
                this.pbDeviceStatus.Image = this.ControlimageList.Images[DEVICE_STATUS_UNKNOWN_IMG_INDEX];

                return;
            }

            this.ipDeviceAddress.Enabled = true;
            this.ipDeviceAddress.Text = conf.DeviceAddress;

            this.rbModeTunneling.Enabled = true;
            this.rbModeRouting.Enabled = true;
            this.cbAutoDetect.Enabled = true;

            if (conf.DeviceMode == DeviceMode.MODE_ROUTING)
            {
                this.rbModeRouting.Checked = true;
            }
            else if (conf.DeviceMode == DeviceMode.MODE_TUNNELING)
            {
                this.rbModeTunneling.Checked = true;
            }
            this.cbAutoDetect.Checked = conf.AutoDetect;
            this.lblTotalRecvdVal.Text = conf.TotalPacketsReceived.ToString();
            this.lblTotalSentVal.Text = conf.TotalPacketsSent.ToString();
            if (conf.Running)
            {
                this.pbDeviceStatus.Image = this.ControlimageList.Images[DEVICE_STATUS_RUNNING_IMG_INDEX];
                this.lblInterfaceStatus.Text = "Running";
                this.btnStart.Enabled = false;
                this.btnStop.Enabled = true;
            }
            else
            {
                this.pbDeviceStatus.Image = this.ControlimageList.Images[DEVICE_STATUS_STOPPED_IMG_INDEX];
                this.lblInterfaceStatus.Text = "Stopped";
                this.btnStart.Enabled = true;
                this.btnStop.Enabled = false;
            }
        }


        #region IRefreshable Members

        void IRefreshable.Refresh()
        {
            Cursor.Current = Cursors.WaitCursor;
            EIBInterfaceConf conf = ConsoleAPI.GetEIBInterfaceConf();
            UpdateView(conf);
            Cursor.Current = Cursors.Default;
        }

        void IRefreshable.OnDisconnect()
        {
            UpdateView(null);
        }

        void IRefreshable.OnConnect()
        {
            throw new NotImplementedException();
        }

        #endregion

        private void btnStart_Click(object sender, EventArgs e)
        {
            Cursor.Current = Cursors.WaitCursor;
            EIBInterfaceConf conf = ConsoleAPI.StartEIBInterface();
            UpdateView(conf);
            Cursor.Current = Cursors.Default;
        }

        private void btnStop_Click(object sender, EventArgs e)
        {
            Cursor.Current = Cursors.WaitCursor;
            EIBInterfaceConf conf = ConsoleAPI.StopEIBInterface();
            UpdateView(conf);
            Cursor.Current = Cursors.Default;
        }
    }
}
