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
    public partial class EIBUsersControl : UserControl, IRefreshable
    {
        public EIBUsersControl()
        {
            InitializeComponent();
            this.lvUsers.CheckBoxes = true;
            UpdateView(null);
        }

        private void UpdateView(EIBUsersConf conf)
        {
            this.lvUsers.Items.Clear();

            if (conf == null)
            {
                this.btnSaveUsers.Enabled = false;
                this.btnAddUser.Enabled = false;
                this.btnDeleteUser.Enabled = false;
                this.cbReadAllowed.Checked = false;
                this.cbReadAllowed.Enabled = false;
                this.cbWriteAllowed.Checked = false;
                this.cbWriteAllowed.Enabled = false;
                this.lblSrcMaskVal.Text = "0x0";
                this.lblDstMaskVal.Text = "0x0"; 
                return;
            }

            foreach (EIBUserConf user in conf.List)
            {
                string[] texts = new string[3];
                texts[0] = user.Name;
                texts[1] = user.Ip_address;
                texts[2] = user.Connected.ToString();
                ListViewItem item = new ListViewItem(texts);
                this.lvUsers.Items.Add(item);
                item.Tag = user;
            }

            this.btnSaveUsers.Enabled = true;
            this.btnAddUser.Enabled = true;
        }

        #region IRefreshable Members

        void IRefreshable.Refresh()
        {
            Cursor.Current = Cursors.WaitCursor;
            EIBUsersConf conf = ConsoleAPI.GetUsersList();
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

        private void btnSaveUsers_Click(object sender, EventArgs e)
        {
            Cursor.Current = Cursors.WaitCursor;
            EIBUsersConf conf = new EIBUsersConf();
            foreach (ListViewItem item in this.lvUsers.Items)
            {
                EIBUserConf user = item.Tag as EIBUserConf;
                conf.List.Add(user);
            }

            bool res = ConsoleAPI.SetUsersList(conf);

            Cursor.Current = Cursors.Default;
        }

        private void lvUsers_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (this.lvUsers.SelectedIndices.Count != 1)
            {
                this.btnDeleteUser.Enabled = false;
                return;
            }

            EIBUserConf user = this.lvUsers.Items[this.lvUsers.SelectedIndices[0]].Tag as EIBUserConf;
            this.lblSrcMaskVal.Text = "0x" + user.SrcMask.ToString("X4");
            this.lblDstMaskVal.Text = "0x" + user.DstMask.ToString("X4");
            this.cbReadAllowed.Enabled = true;
            this.cbReadAllowed.Checked = (user.Priviliges & 1) != 0;
            this.cbWriteAllowed.Enabled = true;
            this.cbWriteAllowed.Checked = (user.Priviliges & 2) != 0;
        }
    }
}
