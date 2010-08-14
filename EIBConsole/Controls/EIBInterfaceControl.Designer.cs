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
            this.lblStatusValue = new System.Windows.Forms.Label();
            this.pbStatus = new System.Windows.Forms.PictureBox();
            this.imageList1 = new System.Windows.Forms.ImageList(this.components);
            this.gbDevInfo = new System.Windows.Forms.GroupBox();
            this.lblDeviceName = new System.Windows.Forms.Label();
            this.lblDeviceNameValue = new System.Windows.Forms.Label();
            this.lblMacAddress = new System.Windows.Forms.Label();
            this.lblMacAddressValue = new System.Windows.Forms.Label();
            this.lblSerialNum = new System.Windows.Forms.Label();
            this.lblSerialNumValue = new System.Windows.Forms.Label();
            this.lblMcastAddress = new System.Windows.Forms.Label();
            this.lblMcastAddressValue = new System.Windows.Forms.Label();
            this.lblPhyAddress = new System.Windows.Forms.Label();
            this.lblPhyAddressValue = new System.Windows.Forms.Label();
            this.lblSupportedServices = new System.Windows.Forms.Label();
            this.cbServiceCore = new System.Windows.Forms.CheckBox();
            this.cbServiceDevMngmt = new System.Windows.Forms.CheckBox();
            this.cbServiceTunneling = new System.Windows.Forms.CheckBox();
            this.cbServiceRouting = new System.Windows.Forms.CheckBox();
            this.cbServiceObjSrv = new System.Windows.Forms.CheckBox();
            this.cbServiceRemLog = new System.Windows.Forms.CheckBox();
            this.cbServiceRemConf = new System.Windows.Forms.CheckBox();
            this.gbDeviceMode.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.EibPortNUD)).BeginInit();
            this.gbDeviceStats.SuspendLayout();
            this.gbDeviceManagment.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pbStatus)).BeginInit();
            this.gbDevInfo.SuspendLayout();
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
            this.gbDeviceMode.Location = new System.Drawing.Point(7, 155);
            this.gbDeviceMode.Name = "gbDeviceMode";
            this.gbDeviceMode.Size = new System.Drawing.Size(441, 127);
            this.gbDeviceMode.TabIndex = 0;
            this.gbDeviceMode.TabStop = false;
            this.gbDeviceMode.Text = "KNX Bus Connection Properties";
            // 
            // EibPortNUD
            // 
            this.EibPortNUD.Location = new System.Drawing.Point(252, 52);
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
            this.lblEibPort.Location = new System.Drawing.Point(188, 56);
            this.lblEibPort.Name = "lblEibPort";
            this.lblEibPort.Size = new System.Drawing.Size(26, 13);
            this.lblEibPort.TabIndex = 8;
            this.lblEibPort.Text = "Port";
            // 
            // rbModeTunneling
            // 
            this.rbModeTunneling.AutoSize = true;
            this.rbModeTunneling.Location = new System.Drawing.Point(12, 52);
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
            this.rbModeRouting.Location = new System.Drawing.Point(12, 29);
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
            this.tbDeviceAddress.Location = new System.Drawing.Point(252, 28);
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
            this.lblDeviceAddress.Location = new System.Drawing.Point(188, 33);
            this.lblDeviceAddress.Name = "lblDeviceAddress";
            this.lblDeviceAddress.Size = new System.Drawing.Size(45, 13);
            this.lblDeviceAddress.TabIndex = 7;
            this.lblDeviceAddress.Text = "Address";
            // 
            // cbAutoDetect
            // 
            this.cbAutoDetect.AutoSize = true;
            this.cbAutoDetect.Location = new System.Drawing.Point(12, 84);
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
            this.gbDeviceStats.Location = new System.Drawing.Point(7, 5);
            this.gbDeviceStats.Name = "gbDeviceStats";
            this.gbDeviceStats.Size = new System.Drawing.Size(331, 144);
            this.gbDeviceStats.TabIndex = 2;
            this.gbDeviceStats.TabStop = false;
            this.gbDeviceStats.Text = "Device Statistics";
            // 
            // lblLastReceivedTime
            // 
            this.lblLastReceivedTime.AutoSize = true;
            this.lblLastReceivedTime.Location = new System.Drawing.Point(6, 53);
            this.lblLastReceivedTime.Name = "lblLastReceivedTime";
            this.lblLastReceivedTime.Size = new System.Drawing.Size(139, 13);
            this.lblLastReceivedTime.TabIndex = 8;
            this.lblLastReceivedTime.Text = "Last Time Packet Received";
            // 
            // lblTotalReceived
            // 
            this.lblTotalReceived.AutoSize = true;
            this.lblTotalReceived.Location = new System.Drawing.Point(182, 93);
            this.lblTotalReceived.Name = "lblTotalReceived";
            this.lblTotalReceived.Size = new System.Drawing.Size(13, 13);
            this.lblTotalReceived.TabIndex = 8;
            this.lblTotalReceived.Text = "0";
            // 
            // lblTotalSent
            // 
            this.lblTotalSent.AutoSize = true;
            this.lblTotalSent.Location = new System.Drawing.Point(182, 73);
            this.lblTotalSent.Name = "lblTotalSent";
            this.lblTotalSent.Size = new System.Drawing.Size(13, 13);
            this.lblTotalSent.TabIndex = 8;
            this.lblTotalSent.Text = "0";
            // 
            // lblLasttimeReceived
            // 
            this.lblLasttimeReceived.AutoSize = true;
            this.lblLasttimeReceived.Location = new System.Drawing.Point(182, 53);
            this.lblLasttimeReceived.Name = "lblLasttimeReceived";
            this.lblLasttimeReceived.Size = new System.Drawing.Size(36, 13);
            this.lblLasttimeReceived.TabIndex = 8;
            this.lblLasttimeReceived.Text = "Never";
            // 
            // lblLasttimeSent
            // 
            this.lblLasttimeSent.AutoSize = true;
            this.lblLasttimeSent.Location = new System.Drawing.Point(182, 33);
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
            this.lblTotalPktsSent.Location = new System.Drawing.Point(6, 93);
            this.lblTotalPktsSent.Name = "lblTotalPktsSent";
            this.lblTotalPktsSent.Size = new System.Drawing.Size(122, 13);
            this.lblTotalPktsSent.TabIndex = 8;
            this.lblTotalPktsSent.Text = "Total Received Packets";
            // 
            // lblTotalPktsRecevied
            // 
            this.lblTotalPktsRecevied.AutoSize = true;
            this.lblTotalPktsRecevied.Location = new System.Drawing.Point(6, 73);
            this.lblTotalPktsRecevied.Name = "lblTotalPktsRecevied";
            this.lblTotalPktsRecevied.Size = new System.Drawing.Size(98, 13);
            this.lblTotalPktsRecevied.TabIndex = 8;
            this.lblTotalPktsRecevied.Text = "Total Sent Packets";
            // 
            // gbDeviceManagment
            // 
            this.gbDeviceManagment.Controls.Add(this.btnStopInterface);
            this.gbDeviceManagment.Controls.Add(this.btnStartInterface);
            this.gbDeviceManagment.Controls.Add(this.lblStatusValue);
            this.gbDeviceManagment.Controls.Add(this.pbStatus);
            this.gbDeviceManagment.Location = new System.Drawing.Point(344, 5);
            this.gbDeviceManagment.Name = "gbDeviceManagment";
            this.gbDeviceManagment.Size = new System.Drawing.Size(104, 144);
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
            // lblStatusValue
            // 
            this.lblStatusValue.AutoSize = true;
            this.lblStatusValue.Location = new System.Drawing.Point(28, 25);
            this.lblStatusValue.Name = "lblStatusValue";
            this.lblStatusValue.Size = new System.Drawing.Size(53, 13);
            this.lblStatusValue.TabIndex = 1;
            this.lblStatusValue.Text = "Unknown";
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
            // gbDevInfo
            // 
            this.gbDevInfo.Controls.Add(this.cbServiceRemConf);
            this.gbDevInfo.Controls.Add(this.cbServiceRemLog);
            this.gbDevInfo.Controls.Add(this.cbServiceObjSrv);
            this.gbDevInfo.Controls.Add(this.cbServiceRouting);
            this.gbDevInfo.Controls.Add(this.cbServiceTunneling);
            this.gbDevInfo.Controls.Add(this.cbServiceDevMngmt);
            this.gbDevInfo.Controls.Add(this.cbServiceCore);
            this.gbDevInfo.Controls.Add(this.lblPhyAddressValue);
            this.gbDevInfo.Controls.Add(this.lblMcastAddressValue);
            this.gbDevInfo.Controls.Add(this.lblSerialNumValue);
            this.gbDevInfo.Controls.Add(this.lblMacAddressValue);
            this.gbDevInfo.Controls.Add(this.lblDeviceNameValue);
            this.gbDevInfo.Controls.Add(this.lblSupportedServices);
            this.gbDevInfo.Controls.Add(this.lblPhyAddress);
            this.gbDevInfo.Controls.Add(this.lblMcastAddress);
            this.gbDevInfo.Controls.Add(this.lblSerialNum);
            this.gbDevInfo.Controls.Add(this.lblMacAddress);
            this.gbDevInfo.Controls.Add(this.lblDeviceName);
            this.gbDevInfo.Location = new System.Drawing.Point(454, 5);
            this.gbDevInfo.Name = "gbDevInfo";
            this.gbDevInfo.Size = new System.Drawing.Size(255, 277);
            this.gbDevInfo.TabIndex = 10;
            this.gbDevInfo.TabStop = false;
            this.gbDevInfo.Text = "Device Info";
            // 
            // lblDeviceName
            // 
            this.lblDeviceName.AutoSize = true;
            this.lblDeviceName.Location = new System.Drawing.Point(6, 33);
            this.lblDeviceName.Name = "lblDeviceName";
            this.lblDeviceName.Size = new System.Drawing.Size(38, 13);
            this.lblDeviceName.TabIndex = 0;
            this.lblDeviceName.Text = "Name:";
            // 
            // lblDeviceNameValue
            // 
            this.lblDeviceNameValue.AutoSize = true;
            this.lblDeviceNameValue.Location = new System.Drawing.Point(121, 33);
            this.lblDeviceNameValue.Name = "lblDeviceNameValue";
            this.lblDeviceNameValue.Size = new System.Drawing.Size(27, 13);
            this.lblDeviceNameValue.TabIndex = 0;
            this.lblDeviceNameValue.Text = "N/A";
            // 
            // lblMacAddress
            // 
            this.lblMacAddress.AutoSize = true;
            this.lblMacAddress.Location = new System.Drawing.Point(6, 53);
            this.lblMacAddress.Name = "lblMacAddress";
            this.lblMacAddress.Size = new System.Drawing.Size(74, 13);
            this.lblMacAddress.TabIndex = 0;
            this.lblMacAddress.Text = "MAC Address:";
            // 
            // lblMacAddressValue
            // 
            this.lblMacAddressValue.AutoSize = true;
            this.lblMacAddressValue.Location = new System.Drawing.Point(121, 53);
            this.lblMacAddressValue.Name = "lblMacAddressValue";
            this.lblMacAddressValue.Size = new System.Drawing.Size(27, 13);
            this.lblMacAddressValue.TabIndex = 0;
            this.lblMacAddressValue.Text = "N/A";
            // 
            // lblSerialNum
            // 
            this.lblSerialNum.AutoSize = true;
            this.lblSerialNum.Location = new System.Drawing.Point(6, 73);
            this.lblSerialNum.Name = "lblSerialNum";
            this.lblSerialNum.Size = new System.Drawing.Size(76, 13);
            this.lblSerialNum.TabIndex = 0;
            this.lblSerialNum.Text = "Serial Number:";
            // 
            // lblSerialNumValue
            // 
            this.lblSerialNumValue.AutoSize = true;
            this.lblSerialNumValue.Location = new System.Drawing.Point(121, 73);
            this.lblSerialNumValue.Name = "lblSerialNumValue";
            this.lblSerialNumValue.Size = new System.Drawing.Size(27, 13);
            this.lblSerialNumValue.TabIndex = 0;
            this.lblSerialNumValue.Text = "N/A";
            // 
            // lblMcastAddress
            // 
            this.lblMcastAddress.AutoSize = true;
            this.lblMcastAddress.Location = new System.Drawing.Point(6, 93);
            this.lblMcastAddress.Name = "lblMcastAddress";
            this.lblMcastAddress.Size = new System.Drawing.Size(93, 13);
            this.lblMcastAddress.TabIndex = 0;
            this.lblMcastAddress.Text = "Multicast Address:";
            // 
            // lblMcastAddressValue
            // 
            this.lblMcastAddressValue.AutoSize = true;
            this.lblMcastAddressValue.Location = new System.Drawing.Point(121, 93);
            this.lblMcastAddressValue.Name = "lblMcastAddressValue";
            this.lblMcastAddressValue.Size = new System.Drawing.Size(27, 13);
            this.lblMcastAddressValue.TabIndex = 0;
            this.lblMcastAddressValue.Text = "N/A";
            // 
            // lblPhyAddress
            // 
            this.lblPhyAddress.AutoSize = true;
            this.lblPhyAddress.Location = new System.Drawing.Point(6, 113);
            this.lblPhyAddress.Name = "lblPhyAddress";
            this.lblPhyAddress.Size = new System.Drawing.Size(90, 13);
            this.lblPhyAddress.TabIndex = 0;
            this.lblPhyAddress.Text = "Physical Address:";
            // 
            // lblPhyAddressValue
            // 
            this.lblPhyAddressValue.AutoSize = true;
            this.lblPhyAddressValue.Location = new System.Drawing.Point(121, 113);
            this.lblPhyAddressValue.Name = "lblPhyAddressValue";
            this.lblPhyAddressValue.Size = new System.Drawing.Size(27, 13);
            this.lblPhyAddressValue.TabIndex = 0;
            this.lblPhyAddressValue.Text = "N/A";
            // 
            // lblSupportedServices
            // 
            this.lblSupportedServices.AutoSize = true;
            this.lblSupportedServices.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Underline, System.Drawing.GraphicsUnit.Point, ((byte)(177)));
            this.lblSupportedServices.Location = new System.Drawing.Point(6, 161);
            this.lblSupportedServices.Name = "lblSupportedServices";
            this.lblSupportedServices.Size = new System.Drawing.Size(103, 13);
            this.lblSupportedServices.TabIndex = 0;
            this.lblSupportedServices.Text = "Supported Services:";
            // 
            // cbServiceCore
            // 
            this.cbServiceCore.AutoSize = true;
            this.cbServiceCore.Enabled = false;
            this.cbServiceCore.Location = new System.Drawing.Point(8, 185);
            this.cbServiceCore.Name = "cbServiceCore";
            this.cbServiceCore.Size = new System.Drawing.Size(48, 17);
            this.cbServiceCore.TabIndex = 1;
            this.cbServiceCore.Text = "Core";
            this.cbServiceCore.UseVisualStyleBackColor = true;
            // 
            // cbServiceDevMngmt
            // 
            this.cbServiceDevMngmt.AutoSize = true;
            this.cbServiceDevMngmt.Enabled = false;
            this.cbServiceDevMngmt.Location = new System.Drawing.Point(8, 205);
            this.cbServiceDevMngmt.Name = "cbServiceDevMngmt";
            this.cbServiceDevMngmt.Size = new System.Drawing.Size(111, 17);
            this.cbServiceDevMngmt.TabIndex = 1;
            this.cbServiceDevMngmt.Text = "Dev Management";
            this.cbServiceDevMngmt.UseVisualStyleBackColor = true;
            // 
            // cbServiceTunneling
            // 
            this.cbServiceTunneling.AutoSize = true;
            this.cbServiceTunneling.Enabled = false;
            this.cbServiceTunneling.Location = new System.Drawing.Point(8, 225);
            this.cbServiceTunneling.Name = "cbServiceTunneling";
            this.cbServiceTunneling.Size = new System.Drawing.Size(73, 17);
            this.cbServiceTunneling.TabIndex = 1;
            this.cbServiceTunneling.Text = "Tunneling";
            this.cbServiceTunneling.UseVisualStyleBackColor = true;
            // 
            // cbServiceRouting
            // 
            this.cbServiceRouting.AutoSize = true;
            this.cbServiceRouting.Enabled = false;
            this.cbServiceRouting.Location = new System.Drawing.Point(8, 245);
            this.cbServiceRouting.Name = "cbServiceRouting";
            this.cbServiceRouting.Size = new System.Drawing.Size(63, 17);
            this.cbServiceRouting.TabIndex = 1;
            this.cbServiceRouting.Text = "Routing";
            this.cbServiceRouting.UseVisualStyleBackColor = true;
            // 
            // cbServiceObjSrv
            // 
            this.cbServiceObjSrv.AutoSize = true;
            this.cbServiceObjSrv.Enabled = false;
            this.cbServiceObjSrv.Location = new System.Drawing.Point(124, 225);
            this.cbServiceObjSrv.Name = "cbServiceObjSrv";
            this.cbServiceObjSrv.Size = new System.Drawing.Size(91, 17);
            this.cbServiceObjSrv.TabIndex = 1;
            this.cbServiceObjSrv.Text = "Object Server";
            this.cbServiceObjSrv.UseVisualStyleBackColor = true;
            // 
            // cbServiceRemLog
            // 
            this.cbServiceRemLog.AutoSize = true;
            this.cbServiceRemLog.Enabled = false;
            this.cbServiceRemLog.Location = new System.Drawing.Point(124, 185);
            this.cbServiceRemLog.Name = "cbServiceRemLog";
            this.cbServiceRemLog.Size = new System.Drawing.Size(84, 17);
            this.cbServiceRemLog.TabIndex = 1;
            this.cbServiceRemLog.Text = "Remote Log";
            this.cbServiceRemLog.UseVisualStyleBackColor = true;
            // 
            // cbServiceRemConf
            // 
            this.cbServiceRemConf.AutoSize = true;
            this.cbServiceRemConf.Enabled = false;
            this.cbServiceRemConf.Location = new System.Drawing.Point(124, 205);
            this.cbServiceRemConf.Name = "cbServiceRemConf";
            this.cbServiceRemConf.Size = new System.Drawing.Size(128, 17);
            this.cbServiceRemConf.TabIndex = 1;
            this.cbServiceRemConf.Text = "Remote Configuration";
            this.cbServiceRemConf.UseVisualStyleBackColor = true;
            // 
            // EIBInterfaceControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.gbDevInfo);
            this.Controls.Add(this.gbDeviceManagment);
            this.Controls.Add(this.gbDeviceStats);
            this.Controls.Add(this.gbDeviceMode);
            this.Name = "EIBInterfaceControl";
            this.Size = new System.Drawing.Size(779, 311);
            this.Load += new System.EventHandler(this.EIBInterfaceControl_Load);
            this.gbDeviceMode.ResumeLayout(false);
            this.gbDeviceMode.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.EibPortNUD)).EndInit();
            this.gbDeviceStats.ResumeLayout(false);
            this.gbDeviceStats.PerformLayout();
            this.gbDeviceManagment.ResumeLayout(false);
            this.gbDeviceManagment.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pbStatus)).EndInit();
            this.gbDevInfo.ResumeLayout(false);
            this.gbDevInfo.PerformLayout();
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
        private System.Windows.Forms.Label lblStatusValue;
        private System.Windows.Forms.Button btnStopInterface;
        private System.Windows.Forms.Button btnStartInterface;
        private System.Windows.Forms.GroupBox gbDevInfo;
        private System.Windows.Forms.Label lblSerialNumValue;
        private System.Windows.Forms.Label lblMacAddressValue;
        private System.Windows.Forms.Label lblDeviceNameValue;
        private System.Windows.Forms.Label lblSerialNum;
        private System.Windows.Forms.Label lblMacAddress;
        private System.Windows.Forms.Label lblDeviceName;
        private System.Windows.Forms.CheckBox cbServiceRemConf;
        private System.Windows.Forms.CheckBox cbServiceRemLog;
        private System.Windows.Forms.CheckBox cbServiceObjSrv;
        private System.Windows.Forms.CheckBox cbServiceRouting;
        private System.Windows.Forms.CheckBox cbServiceTunneling;
        private System.Windows.Forms.CheckBox cbServiceDevMngmt;
        private System.Windows.Forms.CheckBox cbServiceCore;
        private System.Windows.Forms.Label lblPhyAddressValue;
        private System.Windows.Forms.Label lblMcastAddressValue;
        private System.Windows.Forms.Label lblSupportedServices;
        private System.Windows.Forms.Label lblPhyAddress;
        private System.Windows.Forms.Label lblMcastAddress;
    }
}
