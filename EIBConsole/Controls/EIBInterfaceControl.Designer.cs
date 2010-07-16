namespace EIBConsole.Controls
{
    partial class EIBInterfaceControl
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(EIBInterfaceControl));
            this.gbDeviceMode = new System.Windows.Forms.GroupBox();
            this.EibPortNUD = new System.Windows.Forms.NumericUpDown();
            this.lblEibPort = new System.Windows.Forms.Label();
            this.rbModeTunneling = new System.Windows.Forms.RadioButton();
            this.rbModeRouting = new System.Windows.Forms.RadioButton();
            this.tbDeviceAddress = new IPAddressControlLib.IPAddressControl();
            this.lblDeviceAddress = new System.Windows.Forms.Label();
            this.cbAutoDetect = new System.Windows.Forms.CheckBox();
            this.gbDeviceStats = new System.Windows.Forms.GroupBox();
            this.lblLastReceivedTime = new System.Windows.Forms.Label();
            this.lblTotalReceived = new System.Windows.Forms.Label();
            this.lblTotalSent = new System.Windows.Forms.Label();
            this.lblLasttimeReceived = new System.Windows.Forms.Label();
            this.lblLasttimeSent = new System.Windows.Forms.Label();
            this.lblLastSentTime = new System.Windows.Forms.Label();
            this.lblTotalPktsSent = new System.Windows.Forms.Label();
            this.lblTotalPktsRecevied = new System.Windows.Forms.Label();
            this.gbDeviceManagment = new System.Windows.Forms.GroupBox();
            this.btnStopInterface = new System.Windows.Forms.Button();
            this.btnStartInterface = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.pbStatus = new System.Windows.Forms.PictureBox();
            this.imageList1 = new System.Windows.Forms.ImageList(this.components);
            this.gbDeviceMode.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.EibPortNUD)).BeginInit();
            this.gbDeviceStats.SuspendLayout();
            this.gbDeviceManagment.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pbStatus)).BeginInit();
            this.SuspendLayout();
            // 
            // gbDeviceMode
            // 
            this.gbDeviceMode.Controls.Add(this.EibPortNUD);
            this.gbDeviceMode.Controls.Add(this.lblEibPort);
            this.gbDeviceMode.Controls.Add(this.rbModeTunneling);
            this.gbDeviceMode.Controls.Add(this.rbModeRouting);
            this.gbDeviceMode.Controls.Add(this.tbDeviceAddress);
            this.gbDeviceMode.Controls.Add(this.lblDeviceAddress);
            this.gbDeviceMode.Controls.Add(this.cbAutoDetect);
            this.gbDeviceMode.Location = new System.Drawing.Point(18, 162);
            this.gbDeviceMode.Name = "gbDeviceMode";
            this.gbDeviceMode.Size = new System.Drawing.Size(469, 131);
            this.gbDeviceMode.TabIndex = 0;
            this.gbDeviceMode.TabStop = false;
            this.gbDeviceMode.Text = "Device Properties";
            // 
            // EibPortNUD
            // 
            this.EibPortNUD.Location = new System.Drawing.Point(256, 64);
            this.EibPortNUD.Maximum = new decimal(new int[] {
            65000,
            0,
            0,
            0});
            this.EibPortNUD.Name = "EibPortNUD";
            this.EibPortNUD.ReadOnly = true;
            this.EibPortNUD.Size = new System.Drawing.Size(64, 20);
            this.EibPortNUD.TabIndex = 9;
            // 
            // lblEibPort
            // 
            this.lblEibPort.AutoSize = true;
            this.lblEibPort.Location = new System.Drawing.Point(192, 66);
            this.lblEibPort.Name = "lblEibPort";
            this.lblEibPort.Size = new System.Drawing.Size(26, 13);
            this.lblEibPort.TabIndex = 8;
            this.lblEibPort.Text = "Port";
            // 
            // rbModeTunneling
            // 
            this.rbModeTunneling.AutoSize = true;
            this.rbModeTunneling.Location = new System.Drawing.Point(16, 64);
            this.rbModeTunneling.Name = "rbModeTunneling";
            this.rbModeTunneling.Size = new System.Drawing.Size(102, 17);
            this.rbModeTunneling.TabIndex = 1;
            this.rbModeTunneling.TabStop = true;
            this.rbModeTunneling.Text = "Mode Tunneling";
            this.rbModeTunneling.UseVisualStyleBackColor = true;
            // 
            // rbModeRouting
            // 
            this.rbModeRouting.AutoSize = true;
            this.rbModeRouting.Location = new System.Drawing.Point(16, 32);
            this.rbModeRouting.Name = "rbModeRouting";
            this.rbModeRouting.Size = new System.Drawing.Size(92, 17);
            this.rbModeRouting.TabIndex = 1;
            this.rbModeRouting.TabStop = true;
            this.rbModeRouting.Text = "Mode Routing";
            this.rbModeRouting.UseVisualStyleBackColor = true;
            this.rbModeRouting.CheckedChanged += new System.EventHandler(this.rbModeRouting_CheckedChanged);
            // 
            // tbDeviceAddress
            // 
            this.tbDeviceAddress.AllowInternalTab = false;
            this.tbDeviceAddress.AutoHeight = true;
            this.tbDeviceAddress.BackColor = System.Drawing.SystemColors.Window;
            this.tbDeviceAddress.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.tbDeviceAddress.Cursor = System.Windows.Forms.Cursors.IBeam;
            this.tbDeviceAddress.Location = new System.Drawing.Point(256, 31);
            this.tbDeviceAddress.MinimumSize = new System.Drawing.Size(87, 20);
            this.tbDeviceAddress.Name = "tbDeviceAddress";
            this.tbDeviceAddress.ReadOnly = false;
            this.tbDeviceAddress.Size = new System.Drawing.Size(102, 20);
            this.tbDeviceAddress.TabIndex = 10;
            this.tbDeviceAddress.Text = "0.0.0.0";
            // 
            // lblDeviceAddress
            // 
            this.lblDeviceAddress.AutoSize = true;
            this.lblDeviceAddress.Location = new System.Drawing.Point(192, 34);
            this.lblDeviceAddress.Name = "lblDeviceAddress";
            this.lblDeviceAddress.Size = new System.Drawing.Size(45, 13);
            this.lblDeviceAddress.TabIndex = 7;
            this.lblDeviceAddress.Text = "Address";
            // 
            // cbAutoDetect
            // 
            this.cbAutoDetect.AutoSize = true;
            this.cbAutoDetect.Location = new System.Drawing.Point(16, 97);
            this.cbAutoDetect.Name = "cbAutoDetect";
            this.cbAutoDetect.Size = new System.Drawing.Size(134, 17);
            this.cbAutoDetect.TabIndex = 0;
            this.cbAutoDetect.Text = "Device Auto Detection";
            this.cbAutoDetect.UseVisualStyleBackColor = true;
            // 
            // gbDeviceStats
            // 
            this.gbDeviceStats.Controls.Add(this.lblLastReceivedTime);
            this.gbDeviceStats.Controls.Add(this.lblTotalReceived);
            this.gbDeviceStats.Controls.Add(this.lblTotalSent);
            this.gbDeviceStats.Controls.Add(this.lblLasttimeReceived);
            this.gbDeviceStats.Controls.Add(this.lblLasttimeSent);
            this.gbDeviceStats.Controls.Add(this.lblLastSentTime);
            this.gbDeviceStats.Controls.Add(this.lblTotalPktsSent);
            this.gbDeviceStats.Controls.Add(this.lblTotalPktsRecevied);
            this.gbDeviceStats.Location = new System.Drawing.Point(18, 5);
            this.gbDeviceStats.Name = "gbDeviceStats";
            this.gbDeviceStats.Size = new System.Drawing.Size(348, 151);
            this.gbDeviceStats.TabIndex = 2;
            this.gbDeviceStats.TabStop = false;
            this.gbDeviceStats.Text = "Device Statistics";
            // 
            // lblLastReceivedTime
            // 
            this.lblLastReceivedTime.AutoSize = true;
            this.lblLastReceivedTime.Location = new System.Drawing.Point(6, 60);
            this.lblLastReceivedTime.Name = "lblLastReceivedTime";
            this.lblLastReceivedTime.Size = new System.Drawing.Size(139, 13);
            this.lblLastReceivedTime.TabIndex = 8;
            this.lblLastReceivedTime.Text = "Last Time Packet Received";
            // 
            // lblTotalReceived
            // 
            this.lblTotalReceived.AutoSize = true;
            this.lblTotalReceived.Location = new System.Drawing.Point(192, 117);
            this.lblTotalReceived.Name = "lblTotalReceived";
            this.lblTotalReceived.Size = new System.Drawing.Size(13, 13);
            this.lblTotalReceived.TabIndex = 8;
            this.lblTotalReceived.Text = "0";
            // 
            // lblTotalSent
            // 
            this.lblTotalSent.AutoSize = true;
            this.lblTotalSent.Location = new System.Drawing.Point(192, 90);
            this.lblTotalSent.Name = "lblTotalSent";
            this.lblTotalSent.Size = new System.Drawing.Size(13, 13);
            this.lblTotalSent.TabIndex = 8;
            this.lblTotalSent.Text = "0";
            // 
            // lblLasttimeReceived
            // 
            this.lblLasttimeReceived.AutoSize = true;
            this.lblLasttimeReceived.Location = new System.Drawing.Point(192, 60);
            this.lblLasttimeReceived.Name = "lblLasttimeReceived";
            this.lblLasttimeReceived.Size = new System.Drawing.Size(36, 13);
            this.lblLasttimeReceived.TabIndex = 8;
            this.lblLasttimeReceived.Text = "Never";
            // 
            // lblLasttimeSent
            // 
            this.lblLasttimeSent.AutoSize = true;
            this.lblLasttimeSent.Location = new System.Drawing.Point(192, 33);
            this.lblLasttimeSent.Name = "lblLasttimeSent";
            this.lblLasttimeSent.Size = new System.Drawing.Size(36, 13);
            this.lblLasttimeSent.TabIndex = 8;
            this.lblLasttimeSent.Text = "Never";
            // 
            // lblLastSentTime
            // 
            this.lblLastSentTime.AutoSize = true;
            this.lblLastSentTime.Location = new System.Drawing.Point(6, 33);
            this.lblLastSentTime.Name = "lblLastSentTime";
            this.lblLastSentTime.Size = new System.Drawing.Size(115, 13);
            this.lblLastSentTime.TabIndex = 8;
            this.lblLastSentTime.Text = "Last Time Packet Sent";
            // 
            // lblTotalPktsSent
            // 
            this.lblTotalPktsSent.AutoSize = true;
            this.lblTotalPktsSent.Location = new System.Drawing.Point(6, 117);
            this.lblTotalPktsSent.Name = "lblTotalPktsSent";
            this.lblTotalPktsSent.Size = new System.Drawing.Size(174, 13);
            this.lblTotalPktsSent.TabIndex = 8;
            this.lblTotalPktsSent.Text = "Total Number of Received Packets";
            // 
            // lblTotalPktsRecevied
            // 
            this.lblTotalPktsRecevied.AutoSize = true;
            this.lblTotalPktsRecevied.Location = new System.Drawing.Point(6, 90);
            this.lblTotalPktsRecevied.Name = "lblTotalPktsRecevied";
            this.lblTotalPktsRecevied.Size = new System.Drawing.Size(150, 13);
            this.lblTotalPktsRecevied.TabIndex = 8;
            this.lblTotalPktsRecevied.Text = "Total Number of Sent Packets";
            // 
            // gbDeviceManagment
            // 
            this.gbDeviceManagment.Controls.Add(this.btnStopInterface);
            this.gbDeviceManagment.Controls.Add(this.btnStartInterface);
            this.gbDeviceManagment.Controls.Add(this.label1);
            this.gbDeviceManagment.Controls.Add(this.pbStatus);
            this.gbDeviceManagment.Location = new System.Drawing.Point(383, 5);
            this.gbDeviceManagment.Name = "gbDeviceManagment";
            this.gbDeviceManagment.Size = new System.Drawing.Size(104, 151);
            this.gbDeviceManagment.TabIndex = 9;
            this.gbDeviceManagment.TabStop = false;
            this.gbDeviceManagment.Text = "Management";
            // 
            // btnStopInterface
            // 
            this.btnStopInterface.Location = new System.Drawing.Point(16, 90);
            this.btnStopInterface.Name = "btnStopInterface";
            this.btnStopInterface.Size = new System.Drawing.Size(75, 23);
            this.btnStopInterface.TabIndex = 2;
            this.btnStopInterface.Text = "Stop";
            this.btnStopInterface.UseVisualStyleBackColor = true;
            this.btnStopInterface.Click += new System.EventHandler(this.btnStopInterface_Click);
            // 
            // btnStartInterface
            // 
            this.btnStartInterface.Location = new System.Drawing.Point(16, 60);
            this.btnStartInterface.Name = "btnStartInterface";
            this.btnStartInterface.Size = new System.Drawing.Size(75, 23);
            this.btnStartInterface.TabIndex = 2;
            this.btnStartInterface.Text = "Start";
            this.btnStartInterface.UseVisualStyleBackColor = true;
            this.btnStartInterface.Click += new System.EventHandler(this.btnStartInterface_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(28, 25);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(53, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Unknown";
            // 
            // pbStatus
            // 
            this.pbStatus.Location = new System.Drawing.Point(6, 23);
            this.pbStatus.Name = "pbStatus";
            this.pbStatus.Size = new System.Drawing.Size(16, 16);
            this.pbStatus.TabIndex = 0;
            this.pbStatus.TabStop = false;
            // 
            // imageList1
            // 
            this.imageList1.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList1.ImageStream")));
            this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
            this.imageList1.Images.SetKeyName(0, "device_running");
            this.imageList1.Images.SetKeyName(1, "device_stopped");
            this.imageList1.Images.SetKeyName(2, "device_unknown");
            // 
            // EIBInterfaceControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.gbDeviceManagment);
            this.Controls.Add(this.gbDeviceStats);
            this.Controls.Add(this.gbDeviceMode);
            this.Name = "EIBInterfaceControl";
            this.Size = new System.Drawing.Size(500, 311);
            this.Load += new System.EventHandler(this.EIBInterfaceControl_Load);
            this.gbDeviceMode.ResumeLayout(false);
            this.gbDeviceMode.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.EibPortNUD)).EndInit();
            this.gbDeviceStats.ResumeLayout(false);
            this.gbDeviceStats.PerformLayout();
            this.gbDeviceManagment.ResumeLayout(false);
            this.gbDeviceManagment.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pbStatus)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox gbDeviceMode;
        private System.Windows.Forms.RadioButton rbModeTunneling;
        private System.Windows.Forms.RadioButton rbModeRouting;
        private System.Windows.Forms.CheckBox cbAutoDetect;
        private System.Windows.Forms.Label lblEibPort;
        private System.Windows.Forms.NumericUpDown EibPortNUD;
        private System.Windows.Forms.Label lblDeviceAddress;
        private IPAddressControlLib.IPAddressControl tbDeviceAddress;
        private System.Windows.Forms.GroupBox gbDeviceStats;
        private System.Windows.Forms.Label lblLastSentTime;
        private System.Windows.Forms.Label lblTotalPktsSent;
        private System.Windows.Forms.Label lblTotalPktsRecevied;
        private System.Windows.Forms.Label lblLastReceivedTime;
        private System.Windows.Forms.Label lblLasttimeSent;
        private System.Windows.Forms.Label lblLasttimeReceived;
        private System.Windows.Forms.Label lblTotalSent;
        private System.Windows.Forms.Label lblTotalReceived;
        private System.Windows.Forms.GroupBox gbDeviceManagment;
        private System.Windows.Forms.PictureBox pbStatus;
        private System.Windows.Forms.ImageList imageList1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btnStopInterface;
        private System.Windows.Forms.Button btnStartInterface;
    }
}
