using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace EIBMobile
{
    public partial class LoginDlg : Form
    {
        public LoginDlg()
        {
            InitializeComponent();
        }

        public string ServerAddress
        {
            get { return tbServerAddress.Text; }
        }

        public int ServerPort
        {
            get
            {
                try
                {
                    int result = int.Parse(tbServerPort.Text);
                    return result;
                }
                catch (Exception)
                {
                    return -1;
                }
            }
        }

        public string UserName
        {
            get { return tbUserName.Text; }
        }

        public string Password
        {
            get { return tbPassword.Text; }
        }

        public bool Login()
        {
            try
            {
                ConsoleAPI.Connect(ServerAddress, ServerPort, UserName, Password);
            }
            catch (Exception e)
            {
                MessageBox.Show("Exception: " + e.Message);
                return false;
            }
            return Connection.ConnectionParams.Instance.Status == EIBMobile.Connection.LoginStatus.Connected;
        }
    }
}