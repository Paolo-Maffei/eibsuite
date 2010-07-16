using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Net;

namespace EIBMobile.Controls
{
    public partial class IPAddressControl : UserControl
    {
        Pen _pen;

        public IPAddressControl()
        {
            InitializeComponent();
            _pen = new Pen(Color.Black);
            this.tb1.TextAlign = HorizontalAlignment.Center;
            this.tb2.TextAlign = HorizontalAlignment.Center;
            this.tb3.TextAlign = HorizontalAlignment.Center;
            this.tb4.TextAlign = HorizontalAlignment.Center;
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            Rectangle rec = new Rectangle(e.ClipRectangle.X, e.ClipRectangle.Y, e.ClipRectangle.Width - 2, e.ClipRectangle.Height - 2);
            e.Graphics.DrawRectangle(_pen, rec);
            base.OnPaint(e);
        }

        public override Color BackColor
        {
            get
            {
                return base.BackColor;
            }
            set
            {
                this.tb1.BackColor = value;
                this.tb2.BackColor = value;
                this.tb3.BackColor = value;
                this.tb4.BackColor = value;
                this.label1.BackColor = value;
                this.label2.BackColor = value;
                this.label3.BackColor = value;
                base.BackColor = value;
            }
        }

        public override string Text
        {
            get
            {
                string text = string.Empty;
                if (!String.IsNullOrEmpty(this.tb1.Text) &&
                    !String.IsNullOrEmpty(this.tb2.Text) &&
                    !String.IsNullOrEmpty(this.tb3.Text) &&
                    !String.IsNullOrEmpty(this.tb4.Text))
                {
                    text = this.tb1.Text;
                    text += '.';
                    text += this.tb2.Text;
                    text += '.';
                    text += this.tb3.Text;
                    text += '.';
                    text += this.tb4.Text;
                }
                return text;
            }
            set
            {
                try
                {
                    IPAddress ip = IPAddress.Parse(value);
                    byte[] data = ip.GetAddressBytes();
                    this.tb1.Text = data[0].ToString();
                    this.tb2.Text = data[1].ToString();
                    this.tb3.Text = data[2].ToString();
                    this.tb4.Text = data[3].ToString();
                }
                catch (Exception)
                {
                }
            }
        }

        private void tb1_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == '.')
            {
                this.tb2.Focus();
                e.Handled = true;
            }
            HnadleKeyPress(this.tb1, e);
        }

        private void tb2_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == '.')
            {
                this.tb3.Focus();
                e.Handled = true;
            }
            HnadleKeyPress(this.tb2, e);
        }

        private void tb3_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == '.')
            {
                this.tb4.Focus();
                e.Handled = true;
            }
            HnadleKeyPress(this.tb3, e);
        }

        private void tb4_KeyPress(object sender, KeyPressEventArgs e)
        {
            HnadleKeyPress(this.tb4, e);
        }

        private void HnadleKeyPress(TextBox tb, KeyPressEventArgs e)
        {
            try
            {
                if(!IsCharDigit(e.KeyChar))
                {
                    e.Handled = true;
                    return;
                }

                if(!String.IsNullOrEmpty(tb.Text))
                {
                    int val = int.Parse(tb.Text);
                    if (!(val + (int)e.KeyChar >= 0 && val + (int)e.KeyChar <= 255))
                    {
                        e.Handled = true;
                    }
                }
            }
            catch (Exception)
            {
                e.Handled = true;
                return;
            }
        }

        private bool IsCharDigit(char c)
        {
            int x = (int)c;
            return (x >= 48 || x <= 57);
        }
    }
}
