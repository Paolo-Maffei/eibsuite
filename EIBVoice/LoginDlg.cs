using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using IPAddressControlLib;
using EIBNetWrapper;
using EIBVoice.Properties;
using System.Net.NetworkInformation;
using System.Net.Sockets;
using System.Net;


namespace EIBVoice
{
    public partial class LoginDlg : Form
    {
        Logger _looger;

        public LoginDlg(Logger logger)
        {
            InitializeComponent();
            this.FormBorderStyle = FormBorderStyle.FixedDialog;
            this._looger = logger;
        }

        public bool ValidateInput()
        {
            return true;
        }

        public string ServerAddress
        {
            get { return tbServerAddress.Text; }
        }

        public int ServerPort
        {
            get
            {
                int result;
                if (Int32.TryParse(tbServerPort.Text,out result) && result <= UInt16.MaxValue && result > 0)
                {
                    return result;
                }
                throw new Exception("Port must be a positive number.");
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

        public bool Login(CGenericServerWrapper server)
        {
            if (!Validate())
            {
                return false;
            }
            string network_name = Settings.Default.NetworkName;
            string initial_key = Settings.Default.InitialKey;
            
            string local_ip = GeneralSettings.LocalAddress.ToString();
            _looger.Log(String.Format("Connecting through Network interface with address: {0}", local_ip));
            try
            {
                bool res = server.OpenConnection(network_name,
                                                 ServerAddress,
                                                 ServerPort,
                                                 initial_key,
                                                 local_ip,
                                                 UserName,
                                                 Password);

                if (!res)
                {
                    server.Close();
                }
                return res;
            }
            catch (Exception e)
            {
                _looger.Log(String.Format("Error: {0}", e.Message));
                MessageBox.Show(e.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }
        }
    }
}
