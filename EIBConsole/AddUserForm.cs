using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using EIBConsole.Conf;

namespace EIBConsole
{
    public partial class AddUserForm : Form
    {
        EIBUserConf _user = null;

        public AddUserForm()
        {
            InitializeComponent();
        }

        public AddUserForm(EIBUserConf user)
        {
            InitializeComponent();

            _user = user;
            
            this.tbUserName.Text = _user.Name;
            this.tbmaskPassword.Text = _user.Password;
            this.tbmaskRetypePassword.Text = _user.Password;

            this.cbReadAllowed.Checked = (_user.Priviliges & EIBUsersConf.USER_POLICY_READ_ACCESS) != 0;
            this.cbWriteAllowed.Checked = (_user.Priviliges & EIBUsersConf.USER_POLICY_WRITE_ACCESS) != 0;
            this.cbConsoleAccss.Checked = (_user.Priviliges & EIBUsersConf.USER_POLICY_CONSOLE_ACCESS) != 0;

            this.tbSAMask.Text = "0x" + _user.SrcMask.ToString("X");
            this.tbDAMask.Text = "0x" + _user.DstMask.ToString("X");
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            if (!ValidateData())
            {
                return;
            }

            int old_sess_id = _user != null ? _user.SessionId : -1;

            _user = new EIBUserConf();
            _user.SessionId = old_sess_id;
            _user.Name = this.tbUserName.Text;
            _user.Password = this.tbmaskPassword.Text;
            
            _user.Priviliges = 0;
            if (this.cbReadAllowed.Checked)
            {
                _user.Priviliges |= EIBUsersConf.USER_POLICY_READ_ACCESS;
            }
            if (this.cbWriteAllowed.Checked)
            {
                _user.Priviliges |= EIBUsersConf.USER_POLICY_WRITE_ACCESS;
            }
            if (this.cbConsoleAccss.Checked)
            {
                _user.Priviliges |= EIBUsersConf.USER_POLICY_CONSOLE_ACCESS;
            }

            _user.SrcMask = ushort.Parse(this.tbSAMask.Text.Substring(2, this.tbSAMask.Text.Length - 2), System.Globalization.NumberStyles.HexNumber);
            _user.DstMask = ushort.Parse(this.tbDAMask.Text.Substring(2, this.tbDAMask.Text.Length - 2), System.Globalization.NumberStyles.HexNumber);
        }

        public EIBUserConf CurrentUser
        {
            get
            {
                return _user;
            }
        }

        bool ValidateData()
        {
            if (this.tbUserName.Text.Trim().Length == 0)
            {
                MessageBox.Show("Please insert user name. empty user name is not allowed.");
                return false;
            }
            
            if (this.tbmaskPassword.Text != this.tbmaskRetypePassword.Text)
            {
                MessageBox.Show("Mismatch in password. please type again");
                return false;
            }

            try
            {
                ushort.Parse(this.tbSAMask.Text.Substring(2,this.tbSAMask.Text.Length - 2) , System.Globalization.NumberStyles.HexNumber);
                ushort.Parse(this.tbDAMask.Text.Substring(2, this.tbDAMask.Text.Length - 2), System.Globalization.NumberStyles.HexNumber);
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message);
                return false;
            }
                
            return true;
        }
    }
}
