namespace EIBMobile.Controls
{
    partial class EIBUsersControl
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(EIBUsersControl));
            this.lvUsers = new System.Windows.Forms.ListView();
            this.chUserName = new System.Windows.Forms.ColumnHeader();
            this.chIPAddress = new System.Windows.Forms.ColumnHeader();
            this.chIsConnected = new System.Windows.Forms.ColumnHeader();
            this.lblDstMaskVal = new System.Windows.Forms.Label();
            this.lblSrcMask = new System.Windows.Forms.Label();
            this.cbReadAllowed = new System.Windows.Forms.CheckBox();
            this.cbWriteAllowed = new System.Windows.Forms.CheckBox();
            this.lblDstMask = new System.Windows.Forms.Label();
            this.lblSrcMaskVal = new System.Windows.Forms.Label();
            this.btnSaveUsers = new EIBMobile.Controls.ImageButton();
            this.btnDeleteUser = new EIBMobile.Controls.ImageButton();
            this.btnAddUser = new EIBMobile.Controls.ImageButton();
            this.SuspendLayout();
            // 
            // lvUsers
            // 
            this.lvUsers.Columns.Add(this.chUserName);
            this.lvUsers.Columns.Add(this.chIPAddress);
            this.lvUsers.Columns.Add(this.chIsConnected);
            this.lvUsers.FullRowSelect = true;
            this.lvUsers.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
            this.lvUsers.Location = new System.Drawing.Point(0, 20);
            this.lvUsers.Name = "lvUsers";
            this.lvUsers.Size = new System.Drawing.Size(243, 92);
            this.lvUsers.TabIndex = 0;
            this.lvUsers.View = System.Windows.Forms.View.Details;
            this.lvUsers.SelectedIndexChanged += new System.EventHandler(this.lvUsers_SelectedIndexChanged);
            // 
            // chUserName
            // 
            this.chUserName.Text = "Name";
            this.chUserName.Width = 60;
            // 
            // chIPAddress
            // 
            this.chIPAddress.Text = "Address";
            this.chIPAddress.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.chIPAddress.Width = 108;
            // 
            // chIsConnected
            // 
            this.chIsConnected.Text = "Connected";
            this.chIsConnected.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.chIsConnected.Width = 72;
            // 
            // lblDstMaskVal
            // 
            this.lblDstMaskVal.Location = new System.Drawing.Point(139, 181);
            this.lblDstMaskVal.Name = "lblDstMaskVal";
            this.lblDstMaskVal.Size = new System.Drawing.Size(59, 20);
            this.lblDstMaskVal.Text = "0x0";
            // 
            // lblSrcMask
            // 
            this.lblSrcMask.Location = new System.Drawing.Point(3, 161);
            this.lblSrcMask.Name = "lblSrcMask";
            this.lblSrcMask.Size = new System.Drawing.Size(84, 20);
            this.lblSrcMask.Text = "Source Mask: ";
            // 
            // cbReadAllowed
            // 
            this.cbReadAllowed.Location = new System.Drawing.Point(0, 132);
            this.cbReadAllowed.Name = "cbReadAllowed";
            this.cbReadAllowed.Size = new System.Drawing.Size(104, 20);
            this.cbReadAllowed.TabIndex = 3;
            this.cbReadAllowed.Text = "Read Allowed";
            // 
            // cbWriteAllowed
            // 
            this.cbWriteAllowed.Location = new System.Drawing.Point(121, 132);
            this.cbWriteAllowed.Name = "cbWriteAllowed";
            this.cbWriteAllowed.Size = new System.Drawing.Size(109, 20);
            this.cbWriteAllowed.TabIndex = 3;
            this.cbWriteAllowed.Text = "Write Allowed";
            // 
            // lblDstMask
            // 
            this.lblDstMask.Location = new System.Drawing.Point(3, 181);
            this.lblDstMask.Name = "lblDstMask";
            this.lblDstMask.Size = new System.Drawing.Size(108, 20);
            this.lblDstMask.Text = "Destination Mask:";
            // 
            // lblSrcMaskVal
            // 
            this.lblSrcMaskVal.Location = new System.Drawing.Point(139, 161);
            this.lblSrcMaskVal.Name = "lblSrcMaskVal";
            this.lblSrcMaskVal.Size = new System.Drawing.Size(59, 20);
            this.lblSrcMaskVal.Text = "0x0";
            // 
            // btnSaveUsers
            // 
            this.btnSaveUsers.Image = ((System.Drawing.Image)(resources.GetObject("btnSaveUsers.Image")));
            this.btnSaveUsers.Location = new System.Drawing.Point(0, 3);
            this.btnSaveUsers.Name = "btnSaveUsers";
            this.btnSaveUsers.Size = new System.Drawing.Size(16, 16);
            this.btnSaveUsers.TabIndex = 6;
            this.btnSaveUsers.Text = "imageButton1";
            this.btnSaveUsers.Click += new System.EventHandler(this.btnSaveUsers_Click);
            // 
            // btnDeleteUser
            // 
            this.btnDeleteUser.Image = ((System.Drawing.Image)(resources.GetObject("btnDeleteUser.Image")));
            this.btnDeleteUser.Location = new System.Drawing.Point(44, 3);
            this.btnDeleteUser.Name = "btnDeleteUser";
            this.btnDeleteUser.Size = new System.Drawing.Size(16, 16);
            this.btnDeleteUser.TabIndex = 6;
            this.btnDeleteUser.Text = "imageButton1";
            // 
            // btnAddUser
            // 
            this.btnAddUser.Image = ((System.Drawing.Image)(resources.GetObject("btnAddUser.Image")));
            this.btnAddUser.Location = new System.Drawing.Point(22, 3);
            this.btnAddUser.Name = "btnAddUser";
            this.btnAddUser.Size = new System.Drawing.Size(16, 16);
            this.btnAddUser.TabIndex = 6;
            this.btnAddUser.Text = "imageButton1";
            // 
            // EIBUsersControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.BackColor = System.Drawing.SystemColors.Window;
            this.Controls.Add(this.btnSaveUsers);
            this.Controls.Add(this.btnDeleteUser);
            this.Controls.Add(this.btnAddUser);
            this.Controls.Add(this.cbWriteAllowed);
            this.Controls.Add(this.cbReadAllowed);
            this.Controls.Add(this.lblDstMask);
            this.Controls.Add(this.lblSrcMask);
            this.Controls.Add(this.lblSrcMaskVal);
            this.Controls.Add(this.lblDstMaskVal);
            this.Controls.Add(this.lvUsers);
            this.Name = "EIBUsersControl";
            this.Size = new System.Drawing.Size(243, 210);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListView lvUsers;
        private System.Windows.Forms.ColumnHeader chUserName;
        private System.Windows.Forms.ColumnHeader chIPAddress;
        private System.Windows.Forms.ColumnHeader chIsConnected;
        private System.Windows.Forms.Label lblDstMaskVal;
        private System.Windows.Forms.Label lblSrcMask;
        private System.Windows.Forms.CheckBox cbReadAllowed;
        private System.Windows.Forms.CheckBox cbWriteAllowed;
        private System.Windows.Forms.Label lblDstMask;
        private System.Windows.Forms.Label lblSrcMaskVal;
        private ImageButton btnAddUser;
        private ImageButton btnDeleteUser;
        private ImageButton btnSaveUsers;
    }
}
