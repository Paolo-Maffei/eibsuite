namespace EIBMobile.Controls
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(EIBInterfaceControl));
            this.lblInterfaceStatus = new System.Windows.Forms.Label();
            this.pbDeviceStatus = new System.Windows.Forms.PictureBox();
            this.btnStart = new System.Windows.Forms.Button();
            this.btnStop = new System.Windows.Forms.Button();
            this.lblTotalSent = new System.Windows.Forms.Label();
            this.lblTotalSentVal = new System.Windows.Forms.Label();
            this.lblTotalRecvdVal = new System.Windows.Forms.Label();
            this.lblTotalRecvd = new System.Windows.Forms.Label();
            this.lblDeviceAddress = new System.Windows.Forms.Label();
            this.lblDeviceMode = new System.Windows.Forms.Label();
            this.rbModeTunneling = new System.Windows.Forms.RadioButton();
            this.rbModeRouting = new System.Windows.Forms.RadioButton();
            this.cbAutoDetect = new System.Windows.Forms.CheckBox();
            this.ControlimageList = new System.Windows.Forms.ImageList();
            this.ipDeviceAddress = new EIBMobile.Controls.IPAddressControl();
            this.SuspendLayout();
            // 
            // lblInterfaceStatus
            // 
            this.lblInterfaceStatus.Location = new System.Drawing.Point(26, 5);
            this.lblInterfaceStatus.Name = "lblInterfaceStatus";
            this.lblInterfaceStatus.Size = new System.Drawing.Size(101, 15);
            this.lblInterfaceStatus.Text = "Unknown Status";
            // 
            // pbDeviceStatus
            // 
            this.pbDeviceStatus.Location = new System.Drawing.Point(3, 3);
            this.pbDeviceStatus.Name = "pbDeviceStatus";
            this.pbDeviceStatus.Size = new System.Drawing.Size(17, 17);
            // 
            // btnStart
            // 
            this.btnStart.Location = new System.Drawing.Point(3, 26);
            this.btnStart.Name = "btnStart";
            this.btnStart.Size = new System.Drawing.Size(72, 20);
            this.btnStart.TabIndex = 2;
            this.btnStart.Text = "Start";
            this.btnStart.Click += new System.EventHandler(this.btnStart_Click);
            // 
            // btnStop
            // 
            this.btnStop.Location = new System.Drawing.Point(104, 26);
            this.btnStop.Name = "btnStop";
            this.btnStop.Size = new System.Drawing.Size(72, 20);
            this.btnStop.TabIndex = 3;
            this.btnStop.Text = "Stop";
            this.btnStop.Click += new System.EventHandler(this.btnStop_Click);
            // 
            // lblTotalSent
            // 
            this.lblTotalSent.Location = new System.Drawing.Point(3, 56);
            this.lblTotalSent.Name = "lblTotalSent";
            this.lblTotalSent.Size = new System.Drawing.Size(143, 20);
            this.lblTotalSent.Text = "Total Packets Sent :";
            // 
            // lblTotalSentVal
            // 
            this.lblTotalSentVal.Location = new System.Drawing.Point(172, 56);
            this.lblTotalSentVal.Name = "lblTotalSentVal";
            this.lblTotalSentVal.Size = new System.Drawing.Size(43, 20);
            this.lblTotalSentVal.Text = "0";
            // 
            // lblTotalRecvdVal
            // 
            this.lblTotalRecvdVal.Location = new System.Drawing.Point(172, 76);
            this.lblTotalRecvdVal.Name = "lblTotalRecvdVal";
            this.lblTotalRecvdVal.Size = new System.Drawing.Size(43, 20);
            this.lblTotalRecvdVal.Text = "0";
            // 
            // lblTotalRecvd
            // 
            this.lblTotalRecvd.Location = new System.Drawing.Point(3, 76);
            this.lblTotalRecvd.Name = "lblTotalRecvd";
            this.lblTotalRecvd.Size = new System.Drawing.Size(143, 20);
            this.lblTotalRecvd.Text = "Total Packets Recevied :";
            // 
            // lblDeviceAddress
            // 
            this.lblDeviceAddress.Location = new System.Drawing.Point(3, 96);
            this.lblDeviceAddress.Name = "lblDeviceAddress";
            this.lblDeviceAddress.Size = new System.Drawing.Size(111, 16);
            this.lblDeviceAddress.Text = "Device IP Address";
            // 
            // lblDeviceMode
            // 
            this.lblDeviceMode.Location = new System.Drawing.Point(3, 142);
            this.lblDeviceMode.Name = "lblDeviceMode";
            this.lblDeviceMode.Size = new System.Drawing.Size(111, 20);
            this.lblDeviceMode.Text = "Device Mode";
            // 
            // rbModeTunneling
            // 
            this.rbModeTunneling.Location = new System.Drawing.Point(3, 164);
            this.rbModeTunneling.Name = "rbModeTunneling";
            this.rbModeTunneling.Size = new System.Drawing.Size(124, 20);
            this.rbModeTunneling.TabIndex = 13;
            this.rbModeTunneling.Text = "Mode Tunneling";
            // 
            // rbModeRouting
            // 
            this.rbModeRouting.Location = new System.Drawing.Point(3, 184);
            this.rbModeRouting.Name = "rbModeRouting";
            this.rbModeRouting.Size = new System.Drawing.Size(100, 20);
            this.rbModeRouting.TabIndex = 13;
            this.rbModeRouting.Text = "Mode Routing";
            // 
            // cbAutoDetect
            // 
            this.cbAutoDetect.Location = new System.Drawing.Point(120, 142);
            this.cbAutoDetect.Name = "cbAutoDetect";
            this.cbAutoDetect.Size = new System.Drawing.Size(92, 20);
            this.cbAutoDetect.TabIndex = 14;
            this.cbAutoDetect.Text = "Auto Detect";
            this.ControlimageList.Images.Clear();
            this.ControlimageList.Images.Add(((System.Drawing.Icon)(resources.GetObject("resource"))));
            this.ControlimageList.Images.Add(((System.Drawing.Icon)(resources.GetObject("resource1"))));
            this.ControlimageList.Images.Add(((System.Drawing.Icon)(resources.GetObject("resource2"))));
            // 
            // ipDeviceAddress
            // 
            this.ipDeviceAddress.BackColor = System.Drawing.SystemColors.ControlLight;
            this.ipDeviceAddress.Location = new System.Drawing.Point(3, 115);
            this.ipDeviceAddress.Name = "ipDeviceAddress";
            this.ipDeviceAddress.Size = new System.Drawing.Size(151, 20);
            this.ipDeviceAddress.TabIndex = 10;
            // 
            // EIBInterfaceControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.Controls.Add(this.cbAutoDetect);
            this.Controls.Add(this.rbModeRouting);
            this.Controls.Add(this.rbModeTunneling);
            this.Controls.Add(this.lblDeviceMode);
            this.Controls.Add(this.lblDeviceAddress);
            this.Controls.Add(this.ipDeviceAddress);
            this.Controls.Add(this.lblTotalRecvdVal);
            this.Controls.Add(this.lblTotalRecvd);
            this.Controls.Add(this.lblTotalSentVal);
            this.Controls.Add(this.lblTotalSent);
            this.Controls.Add(this.btnStop);
            this.Controls.Add(this.btnStart);
            this.Controls.Add(this.pbDeviceStatus);
            this.Controls.Add(this.lblInterfaceStatus);
            this.Name = "EIBInterfaceControl";
            this.Size = new System.Drawing.Size(215, 210);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label lblInterfaceStatus;
        private System.Windows.Forms.PictureBox pbDeviceStatus;
        private System.Windows.Forms.Button btnStart;
        private System.Windows.Forms.Button btnStop;
        private System.Windows.Forms.Label lblTotalSent;
        private System.Windows.Forms.Label lblTotalSentVal;
        private System.Windows.Forms.Label lblTotalRecvdVal;
        private System.Windows.Forms.Label lblTotalRecvd;
        private IPAddressControl ipDeviceAddress;
        private System.Windows.Forms.Label lblDeviceAddress;
        private System.Windows.Forms.Label lblDeviceMode;
        private System.Windows.Forms.RadioButton rbModeTunneling;
        private System.Windows.Forms.RadioButton rbModeRouting;
        private System.Windows.Forms.CheckBox cbAutoDetect;
        private System.Windows.Forms.ImageList ControlimageList;
    }
}
