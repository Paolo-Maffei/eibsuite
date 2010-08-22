using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net.NetworkInformation;
using System.Net.Sockets;
using System.Net;
using EIBVoice.Properties;

namespace EIBVoice
{
    public partial class GeneralSettingsForm : Form
    {
        IPAddress _current;

        public GeneralSettingsForm()
        {
            InitializeComponent();
            updateInterfacesList();
        }

        private void updateInterfacesList()
        {
            this.netifsComboBox.Items.Clear();
            NetworkInterface[] interfaces = NetworkInterface.GetAllNetworkInterfaces();
            bool selected = false;

            foreach (NetworkInterface n in interfaces)
            {
                foreach (UnicastIPAddressInformation info in n.GetIPProperties().UnicastAddresses)
                {
                    if (info.Address.AddressFamily == AddressFamily.InterNetwork)
                    {
                        this.netifsComboBox.Items.Add(n.Description);
                        if (n.Description == Settings.Default.NetworkInterface)
                        {
                            _current = info.Address;
                            GeneralSettings.LocalAddress = _current;
                            this.netifsComboBox.SelectedItem = n.Description;
                            selected = true;
                        }
                        if (!selected)
                        {
                            _current = info.Address;
                            this.netifsComboBox.SelectedItem = n.Description;
                        }
                    }
                }
            }
            GeneralSettings.LocalAddress = _current;
        }

        private void netifsComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            NetworkInterface[] interfaces = NetworkInterface.GetAllNetworkInterfaces();
            foreach (NetworkInterface n in interfaces)
            {
                if (this.netifsComboBox.SelectedItem.ToString() == n.Description)
                {
                    foreach (UnicastIPAddressInformation info in n.GetIPProperties().UnicastAddresses)
                    {
                        if (info.Address.AddressFamily == AddressFamily.InterNetwork)
                        {
                            _current = info.Address;
                            break;
                        }
                    }
                    break;
                }
            }
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            GeneralSettings.LocalAddress = _current;
            Settings.Default.NetworkInterface = this.netifsComboBox.SelectedItem.ToString();
            Settings.Default.Save();
        }
    }

    public static class GeneralSettings
    {
        public static IPAddress LocalAddress;
    }
}
