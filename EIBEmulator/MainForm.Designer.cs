namespace EIBEmulator
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
            this.ListenerThread = new System.ComponentModel.BackgroundWorker();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.btnClearLog = new System.Windows.Forms.Button();
            this.btnDeviceOn = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.numericUpDown1 = new System.Windows.Forms.NumericUpDown();
            this.ipAddressControl1 = new IPAddressControlLib.IPAddressControl();
            this.btnDeviceOff = new System.Windows.Forms.Button();
            this.gbPktGenerator = new System.Windows.Forms.GroupBox();
            this.nudNumPkts = new System.Windows.Forms.NumericUpDown();
            this.checkBox1 = new System.Windows.Forms.CheckBox();
            this.cbNeedAck = new System.Windows.Forms.CheckBox();
            this.label5 = new System.Windows.Forms.Label();
            this.lblHexPrefix = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.lblSrcAddr = new System.Windows.Forms.Label();
            this.lblDstAddr = new System.Windows.Forms.Label();
            this.tbValue = new System.Windows.Forms.TextBox();
            this.tbSrcAddress = new System.Windows.Forms.TextBox();
            this.tbDestAddress = new System.Windows.Forms.TextBox();
            this.btnSendPkt = new System.Windows.Forms.Button();
            this.gbDeviceMode = new System.Windows.Forms.GroupBox();
            this.rbModeTunneling = new System.Windows.Forms.RadioButton();
            this.rbModeRouting = new System.Windows.Forms.RadioButton();
            this.label6 = new System.Windows.Forms.Label();
            this.netifsComboBox = new System.Windows.Forms.ComboBox();
            this.button1 = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown1)).BeginInit();
            this.gbPktGenerator.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudNumPkts)).BeginInit();
            this.gbDeviceMode.SuspendLayout();
            this.SuspendLayout();
            // 
            // ListenerThread
            // 
            this.ListenerThread.WorkerReportsProgress = true;
            this.ListenerThread.WorkerSupportsCancellation = true;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.textBox1);
            this.groupBox1.Controls.Add(this.btnClearLog);
            this.groupBox1.Location = new System.Drawing.Point(270, 31);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(542, 373);
            this.groupBox1.TabIndex = 6;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Log";
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(14, 19);
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBox1.Size = new System.Drawing.Size(458, 348);
            this.textBox1.TabIndex = 0;
            // 
            // btnClearLog
            // 
            this.btnClearLog.Location = new System.Drawing.Point(478, 19);
            this.btnClearLog.Name = "btnClearLog";
            this.btnClearLog.Size = new System.Drawing.Size(55, 23);
            this.btnClearLog.TabIndex = 5;
            this.btnClearLog.Text = "Clear";
            this.btnClearLog.UseVisualStyleBackColor = true;
            this.btnClearLog.Click += new System.EventHandler(this.btnClearLog_Click);
            // 
            // btnDeviceOn
            // 
            this.btnDeviceOn.Location = new System.Drawing.Point(10, 113);
            this.btnDeviceOn.Name = "btnDeviceOn";
            this.btnDeviceOn.Size = new System.Drawing.Size(98, 23);
            this.btnDeviceOn.TabIndex = 5;
            this.btnDeviceOn.Text = "Turn Device ON";
            this.btnDeviceOn.UseVisualStyleBackColor = true;
            this.btnDeviceOn.Click += new System.EventHandler(this.btnDeviceOn_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(10, 72);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(63, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "Device Port";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(7, 40);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(95, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Device IP Address";
            // 
            // numericUpDown1
            // 
            this.numericUpDown1.Location = new System.Drawing.Point(129, 69);
            this.numericUpDown1.Maximum = new decimal(new int[] {
            65000,
            0,
            0,
            0});
            this.numericUpDown1.Name = "numericUpDown1";
            this.numericUpDown1.Size = new System.Drawing.Size(140, 20);
            this.numericUpDown1.TabIndex = 8;
            this.numericUpDown1.Value = new decimal(new int[] {
            3671,
            0,
            0,
            0});
            // 
            // ipAddressControl1
            // 
            this.ipAddressControl1.AllowInternalTab = false;
            this.ipAddressControl1.AutoHeight = true;
            this.ipAddressControl1.BackColor = System.Drawing.SystemColors.Window;
            this.ipAddressControl1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.ipAddressControl1.Cursor = System.Windows.Forms.Cursors.IBeam;
            this.ipAddressControl1.Location = new System.Drawing.Point(124, 37);
            this.ipAddressControl1.MinimumSize = new System.Drawing.Size(87, 20);
            this.ipAddressControl1.Name = "ipAddressControl1";
            this.ipAddressControl1.ReadOnly = true;
            this.ipAddressControl1.Size = new System.Drawing.Size(140, 20);
            this.ipAddressControl1.TabIndex = 4;
            this.ipAddressControl1.Text = "0.0.0.0";
            // 
            // btnDeviceOff
            // 
            this.btnDeviceOff.Location = new System.Drawing.Point(10, 142);
            this.btnDeviceOff.Name = "btnDeviceOff";
            this.btnDeviceOff.Size = new System.Drawing.Size(98, 23);
            this.btnDeviceOff.TabIndex = 5;
            this.btnDeviceOff.Text = "Turn Device OFF";
            this.btnDeviceOff.UseVisualStyleBackColor = true;
            this.btnDeviceOff.Click += new System.EventHandler(this.btnDeviceOff_Click);
            // 
            // gbPktGenerator
            // 
            this.gbPktGenerator.Controls.Add(this.nudNumPkts);
            this.gbPktGenerator.Controls.Add(this.checkBox1);
            this.gbPktGenerator.Controls.Add(this.cbNeedAck);
            this.gbPktGenerator.Controls.Add(this.label5);
            this.gbPktGenerator.Controls.Add(this.lblHexPrefix);
            this.gbPktGenerator.Controls.Add(this.label4);
            this.gbPktGenerator.Controls.Add(this.lblSrcAddr);
            this.gbPktGenerator.Controls.Add(this.lblDstAddr);
            this.gbPktGenerator.Controls.Add(this.tbValue);
            this.gbPktGenerator.Controls.Add(this.tbSrcAddress);
            this.gbPktGenerator.Controls.Add(this.tbDestAddress);
            this.gbPktGenerator.Controls.Add(this.btnSendPkt);
            this.gbPktGenerator.Location = new System.Drawing.Point(15, 179);
            this.gbPktGenerator.Name = "gbPktGenerator";
            this.gbPktGenerator.Size = new System.Drawing.Size(254, 219);
            this.gbPktGenerator.TabIndex = 9;
            this.gbPktGenerator.TabStop = false;
            this.gbPktGenerator.Text = "Packet Generator";
            // 
            // nudNumPkts
            // 
            this.nudNumPkts.Location = new System.Drawing.Point(128, 130);
            this.nudNumPkts.Name = "nudNumPkts";
            this.nudNumPkts.Size = new System.Drawing.Size(59, 20);
            this.nudNumPkts.TabIndex = 4;
            this.nudNumPkts.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // checkBox1
            // 
            this.checkBox1.AutoSize = true;
            this.checkBox1.Location = new System.Drawing.Point(11, 28);
            this.checkBox1.Name = "checkBox1";
            this.checkBox1.Size = new System.Drawing.Size(106, 17);
            this.checkBox1.TabIndex = 3;
            this.checkBox1.Text = "Random address";
            this.checkBox1.UseVisualStyleBackColor = true;
            this.checkBox1.CheckedChanged += new System.EventHandler(this.checkBox1_CheckedChanged);
            // 
            // cbNeedAck
            // 
            this.cbNeedAck.AutoSize = true;
            this.cbNeedAck.Checked = true;
            this.cbNeedAck.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbNeedAck.Location = new System.Drawing.Point(126, 28);
            this.cbNeedAck.Name = "cbNeedAck";
            this.cbNeedAck.Size = new System.Drawing.Size(74, 17);
            this.cbNeedAck.TabIndex = 3;
            this.cbNeedAck.Text = "Need Ack";
            this.cbNeedAck.UseVisualStyleBackColor = true;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(8, 132);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(98, 13);
            this.label5.TabIndex = 2;
            this.label5.Text = "Number of Packets";
            // 
            // lblHexPrefix
            // 
            this.lblHexPrefix.AutoSize = true;
            this.lblHexPrefix.Location = new System.Drawing.Point(104, 107);
            this.lblHexPrefix.Name = "lblHexPrefix";
            this.lblHexPrefix.Size = new System.Drawing.Size(21, 13);
            this.lblHexPrefix.TabIndex = 2;
            this.lblHexPrefix.Text = "0 x";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(8, 107);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(34, 13);
            this.label4.TabIndex = 2;
            this.label4.Text = "Value";
            // 
            // lblSrcAddr
            // 
            this.lblSrcAddr.AutoSize = true;
            this.lblSrcAddr.Location = new System.Drawing.Point(8, 55);
            this.lblSrcAddr.Name = "lblSrcAddr";
            this.lblSrcAddr.Size = new System.Drawing.Size(82, 13);
            this.lblSrcAddr.TabIndex = 2;
            this.lblSrcAddr.Text = "Source Address";
            // 
            // lblDstAddr
            // 
            this.lblDstAddr.AutoSize = true;
            this.lblDstAddr.Location = new System.Drawing.Point(8, 81);
            this.lblDstAddr.Name = "lblDstAddr";
            this.lblDstAddr.Size = new System.Drawing.Size(101, 13);
            this.lblDstAddr.TabIndex = 2;
            this.lblDstAddr.Text = "Destination Address";
            // 
            // tbValue
            // 
            this.tbValue.Location = new System.Drawing.Point(128, 104);
            this.tbValue.Name = "tbValue";
            this.tbValue.Size = new System.Drawing.Size(59, 20);
            this.tbValue.TabIndex = 1;
            this.tbValue.Text = "80";
            // 
            // tbSrcAddress
            // 
            this.tbSrcAddress.Location = new System.Drawing.Point(128, 52);
            this.tbSrcAddress.Name = "tbSrcAddress";
            this.tbSrcAddress.Size = new System.Drawing.Size(59, 20);
            this.tbSrcAddress.TabIndex = 1;
            this.tbSrcAddress.Text = "0/0/1";
            // 
            // tbDestAddress
            // 
            this.tbDestAddress.Location = new System.Drawing.Point(128, 78);
            this.tbDestAddress.Name = "tbDestAddress";
            this.tbDestAddress.Size = new System.Drawing.Size(59, 20);
            this.tbDestAddress.TabIndex = 1;
            this.tbDestAddress.Text = "1/1/1";
            // 
            // btnSendPkt
            // 
            this.btnSendPkt.Location = new System.Drawing.Point(11, 156);
            this.btnSendPkt.Name = "btnSendPkt";
            this.btnSendPkt.Size = new System.Drawing.Size(221, 51);
            this.btnSendPkt.TabIndex = 0;
            this.btnSendPkt.Text = "SEND";
            this.btnSendPkt.UseVisualStyleBackColor = true;
            this.btnSendPkt.Click += new System.EventHandler(this.btnSendPkt_Click);
            // 
            // gbDeviceMode
            // 
            this.gbDeviceMode.Controls.Add(this.rbModeTunneling);
            this.gbDeviceMode.Controls.Add(this.rbModeRouting);
            this.gbDeviceMode.Location = new System.Drawing.Point(126, 99);
            this.gbDeviceMode.Name = "gbDeviceMode";
            this.gbDeviceMode.Size = new System.Drawing.Size(143, 74);
            this.gbDeviceMode.TabIndex = 10;
            this.gbDeviceMode.TabStop = false;
            this.gbDeviceMode.Text = "Device Mode";
            // 
            // rbModeTunneling
            // 
            this.rbModeTunneling.AutoSize = true;
            this.rbModeTunneling.Checked = true;
            this.rbModeTunneling.Location = new System.Drawing.Point(17, 43);
            this.rbModeTunneling.Name = "rbModeTunneling";
            this.rbModeTunneling.Size = new System.Drawing.Size(72, 17);
            this.rbModeTunneling.TabIndex = 0;
            this.rbModeTunneling.TabStop = true;
            this.rbModeTunneling.Text = "Tunneling";
            this.rbModeTunneling.UseVisualStyleBackColor = true;
            // 
            // rbModeRouting
            // 
            this.rbModeRouting.AutoSize = true;
            this.rbModeRouting.Location = new System.Drawing.Point(17, 20);
            this.rbModeRouting.Name = "rbModeRouting";
            this.rbModeRouting.Size = new System.Drawing.Size(62, 17);
            this.rbModeRouting.TabIndex = 0;
            this.rbModeRouting.Text = "Routing";
            this.rbModeRouting.UseVisualStyleBackColor = true;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(10, 9);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(95, 13);
            this.label6.TabIndex = 2;
            this.label6.Text = "Network Interfface";
            // 
            // netifsComboBox
            // 
            this.netifsComboBox.FormattingEnabled = true;
            this.netifsComboBox.Location = new System.Drawing.Point(126, 6);
            this.netifsComboBox.Name = "netifsComboBox";
            this.netifsComboBox.Size = new System.Drawing.Size(509, 21);
            this.netifsComboBox.TabIndex = 11;
            this.netifsComboBox.Text = "Please select your network interface...";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(641, 6);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(171, 23);
            this.button1.TabIndex = 5;
            this.button1.Text = "Edit Known Groups";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.btnEditKnownGroups_Click);
            // 
            // MainForm
            // 
            this.AcceptButton = this.btnSendPkt;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(829, 410);
            this.Controls.Add(this.netifsComboBox);
            this.Controls.Add(this.gbDeviceMode);
            this.Controls.Add(this.gbPktGenerator);
            this.Controls.Add(this.numericUpDown1);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.btnDeviceOff);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.btnDeviceOn);
            this.Controls.Add(this.ipAddressControl1);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Name = "MainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "EIB Emulator";
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown1)).EndInit();
            this.gbPktGenerator.ResumeLayout(false);
            this.gbPktGenerator.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudNumPkts)).EndInit();
            this.gbDeviceMode.ResumeLayout(false);
            this.gbDeviceMode.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.ComponentModel.BackgroundWorker ListenerThread;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Button btnDeviceOn;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown numericUpDown1;
        private IPAddressControlLib.IPAddressControl ipAddressControl1;
        private System.Windows.Forms.Button btnDeviceOff;
        private System.Windows.Forms.GroupBox gbPktGenerator;
        private System.Windows.Forms.Button btnSendPkt;
        private System.Windows.Forms.GroupBox gbDeviceMode;
        private System.Windows.Forms.RadioButton rbModeTunneling;
        private System.Windows.Forms.RadioButton rbModeRouting;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label lblDstAddr;
        private System.Windows.Forms.TextBox tbDestAddress;
        private System.Windows.Forms.CheckBox cbNeedAck;
        private System.Windows.Forms.TextBox tbValue;
        private System.Windows.Forms.NumericUpDown nudNumPkts;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Button btnClearLog;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.ComboBox netifsComboBox;
        private System.Windows.Forms.CheckBox checkBox1;
        private System.Windows.Forms.Label lblHexPrefix;
        private System.Windows.Forms.Label lblSrcAddr;
        private System.Windows.Forms.TextBox tbSrcAddress;
        private System.Windows.Forms.Button button1;
    }
}

