namespace EIBConsole.Controls
{
    partial class EIBBusMonitorControl
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
            System.Windows.Forms.TreeNode treeNode1 = new System.Windows.Forms.TreeNode("Root", 0, 0);
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(EIBBusMonitorControl));
            this.gbAddresses = new System.Windows.Forms.GroupBox();
            this.treeView1 = new System.Windows.Forms.TreeView();
            this.imageList1 = new System.Windows.Forms.ImageList(this.components);
            this.gbAddressDetails = new System.Windows.Forms.GroupBox();
            this.lblTotalNumSeenVal = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.lblTotalNumSeen = new System.Windows.Forms.Label();
            this.lblLastValueVal = new System.Windows.Forms.Label();
            this.lblLastValueSeen = new System.Windows.Forms.Label();
            this.lblLastTimeSeenVal = new System.Windows.Forms.Label();
            this.lblLastTimeSeen = new System.Windows.Forms.Label();
            this.gbAddressActions = new System.Windows.Forms.GroupBox();
            this.btnReplayLastPkt = new System.Windows.Forms.Button();
            this.gbAddresses.SuspendLayout();
            this.gbAddressDetails.SuspendLayout();
            this.gbAddressActions.SuspendLayout();
            this.SuspendLayout();
            // 
            // gbAddresses
            // 
            this.gbAddresses.Controls.Add(this.treeView1);
            this.gbAddresses.Location = new System.Drawing.Point(3, 3);
            this.gbAddresses.Name = "gbAddresses";
            this.gbAddresses.Size = new System.Drawing.Size(211, 275);
            this.gbAddresses.TabIndex = 0;
            this.gbAddresses.TabStop = false;
            this.gbAddresses.Text = "Addresses";
            // 
            // treeView1
            // 
            this.treeView1.ImageIndex = 0;
            this.treeView1.ImageList = this.imageList1;
            this.treeView1.Location = new System.Drawing.Point(30, 45);
            this.treeView1.Name = "treeView1";
            treeNode1.ImageIndex = 0;
            treeNode1.Name = "Root";
            treeNode1.SelectedImageIndex = 0;
            treeNode1.Text = "Root";
            this.treeView1.Nodes.AddRange(new System.Windows.Forms.TreeNode[] {
            treeNode1});
            this.treeView1.SelectedImageIndex = 0;
            this.treeView1.Size = new System.Drawing.Size(151, 218);
            this.treeView1.TabIndex = 0;
            // 
            // imageList1
            // 
            this.imageList1.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList1.ImageStream")));
            this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
            this.imageList1.Images.SetKeyName(0, "folderopen.ico");
            this.imageList1.Images.SetKeyName(1, "eib_addr");
            // 
            // gbAddressDetails
            // 
            this.gbAddressDetails.Controls.Add(this.lblTotalNumSeenVal);
            this.gbAddressDetails.Controls.Add(this.label3);
            this.gbAddressDetails.Controls.Add(this.lblTotalNumSeen);
            this.gbAddressDetails.Controls.Add(this.lblLastValueVal);
            this.gbAddressDetails.Controls.Add(this.lblLastValueSeen);
            this.gbAddressDetails.Controls.Add(this.lblLastTimeSeenVal);
            this.gbAddressDetails.Controls.Add(this.lblLastTimeSeen);
            this.gbAddressDetails.Location = new System.Drawing.Point(220, 3);
            this.gbAddressDetails.Name = "gbAddressDetails";
            this.gbAddressDetails.Size = new System.Drawing.Size(398, 149);
            this.gbAddressDetails.TabIndex = 1;
            this.gbAddressDetails.TabStop = false;
            this.gbAddressDetails.Text = "Address Information";
            // 
            // lblTotalNumSeenVal
            // 
            this.lblTotalNumSeenVal.AutoSize = true;
            this.lblTotalNumSeenVal.Location = new System.Drawing.Point(152, 60);
            this.lblTotalNumSeenVal.Name = "lblTotalNumSeenVal";
            this.lblTotalNumSeenVal.Size = new System.Drawing.Size(13, 13);
            this.lblTotalNumSeenVal.TabIndex = 0;
            this.lblTotalNumSeenVal.Text = "0";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(15, 60);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(114, 13);
            this.label3.TabIndex = 0;
            this.label3.Text = "Total number of times :";
            // 
            // lblTotalNumSeen
            // 
            this.lblTotalNumSeen.AutoSize = true;
            this.lblTotalNumSeen.Location = new System.Drawing.Point(15, 60);
            this.lblTotalNumSeen.Name = "lblTotalNumSeen";
            this.lblTotalNumSeen.Size = new System.Drawing.Size(114, 13);
            this.lblTotalNumSeen.TabIndex = 0;
            this.lblTotalNumSeen.Text = "Total number of times :";
            // 
            // lblLastValueVal
            // 
            this.lblLastValueVal.AutoSize = true;
            this.lblLastValueVal.Location = new System.Drawing.Point(152, 87);
            this.lblLastValueVal.Name = "lblLastValueVal";
            this.lblLastValueVal.Size = new System.Drawing.Size(24, 13);
            this.lblLastValueVal.TabIndex = 0;
            this.lblLastValueVal.Text = "0x0";
            // 
            // lblLastValueSeen
            // 
            this.lblLastValueSeen.AutoSize = true;
            this.lblLastValueSeen.Location = new System.Drawing.Point(15, 87);
            this.lblLastValueSeen.Name = "lblLastValueSeen";
            this.lblLastValueSeen.Size = new System.Drawing.Size(108, 13);
            this.lblLastValueSeen.TabIndex = 0;
            this.lblLastValueSeen.Text = "Last Value recorded :";
            // 
            // lblLastTimeSeenVal
            // 
            this.lblLastTimeSeenVal.AutoSize = true;
            this.lblLastTimeSeenVal.Location = new System.Drawing.Point(152, 30);
            this.lblLastTimeSeenVal.Name = "lblLastTimeSeenVal";
            this.lblLastTimeSeenVal.Size = new System.Drawing.Size(36, 13);
            this.lblLastTimeSeenVal.TabIndex = 0;
            this.lblLastTimeSeenVal.Text = "Never";
            // 
            // lblLastTimeSeen
            // 
            this.lblLastTimeSeen.AutoSize = true;
            this.lblLastTimeSeen.Location = new System.Drawing.Point(15, 30);
            this.lblLastTimeSeen.Name = "lblLastTimeSeen";
            this.lblLastTimeSeen.Size = new System.Drawing.Size(85, 13);
            this.lblLastTimeSeen.TabIndex = 0;
            this.lblLastTimeSeen.Text = "Last Time seen :";
            // 
            // gbAddressActions
            // 
            this.gbAddressActions.Controls.Add(this.btnReplayLastPkt);
            this.gbAddressActions.Location = new System.Drawing.Point(220, 158);
            this.gbAddressActions.Name = "gbAddressActions";
            this.gbAddressActions.Size = new System.Drawing.Size(398, 120);
            this.gbAddressActions.TabIndex = 1;
            this.gbAddressActions.TabStop = false;
            this.gbAddressActions.Text = "Actions";
            // 
            // btnReplayLastPkt
            // 
            this.btnReplayLastPkt.Location = new System.Drawing.Point(18, 28);
            this.btnReplayLastPkt.Name = "btnReplayLastPkt";
            this.btnReplayLastPkt.Size = new System.Drawing.Size(82, 71);
            this.btnReplayLastPkt.TabIndex = 0;
            this.btnReplayLastPkt.Text = "Send";
            this.btnReplayLastPkt.UseVisualStyleBackColor = true;
            // 
            // EIBBusMonitorControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.gbAddressActions);
            this.Controls.Add(this.gbAddressDetails);
            this.Controls.Add(this.gbAddresses);
            this.Name = "EIBBusMonitorControl";
            this.Size = new System.Drawing.Size(621, 295);
            this.Load += new System.EventHandler(this.EIBBusMonitorControl_Load);
            this.gbAddresses.ResumeLayout(false);
            this.gbAddressDetails.ResumeLayout(false);
            this.gbAddressDetails.PerformLayout();
            this.gbAddressActions.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox gbAddresses;
        private System.Windows.Forms.TreeView treeView1;
        private System.Windows.Forms.GroupBox gbAddressDetails;
        private System.Windows.Forms.GroupBox gbAddressActions;
        private System.Windows.Forms.ImageList imageList1;
        private System.Windows.Forms.Label lblTotalNumSeen;
        private System.Windows.Forms.Label lblLastValueSeen;
        private System.Windows.Forms.Label lblLastTimeSeen;
        private System.Windows.Forms.Label lblTotalNumSeenVal;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label lblLastValueVal;
        private System.Windows.Forms.Label lblLastTimeSeenVal;
        private System.Windows.Forms.Button btnReplayLastPkt;
    }
}
