using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using EIBVoice.KNX;

namespace EIBVoice
{
    public partial class EIBAddressForm : Form
    {
        EIBAddress _current_val;

        public EIBAddressForm()
        {
            InitializeComponent();
            _current_val = new EIBAddress();
        }

        private void tbDecimal_KeyPress(object sender, System.Windows.Forms.KeyPressEventArgs e)
        {
            // Process only digits and backspace
            if (Char.IsDigit(e.KeyChar) == true || e.KeyChar == '\b')
                e.Handled = false;

            // Do not process other keys
            else
                e.Handled = true;
        }

        public EIBAddress CurrentValue
        {
            get { return _current_val; }
            set { _current_val = value; }
        }

        private void EIBAddressForm_Load(object sender, EventArgs e)
        {
            string addr = _current_val.ToString();
            this.tb1.Text = addr.Substring(0, addr.IndexOf('/'));
            this.tb2.Text = addr.Substring(addr.IndexOf('/') + 1, addr.LastIndexOf('/') - 2);
            this.tb3.Text = addr.Substring(addr.LastIndexOf('/') + 1 , addr.Length - addr.LastIndexOf('/') - 1);
        }

        private void tbDecimal_TextChanged(object sender, System.EventArgs e)
        {
            // To hold our converted unsigned type value
            uint uiDecimal = 0;
            try
            {
                uiDecimal = checked((uint)System.Convert.ToByte(((TextBox)sender).Text));
            }
            catch (System.OverflowException)
            {
                this.btnOK.Enabled = false;
                return;
            }
            catch (System.FormatException)
            {
                this.btnOK.Enabled = false;
                return;
            }
            this.btnOK.Enabled = true;
            if (tb1.Text.Length > 0 && tb2.Text.Length > 0 && tb3.Text.Length > 0)
            {
                _current_val = new EIBAddress(this.tb1.Text + "/" + this.tb2.Text + "/" + this.tb3.Text);
            }
        }
    }
}
