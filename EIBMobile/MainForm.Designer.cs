namespace EIBMobile
{
    partial class MainForm
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

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.tabControl = new System.Windows.Forms.TabControl();
            this.tpInterface = new System.Windows.Forms.TabPage();
            this.eibInterfaceControl1 = new EIBMobile.Controls.EIBInterfaceControl();
            this.tpUsers = new System.Windows.Forms.TabPage();
            this.eibUsersControl1 = new EIBMobile.Controls.EIBUsersControl();
            this.tpBusMon = new System.Windows.Forms.TabPage();
            this.MainMenu = new System.Windows.Forms.MainMenu();
            this.miOptions = new System.Windows.Forms.MenuItem();
            this.miConnect = new System.Windows.Forms.MenuItem();
            this.miDisconnect = new System.Windows.Forms.MenuItem();
            this.menuItem1 = new System.Windows.Forms.MenuItem();
            this.miRefresh = new System.Windows.Forms.MenuItem();
            this.statusBar = new System.Windows.Forms.StatusBar();
            this.tabControl.SuspendLayout();
            this.tpInterface.SuspendLayout();
            this.tpUsers.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabControl
            // 
            this.tabControl.Controls.Add(this.tpInterface);
            this.tabControl.Controls.Add(this.tpUsers);
            this.tabControl.Controls.Add(this.tpBusMon);
            this.tabControl.Location = new System.Drawing.Point(0, 0);
            this.tabControl.Name = "tabControl";
            this.tabControl.SelectedIndex = 0;
            this.tabControl.Size = new System.Drawing.Size(240, 240);
            this.tabControl.TabIndex = 0;
            // 
            // tpInterface
            // 
            this.tpInterface.Controls.Add(this.eibInterfaceControl1);
            this.tpInterface.Location = new System.Drawing.Point(0, 0);
            this.tpInterface.Name = "tpInterface";
            this.tpInterface.Size = new System.Drawing.Size(240, 217);
            this.tpInterface.Text = "EIB Interface";
            // 
            // eibInterfaceControl1
            // 
            this.eibInterfaceControl1.BackColor = System.Drawing.SystemColors.Window;
            this.eibInterfaceControl1.Location = new System.Drawing.Point(0, 0);
            this.eibInterfaceControl1.Name = "eibInterfaceControl1";
            this.eibInterfaceControl1.Size = new System.Drawing.Size(240, 216);
            this.eibInterfaceControl1.TabIndex = 0;
            // 
            // tpUsers
            // 
            this.tpUsers.Controls.Add(this.eibUsersControl1);
            this.tpUsers.Location = new System.Drawing.Point(0, 0);
            this.tpUsers.Name = "tpUsers";
            this.tpUsers.Size = new System.Drawing.Size(240, 217);
            this.tpUsers.Text = "Users";
            // 
            // eibUsersControl1
            // 
            this.eibUsersControl1.BackColor = System.Drawing.SystemColors.Window;
            this.eibUsersControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.eibUsersControl1.Location = new System.Drawing.Point(0, 0);
            this.eibUsersControl1.Name = "eibUsersControl1";
            this.eibUsersControl1.Size = new System.Drawing.Size(240, 217);
            this.eibUsersControl1.TabIndex = 0;
            // 
            // tpBusMon
            // 
            this.tpBusMon.Location = new System.Drawing.Point(0, 0);
            this.tpBusMon.Name = "tpBusMon";
            this.tpBusMon.Size = new System.Drawing.Size(232, 214);
            this.tpBusMon.Text = "Bus monitor";
            // 
            // MainMenu
            // 
            this.MainMenu.MenuItems.Add(this.miOptions);
            this.MainMenu.MenuItems.Add(this.miRefresh);
            // 
            // miOptions
            // 
            this.miOptions.MenuItems.Add(this.miConnect);
            this.miOptions.MenuItems.Add(this.miDisconnect);
            this.miOptions.MenuItems.Add(this.menuItem1);
            this.miOptions.Text = "Options";
            // 
            // miConnect
            // 
            this.miConnect.Text = "Connect";
            this.miConnect.Click += new System.EventHandler(this.miConnect_Click);
            // 
            // miDisconnect
            // 
            this.miDisconnect.Text = "Disconnect";
            // 
            // menuItem1
            // 
            this.menuItem1.Text = "Exit";
            this.menuItem1.Click += new System.EventHandler(this.menuItem1_Click);
            // 
            // miRefresh
            // 
            this.miRefresh.Text = "Refresh";
            this.miRefresh.Click += new System.EventHandler(this.miRefresh_Click);
            // 
            // statusBar
            // 
            this.statusBar.Location = new System.Drawing.Point(0, 246);
            this.statusBar.Name = "statusBar";
            this.statusBar.Size = new System.Drawing.Size(240, 22);
            this.statusBar.Text = "Disconnected";
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.AutoScroll = true;
            this.ClientSize = new System.Drawing.Size(240, 268);
            this.Controls.Add(this.statusBar);
            this.Controls.Add(this.tabControl);
            this.Menu = this.MainMenu;
            this.Name = "MainForm";
            this.Text = "EIB Mobile";
            this.tabControl.ResumeLayout(false);
            this.tpInterface.ResumeLayout(false);
            this.tpUsers.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tabControl;
        private System.Windows.Forms.TabPage tpInterface;
        private System.Windows.Forms.TabPage tpUsers;
        private System.Windows.Forms.MainMenu MainMenu;
        private System.Windows.Forms.MenuItem miOptions;
        private System.Windows.Forms.MenuItem miConnect;
        private System.Windows.Forms.MenuItem miDisconnect;
        private System.Windows.Forms.StatusBar statusBar;
        private System.Windows.Forms.MenuItem miRefresh;
        private System.Windows.Forms.MenuItem menuItem1;
        private System.Windows.Forms.TabPage tpBusMon;
        private EIBMobile.Controls.EIBInterfaceControl eibInterfaceControl1;
        private EIBMobile.Controls.EIBUsersControl eibUsersControl1;

    }
}

