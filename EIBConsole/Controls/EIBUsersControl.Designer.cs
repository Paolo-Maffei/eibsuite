namespace EIBConsole.Controls
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
            this.gbUserSettings = new System.Windows.Forms.GroupBox();
            this.lblDstMask = new System.Windows.Forms.Label();
            this.lbSrcMask = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.cbWritePolicy = new System.Windows.Forms.CheckBox();
            this.cbReadPolicy = new System.Windows.Forms.CheckBox();
            this.gbUsers = new System.Windows.Forms.GroupBox();
            this.toolStripCategories = new System.Windows.Forms.ToolStrip();
            this.btnSaveUsers = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.btnAddUser = new System.Windows.Forms.ToolStripButton();
            this.btnDeleteUser = new System.Windows.Forms.ToolStripButton();
            this.lvUsers = new System.Windows.Forms.ListView();
            this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader3 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader4 = new System.Windows.Forms.ColumnHeader();
            this.cbConsoleAccess = new System.Windows.Forms.CheckBox();
            this.gbUserSettings.SuspendLayout();
            this.gbUsers.SuspendLayout();
            this.toolStripCategories.SuspendLayout();
            this.SuspendLayout();
            // 
            // gbUserSettings
            // 
            this.gbUserSettings.Controls.Add(this.lblDstMask);
            this.gbUserSettings.Controls.Add(this.lbSrcMask);
            this.gbUserSettings.Controls.Add(this.label2);
            this.gbUserSettings.Controls.Add(this.label1);
            this.gbUserSettings.Controls.Add(this.cbConsoleAccess);
            this.gbUserSettings.Controls.Add(this.cbWritePolicy);
            this.gbUserSettings.Controls.Add(this.cbReadPolicy);
            this.gbUserSettings.Location = new System.Drawing.Point(438, 16);
            this.gbUserSettings.Name = "gbUserSettings";
            this.gbUserSettings.Size = new System.Drawing.Size(254, 247);
            this.gbUserSettings.TabIndex = 8;
            this.gbUserSettings.TabStop = false;
            this.gbUserSettings.Text = "User Settings";
            // 
            // lblDstMask
            // 
            this.lblDstMask.AutoSize = true;
            this.lblDstMask.Location = new System.Drawing.Point(150, 57);
            this.lblDstMask.Name = "lblDstMask";
            this.lblDstMask.Size = new System.Drawing.Size(18, 13);
            this.lblDstMask.TabIndex = 6;
            this.lblDstMask.Text = "0x";
            // 
            // lbSrcMask
            // 
            this.lbSrcMask.AutoSize = true;
            this.lbSrcMask.Location = new System.Drawing.Point(150, 24);
            this.lbSrcMask.Name = "lbSrcMask";
            this.lbSrcMask.Size = new System.Drawing.Size(18, 13);
            this.lbSrcMask.TabIndex = 5;
            this.lbSrcMask.Text = "0x";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 57);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(130, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "Destination Address Mask";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 24);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(111, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "Source Address Mask";
            // 
            // cbWritePolicy
            // 
            this.cbWritePolicy.AutoSize = true;
            this.cbWritePolicy.Enabled = false;
            this.cbWritePolicy.Location = new System.Drawing.Point(9, 113);
            this.cbWritePolicy.Name = "cbWritePolicy";
            this.cbWritePolicy.Size = new System.Drawing.Size(127, 17);
            this.cbWritePolicy.TabIndex = 0;
            this.cbWritePolicy.Text = "Write To EIB Allowed";
            this.cbWritePolicy.UseVisualStyleBackColor = true;
            // 
            // cbReadPolicy
            // 
            this.cbReadPolicy.AutoSize = true;
            this.cbReadPolicy.Enabled = false;
            this.cbReadPolicy.Location = new System.Drawing.Point(9, 90);
            this.cbReadPolicy.Name = "cbReadPolicy";
            this.cbReadPolicy.Size = new System.Drawing.Size(138, 17);
            this.cbReadPolicy.TabIndex = 0;
            this.cbReadPolicy.Text = "Read From EIB Allowed";
            this.cbReadPolicy.UseVisualStyleBackColor = true;
            // 
            // gbUsers
            // 
            this.gbUsers.Controls.Add(this.toolStripCategories);
            this.gbUsers.Controls.Add(this.lvUsers);
            this.gbUsers.Location = new System.Drawing.Point(16, 16);
            this.gbUsers.Name = "gbUsers";
            this.gbUsers.Size = new System.Drawing.Size(406, 247);
            this.gbUsers.TabIndex = 7;
            this.gbUsers.TabStop = false;
            this.gbUsers.Text = "Users";
            // 
            // toolStripCategories
            // 
            this.toolStripCategories.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btnSaveUsers,
            this.toolStripSeparator1,
            this.btnAddUser,
            this.btnDeleteUser});
            this.toolStripCategories.Location = new System.Drawing.Point(3, 16);
            this.toolStripCategories.Name = "toolStripCategories";
            this.toolStripCategories.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
            this.toolStripCategories.Size = new System.Drawing.Size(400, 25);
            this.toolStripCategories.TabIndex = 5;
            this.toolStripCategories.Text = "toolStrip1";
            // 
            // btnSaveUsers
            // 
            this.btnSaveUsers.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnSaveUsers.Image = ((System.Drawing.Image)(resources.GetObject("btnSaveUsers.Image")));
            this.btnSaveUsers.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnSaveUsers.Name = "btnSaveUsers";
            this.btnSaveUsers.Size = new System.Drawing.Size(23, 22);
            this.btnSaveUsers.Text = "Save";
            this.btnSaveUsers.ToolTipText = "Save Users list";
            this.btnSaveUsers.Click += new System.EventHandler(this.btnSaveUsers_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
            // 
            // btnAddUser
            // 
            this.btnAddUser.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnAddUser.Image = ((System.Drawing.Image)(resources.GetObject("btnAddUser.Image")));
            this.btnAddUser.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnAddUser.Name = "btnAddUser";
            this.btnAddUser.Size = new System.Drawing.Size(23, 22);
            this.btnAddUser.Text = "Add New User";
            this.btnAddUser.Click += new System.EventHandler(this.btnAddUser_Click);
            // 
            // btnDeleteUser
            // 
            this.btnDeleteUser.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnDeleteUser.Enabled = false;
            this.btnDeleteUser.Image = ((System.Drawing.Image)(resources.GetObject("btnDeleteUser.Image")));
            this.btnDeleteUser.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnDeleteUser.Name = "btnDeleteUser";
            this.btnDeleteUser.Size = new System.Drawing.Size(23, 22);
            this.btnDeleteUser.Text = "toolStripButton2";
            this.btnDeleteUser.ToolTipText = "Delete selected User";
            this.btnDeleteUser.Click += new System.EventHandler(this.btnDeleteUser_Click);
            // 
            // lvUsers
            // 
            this.lvUsers.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2,
            this.columnHeader3,
            this.columnHeader4});
            this.lvUsers.FullRowSelect = true;
            this.lvUsers.HideSelection = false;
            this.lvUsers.Location = new System.Drawing.Point(6, 46);
            this.lvUsers.MultiSelect = false;
            this.lvUsers.Name = "lvUsers";
            this.lvUsers.Size = new System.Drawing.Size(394, 185);
            this.lvUsers.TabIndex = 4;
            this.lvUsers.UseCompatibleStateImageBehavior = false;
            this.lvUsers.View = System.Windows.Forms.View.Details;
            this.lvUsers.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.lvUsers_MouseDoubleClick);
            this.lvUsers.SelectedIndexChanged += new System.EventHandler(this.lvUsers_SelectedIndexChanged);
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Name";
            this.columnHeader1.Width = 75;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "IP Address";
            this.columnHeader2.Width = 97;
            // 
            // columnHeader3
            // 
            this.columnHeader3.Text = "Session ID";
            this.columnHeader3.Width = 101;
            // 
            // columnHeader4
            // 
            this.columnHeader4.Text = "Connected";
            this.columnHeader4.Width = 82;
            // 
            // cbConsoleAccess
            // 
            this.cbConsoleAccess.AutoSize = true;
            this.cbConsoleAccess.Enabled = false;
            this.cbConsoleAccess.Location = new System.Drawing.Point(9, 136);
            this.cbConsoleAccess.Name = "cbConsoleAccess";
            this.cbConsoleAccess.Size = new System.Drawing.Size(134, 17);
            this.cbConsoleAccess.TabIndex = 0;
            this.cbConsoleAccess.Text = "Access to EIB Console";
            this.cbConsoleAccess.UseVisualStyleBackColor = true;
            // 
            // EIBUsersControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.gbUserSettings);
            this.Controls.Add(this.gbUsers);
            this.Name = "EIBUsersControl";
            this.Size = new System.Drawing.Size(711, 278);
            this.Load += new System.EventHandler(this.EIBUsersControl_Load);
            this.gbUserSettings.ResumeLayout(false);
            this.gbUserSettings.PerformLayout();
            this.gbUsers.ResumeLayout(false);
            this.gbUsers.PerformLayout();
            this.toolStripCategories.ResumeLayout(false);
            this.toolStripCategories.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox gbUserSettings;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.CheckBox cbWritePolicy;
        private System.Windows.Forms.CheckBox cbReadPolicy;
        private System.Windows.Forms.GroupBox gbUsers;
        private System.Windows.Forms.ToolStrip toolStripCategories;
        private System.Windows.Forms.ToolStripButton btnSaveUsers;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripButton btnAddUser;
        private System.Windows.Forms.ToolStripButton btnDeleteUser;
        private System.Windows.Forms.ListView lvUsers;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.ColumnHeader columnHeader3;
        private System.Windows.Forms.ColumnHeader columnHeader4;
        private System.Windows.Forms.Label lblDstMask;
        private System.Windows.Forms.Label lbSrcMask;
        private System.Windows.Forms.CheckBox cbConsoleAccess;
    }
}
