using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using EIBConsole.Conf;

namespace EIBConsole.Controls
{
    public partial class EIBUsersControl : UserControl, IRefreshable
    {
        private bool _dirty_flag = false;
        private EIBUsersConf _users = null;

        public EIBUsersControl()
        {
            InitializeComponent();
            
            this.btnSaveUsers.Enabled = false;
        }

        private void btnSaveUsers_Click(object sender, EventArgs e)
        {
            if (ConsoleAPI.SetUsersList(_users))
            {
                _dirty_flag = false;
            }
            else
            {
                _dirty_flag = true;
            }
        }

        private void lvUsers_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (this.lvUsers.SelectedItems.Count == 1)
            {
                UpdatSpecificeUserSettings((EIBUserConf)this.lvUsers.SelectedItems[0].Tag);
                this.btnDeleteUser.Enabled = true;
                return;
            }

            if (this.lvUsers.SelectedItems.Count == 0)
            {
                this.btnDeleteUser.Enabled = false;
            }
        }

        private void UpdatSpecificeUserSettings(EIBUserConf user)
        {
            this.cbReadPolicy.Checked = ((user.Priviliges & EIBUsersConf.USER_POLICY_READ_ACCESS) != 0);
            this.cbWritePolicy.Checked = ((user.Priviliges & EIBUsersConf.USER_POLICY_WRITE_ACCESS ) != 0);
            this.cbConsoleAccess.Checked = ((user.Priviliges & EIBUsersConf.USER_POLICY_CONSOLE_ACCESS) != 0);

            this.lbSrcMask.Text = "0x" + user.SrcMask.ToString("X4");
            this.lblDstMask.Text = "0x" + user.DstMask.ToString("X4");
        }

        private void btnDeleteUser_Click(object sender, EventArgs e)
        {
            _dirty_flag = true;
            EIBUserConf user = (EIBUserConf)this.lvUsers.SelectedItems[0].Tag;
            int index = this.lvUsers.SelectedItems[0].Index;
            _users.List.Remove(user);
            UpdateListView();
            if (this.lvUsers.SelectedItems.Count == 0 && this.lvUsers.Items.Count > 0)
            {
                if (index == 0)
                {
                    this.lvUsers.Items[0].Selected = true;
                }
                else
                {
                    this.lvUsers.Items[index - 1].Selected = true;
                }
            }
        }

        private void UpdateListView()
        {
            //clear the view first
            this.lvUsers.Items.Clear();

            //now refill with all users in the list
            foreach (EIBUserConf user in _users.List)
            {
                string[] data = new string[5];
                data[0] = user.Name;
                data[1] = (user.Ip_address == null || user.Ip_address == string.Empty) ? "0.0.0.0" : user.Ip_address;
                data[2] = user.SessionId.ToString();
                data[3] = user.Connected.ToString();
                ListViewItem item = new ListViewItem(data);
                
                item.Tag = user;
                this.lvUsers.Items.Add(item);
            }
        }

        private void btnAddUser_Click(object sender, EventArgs e)
        {
            _dirty_flag = true;

            AddUserForm add_form = new AddUserForm();
            if (add_form.ShowDialog() == DialogResult.OK)
            {
                if (_users == null)
                {
                    _users = new EIBUsersConf();
                }
                _users.List.Add(add_form.CurrentUser);
                UpdateListView();
            }
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            if (!_dirty_flag)
            {
                this.btnSaveUsers.Enabled = false;
            }
            else
            {
                this.btnSaveUsers.Enabled = true;
            }
            base.OnPaint(e);
        }

        private void EIBUsersControl_Load(object sender, EventArgs e)
        {
            this.Dock = DockStyle.Fill;
        }

        private void lvUsers_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            ListViewHitTestInfo hit = lvUsers.HitTest(e.Location);
            if (hit != null)
            {
                AddUserForm add_form = new AddUserForm((EIBUserConf)hit.Item.Tag);
                if (add_form.ShowDialog() == DialogResult.OK)
                {
                    _dirty_flag = true;
                    this._users.List.Remove((EIBUserConf)hit.Item.Tag);
                    this._users.List.Add(add_form.CurrentUser);
                    UpdateListView();
                    UpdatSpecificeUserSettings(add_form.CurrentUser);
                }
            }
        }

        #region IRefreshable Members

        void IRefreshable.Refresh()
        {
            _users = ConsoleAPI.GetUsersList();
            UpdateListView();
        }

        public void OnDisconnect()
        {
            this._dirty_flag = false;
            if (_users != null && _users.List != null)
            {
                this._users.List.Clear();
                UpdateListView();
            }
        }

        public void OnConnect()
        {
            throw new NotImplementedException();
        }

        #endregion
    }
}
