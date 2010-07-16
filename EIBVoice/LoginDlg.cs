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
        public LoginDlg()
        {
            InitializeComponent();
            this.FormBorderStyle = FormBorderStyle.FixedDialog;
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
            
            string local_ip = String.Empty;

            NetworkInterface[] interfaces = NetworkInterface.GetAllNetworkInterfaces();
            foreach (NetworkInterface n in interfaces)
            {
                foreach (UnicastIPAddressInformation info in n.GetIPProperties().UnicastAddresses)
                {
                    if (info.Address.AddressFamily == AddressFamily.InterNetwork && IPAddress.Loopback.ToString() != info.Address.ToString())
                    {
                        local_ip = info.Address.ToString();
                        break;
                    }
                }
            }

            try
            {

                return server.OpenConnection(network_name,
                                             ServerAddress,
                                             ServerPort,
                                             initial_key,
                                             local_ip,
                                             UserName,
                                             Password);
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }
        }
    }
}
