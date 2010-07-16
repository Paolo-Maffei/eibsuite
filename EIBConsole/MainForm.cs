using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using EIBConsole.Conf;
using System.Threading;

namespace EIBConsole
{
    public partial class MainForm : Form
    {
        private LoginDlg _LoginDlg = null;
        private ManualResetEvent _wait;

        public MainForm()
        {
            InitializeComponent();
            this.btnRefresh.Enabled = false;
            this.btnDisconnect.Enabled = false;

            _LoginDlg = new LoginDlg();
            _LoginDlg.StartPosition = FormStartPosition.CenterParent;

            this.tpUsers.Tag = this.eibUsersControl1;
            this.tpEIBInterfaceConfig.Tag = this.eibInterfaceControl1;
            this.tpBusMonitor.Tag = this.eibBusMonitorControl1;
            //place the main window in the center screen (when shown)
            this.StartPosition = FormStartPosition.CenterScreen;
            //load event
            this.Shown += new EventHandler(MainForm_Shown);
            //close /exit event
            this.FormClosing += new FormClosingEventHandler(MainForm_FormClosing);
        }

        void MainForm_Shown(object sender, EventArgs e)
        {
            btnConnect_Click(null, null);
        }

        void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (ConnectionParams.Instance.Status == LoginStatus.Connected)
            {
                btnDisconnect_Click(null, null);
            }
        }

        private void UpdateStatus()
        {
            switch (ConnectionParams.Instance.Status)
            {
                case LoginStatus.Connected:
                    this.lblServerStatus.Text = "Connected";
                    this.btnRefresh.Enabled = true;
                    this.btnDisconnect.Enabled = true;
                    this.btnConnect.Enabled = false;
                    break;
                case LoginStatus.ServerDown:
                case LoginStatus.Disconncted:
                case LoginStatus.ConnectionDenied:
                    this.btnRefresh.Enabled = false;
                    this.btnDisconnect.Enabled = false;
                    this.btnConnect.Enabled = true;
                    this.lblServerStatus.Text = "Disconnected";
                    break;
            }  
        }

        private void btnRefresh_Click(object sender, EventArgs e)
        {
            Cursor.Current = Cursors.WaitCursor;
            
            //do the refresh here...
            IRefreshable control = this.tcConsole.SelectedTab.Tag as IRefreshable;
            if (control != null)
            {
                control.Refresh();
            }
            Cursor.Current = Cursors.Default;
        }

        private void btnDisconnect_Click(object sender, EventArgs e)
        {
            if (ConnectionParams.Instance.Status != LoginStatus.Connected)
            {
                return;
            }
            
            Cursor.Current = Cursors.WaitCursor;

            KeepAliveThread.CancelAsync();
            _wait.Set();
            
            _LoginDlg.LogOut();
            UpdateStatus();

            foreach (TabPage tp in this.tcConsole.TabPages)
            {
                IRefreshable control = tp.Tag as IRefreshable;
                if (control != null)
                {
                    control.OnDisconnect();
                }
            }
            Cursor.Current = Cursors.Default;
        }

        private void btnConnect_Click(object sender, EventArgs e)
        {
            while (_LoginDlg.ShowDialog() == DialogResult.OK)
            {
                Cursor.Current = Cursors.WaitCursor;
                _LoginDlg.Login();
                Cursor.Current = Cursors.Default;
                UpdateStatus();
                if (ConnectionParams.Instance.Status == LoginStatus.ConnectionDenied)
                {
                    DialogResult result = MessageBox.Show("Cannot connect to EIB Server (Incorrect User name or Password)\n Do you want to try again?",
                                                          "Connection Error", MessageBoxButtons.YesNo, MessageBoxIcon.Error, MessageBoxDefaultButton.Button1);
                    if (result == DialogResult.Yes)
                    {
                        continue;
                    }
                    if (result == DialogResult.No)
                    {
                        break;
                    }
                }
                else if (ConnectionParams.Instance.Status == LoginStatus.ServerDown)
                {
                    DialogResult result = MessageBox.Show("Cannot connect to EIB Server (no response from EIB Sever)\n Do you want to try again?",
                                                          "Connection Error", MessageBoxButtons.YesNo, MessageBoxIcon.Error, MessageBoxDefaultButton.Button1);
                    if (result == DialogResult.Yes)
                    {
                        continue;
                    }
                    if (result == DialogResult.No)
                    {
                        break;
                    }
                }
                else if (ConnectionParams.Instance.Status == LoginStatus.Connected)
                {
                    IRefreshable tab = this.tcConsole.SelectedTab.Tag as IRefreshable;
                    tab.Refresh();

                    _wait = new ManualResetEvent(false);
                                        
                    this.KeepAliveThread.RunWorkerAsync();
                    break;
                }
            }
        }

        private void KeepAliveThread_DoWork(object sender, DoWorkEventArgs e)
        {
            while (ConsoleAPI.GetKeepAliveStatus() && !KeepAliveThread.CancellationPending)
            {
                _wait.WaitOne(10000, false);
                _wait.Reset();
            }

            lock (ConsoleAPI._lock_obj)
            {
                ConnectionParams.Instance.Status = LoginStatus.Disconncted;
                ConnectionParams.Instance.SessionID = string.Empty;
            }

            e.Result = KeepAliveThread.CancellationPending;
        }

        private void KeepAliveThread_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if ((bool)e.Result)
            {
                return;
            }
            UpdateStatus();
            MessageBox.Show("EIB Server is not responding. Please Reconnect.","Connection Error",MessageBoxButtons.OK,MessageBoxIcon.Warning);
            foreach (TabPage tp in this.tcConsole.TabPages)
            {
                IRefreshable control = tp.Tag as IRefreshable;
                if (control != null)
                {
                    control.OnDisconnect();
                }
            }
        }
    }
}
