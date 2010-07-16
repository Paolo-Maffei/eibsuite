namespace EIBConsole
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.bottomStatusStrip = new System.Windows.Forms.StatusStrip();
            this.ConsoleStateLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.lblServerStatus = new System.Windows.Forms.ToolStripStatusLabel();
            this.tcConsole = new System.Windows.Forms.TabControl();
            this.tpEIBInterfaceConfig = new System.Windows.Forms.TabPage();
            this.eibInterfaceControl1 = new EIBConsole.Controls.EIBInterfaceControl();
            this.tpUsers = new System.Windows.Forms.TabPage();
            this.eibUsersControl1 = new EIBConsole.Controls.EIBUsersControl();
            this.tpBusMonitor = new System.Windows.Forms.TabPage();
            this.eibBusMonitorControl1 = new EIBConsole.Controls.EIBBusMonitorControl();
            this.imageList1 = new System.Windows.Forms.ImageList(this.components);
            this.topToolStrip = new System.Windows.Forms.ToolStrip();
            this.btnConnect = new System.Windows.Forms.ToolStripButton();
            this.btnDisconnect = new System.Windows.Forms.ToolStripButton();
            this.btnRefresh = new System.Windows.Forms.ToolStripButton();
            this.KeepAliveThread = new System.ComponentModel.BackgroundWorker();
            this.bottomStatusStrip.SuspendLayout();
            this.tcConsole.SuspendLayout();
            this.tpEIBInterfaceConfig.SuspendLayout();
            this.tpUsers.SuspendLayout();
            this.tpBusMonitor.SuspendLayout();
            this.topToolStrip.SuspendLayout();
            this.SuspendLayout();
            // 
            // bottomStatusStrip
            // 
            this.bottomStatusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ConsoleStateLabel,
            this.lblServerStatus});
            this.bottomStatusStrip.Location = new System.Drawing.Point(0, 359);
            this.bottomStatusStrip.Name = "bottomStatusStrip";
            this.bottomStatusStrip.Size = new System.Drawing.Size(732, 22);
            this.bottomStatusStrip.TabIndex = 0;
            this.bottomStatusStrip.Text = "statusStrip1";
            // 
            // ConsoleStateLabel
            // 
            this.ConsoleStateLabel.Name = "ConsoleStateLabel";
            this.ConsoleStateLabel.Size = new System.Drawing.Size(0, 17);
            // 
            // lblServerStatus
            // 
            this.lblServerStatus.Name = "lblServerStatus";
            this.lblServerStatus.Size = new System.Drawing.Size(79, 17);
            this.lblServerStatus.Text = "Disconnected";
            this.lblServerStatus.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // tcConsole
            // 
            this.tcConsole.Controls.Add(this.tpEIBInterfaceConfig);
            this.tcConsole.Controls.Add(this.tpUsers);
            this.tcConsole.Controls.Add(this.tpBusMonitor);
            this.tcConsole.ImageList = this.imageList1;
            this.tcConsole.Location = new System.Drawing.Point(0, 28);
            this.tcConsole.Name = "tcConsole";
            this.tcConsole.SelectedIndex = 0;
            this.tcConsole.Size = new System.Drawing.Size(732, 327);
            this.tcConsole.TabIndex = 5;
            // 
            // tpEIBInterfaceConfig
            // 
            this.tpEIBInterfaceConfig.Controls.Add(this.eibInterfaceControl1);
            this.tpEIBInterfaceConfig.Location = new System.Drawing.Point(4, 23);
            this.tpEIBInterfaceConfig.Name = "tpEIBInterfaceConfig";
            this.tpEIBInterfaceConfig.Size = new System.Drawing.Size(724, 300);
            this.tpEIBInterfaceConfig.TabIndex = 0;
            this.tpEIBInterfaceConfig.Text = "EIB Interface";
            this.tpEIBInterfaceConfig.UseVisualStyleBackColor = true;
            // 
            // eibInterfaceControl1
            // 
            this.eibInterfaceControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.eibInterfaceControl1.Location = new System.Drawing.Point(0, 0);
            this.eibInterfaceControl1.Name = "eibInterfaceControl1";
            this.eibInterfaceControl1.Size = new System.Drawing.Size(724, 300);
            this.eibInterfaceControl1.TabIndex = 0;
            // 
            // tpUsers
            // 
            this.tpUsers.Controls.Add(this.eibUsersControl1);
            this.tpUsers.Location = new System.Drawing.Point(4, 23);
            this.tpUsers.Name = "tpUsers";
            this.tpUsers.Padding = new System.Windows.Forms.Padding(3);
            this.tpUsers.Size = new System.Drawing.Size(724, 300);
            this.tpUsers.TabIndex = 1;
            this.tpUsers.Tag = "";
            this.tpUsers.Text = "Users Management";
            this.tpUsers.UseVisualStyleBackColor = true;
            // 
            // eibUsersControl1
            // 
            this.eibUsersControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.eibUsersControl1.Location = new System.Drawing.Point(3, 3);
            this.eibUsersControl1.Name = "eibUsersControl1";
            this.eibUsersControl1.Size = new System.Drawing.Size(718, 294);
            this.eibUsersControl1.TabIndex = 1;
            // 
            // tpBusMonitor
            // 
            this.tpBusMonitor.Controls.Add(this.eibBusMonitorControl1);
            this.tpBusMonitor.Location = new System.Drawing.Point(4, 23);
            this.tpBusMonitor.Name = "tpBusMonitor";
            this.tpBusMonitor.Padding = new System.Windows.Forms.Padding(3);
            this.tpBusMonitor.Size = new System.Drawing.Size(724, 300);
            this.tpBusMonitor.TabIndex = 2;
            this.tpBusMonitor.Text = "Bus Monitor";
            this.tpBusMonitor.UseVisualStyleBackColor = true;
            // 
            // eibBusMonitorControl1
            // 
            this.eibBusMonitorControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.eibBusMonitorControl1.Location = new System.Drawing.Point(3, 3);
            this.eibBusMonitorControl1.Name = "eibBusMonitorControl1";
            this.eibBusMonitorControl1.Size = new System.Drawing.Size(718, 294);
            this.eibBusMonitorControl1.TabIndex = 0;
            // 
            // imageList1
            // 
            this.imageList1.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList1.ImageStream")));
            this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
            this.imageList1.Images.SetKeyName(0, "Users.png");
            this.imageList1.Images.SetKeyName(1, "server.ico");
            // 
            // topToolStrip
            // 
            this.topToolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btnConnect,
            this.btnDisconnect,
            this.btnRefresh});
            this.topToolStrip.Location = new System.Drawing.Point(0, 0);
            this.topToolStrip.Name = "topToolStrip";
            this.topToolStrip.Size = new System.Drawing.Size(732, 25);
            this.topToolStrip.TabIndex = 6;
            this.topToolStrip.Text = "toolStrip1";
            // 
            // btnConnect
            // 
            this.btnConnect.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnConnect.Image = ((System.Drawing.Image)(resources.GetObject("btnConnect.Image")));
            this.btnConnect.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnConnect.Name = "btnConnect";
            this.btnConnect.Size = new System.Drawing.Size(23, 22);
            this.btnConnect.Text = "toolStripButton2";
            this.btnConnect.ToolTipText = "Connect";
            this.btnConnect.Click += new System.EventHandler(this.btnConnect_Click);
            // 
            // btnDisconnect
            // 
            this.btnDisconnect.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnDisconnect.Image = ((System.Drawing.Image)(resources.GetObject("btnDisconnect.Image")));
            this.btnDisconnect.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnDisconnect.Name = "btnDisconnect";
            this.btnDisconnect.Size = new System.Drawing.Size(23, 22);
            this.btnDisconnect.Text = "toolStripButton1";
            this.btnDisconnect.ToolTipText = "Disconnect";
            this.btnDisconnect.Click += new System.EventHandler(this.btnDisconnect_Click);
            // 
            // btnRefresh
            // 
            this.btnRefresh.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnRefresh.Image = global::EIBConsole.Properties.Resources.refresh_view;
            this.btnRefresh.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnRefresh.Name = "btnRefresh";
            this.btnRefresh.Size = new System.Drawing.Size(23, 22);
            this.btnRefresh.Text = "toolStripButton3";
            this.btnRefresh.ToolTipText = "Refresh";
            this.btnRefresh.Click += new System.EventHandler(this.btnRefresh_Click);
            // 
            // KeepAliveThread
            // 
            this.KeepAliveThread.WorkerReportsProgress = true;
            this.KeepAliveThread.WorkerSupportsCancellation = true;
            this.KeepAliveThread.DoWork += new System.ComponentModel.DoWorkEventHandler(this.KeepAliveThread_DoWork);
            this.KeepAliveThread.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.KeepAliveThread_RunWorkerCompleted);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(732, 381);
            this.Controls.Add(this.topToolStrip);
            this.Controls.Add(this.tcConsole);
            this.Controls.Add(this.bottomStatusStrip);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "MainForm";
            this.Text = "EIB Console";
            this.bottomStatusStrip.ResumeLayout(false);
            this.bottomStatusStrip.PerformLayout();
            this.tcConsole.ResumeLayout(false);
            this.tpEIBInterfaceConfig.ResumeLayout(false);
            this.tpUsers.ResumeLayout(false);
            this.tpBusMonitor.ResumeLayout(false);
            this.topToolStrip.ResumeLayout(false);
            this.topToolStrip.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.StatusStrip bottomStatusStrip;
        private System.Windows.Forms.ToolStripStatusLabel ConsoleStateLabel;
        private System.Windows.Forms.ToolStripStatusLabel lblServerStatus;
        private System.Windows.Forms.TabControl tcConsole;
        private System.Windows.Forms.TabPage tpEIBInterfaceConfig;
        private System.Windows.Forms.TabPage tpUsers;
        private System.Windows.Forms.ToolStrip topToolStrip;
        private System.Windows.Forms.ToolStripButton btnConnect;
        private System.Windows.Forms.ToolStripButton btnRefresh;
        private System.Windows.Forms.ImageList imageList1;
        private EIBConsole.Controls.EIBUsersControl eibUsersControl1;
        private System.Windows.Forms.ToolStripButton btnDisconnect;
        private EIBConsole.Controls.EIBInterfaceControl eibInterfaceControl1;
        private System.ComponentModel.BackgroundWorker KeepAliveThread;
        private System.Windows.Forms.TabPage tpBusMonitor;
        private EIBConsole.Controls.EIBBusMonitorControl eibBusMonitorControl1;
    }
}

