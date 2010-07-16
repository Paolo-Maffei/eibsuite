using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using EIBMobile.Connection;
using EIBMobile.Conf;

namespace EIBMobile
{
    public partial class MainForm : Form
    {
        LoginDlg _login_dlg = null;

        public MainForm()
        {
            InitializeComponent();
            _login_dlg = new LoginDlg();

            this.tpInterface.Tag = this.eibInterfaceControl1;
            this.tpUsers.Tag = this.eibUsersControl1;
        }

        private void miConnect_Click(object sender, EventArgs e)
        {
            if (_login_dlg == null)
            {
                return;
            }

            while (_login_dlg.ShowDialog() == DialogResult.OK)
            {
                if (_login_dlg.Login())
                {
                    this.statusBar.Text = "Connected";
                    IRefreshable control = this.tabControl.TabPages[this.tabControl.SelectedIndex].Tag as IRefreshable;
                    if (control != null)
                    {
                        control.Refresh();
                    }
                    break;
                }
                this.statusBar.Text = "Disconnected";
                switch (ConnectionParams.Instance.Status)
                {
                    case LoginStatus.ConnectionDenied:
                        MessageBox.Show("Connection Denied. Change your credentials.");
                        break;
                    case LoginStatus.ServerDown:
                        MessageBox.Show("EIB Server is not responding.");
                        break;
                }
                
            }
        }

        private void menuItem1_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void miRefresh_Click(object sender, EventArgs e)
        {
            IRefreshable control = this.tabControl.TabPages[this.tabControl.SelectedIndex].Tag as IRefreshable;
            if (control != null)
            {
                control.Refresh();
            }
        }
    }
}