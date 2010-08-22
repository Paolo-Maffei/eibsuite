namespace EIBVoice
{
    partial class GeneralSettingsForm
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
            this.btnCancel = new System.Windows.Forms.Button();
            this.btnOK = new System.Windows.Forms.Button();
            this.gbNetwork = new System.Windows.Forms.GroupBox();
            this.netifsComboBox = new System.Windows.Forms.ComboBox();
            this.lblNetworkInterface = new System.Windows.Forms.Label();
            this.gbNetwork.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnCancel
            // 
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(371, 81);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(80, 27);
            this.btnCancel.TabIndex = 8;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            // 
            // btnOK
            // 
            this.btnOK.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.btnOK.Location = new System.Drawing.Point(457, 81);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new System.Drawing.Size(80, 27);
            this.btnOK.TabIndex = 7;
            this.btnOK.Text = "Save";
            this.btnOK.UseVisualStyleBackColor = true;
            this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
            // 
            // gbNetwork
            // 
            this.gbNetwork.Controls.Add(this.netifsComboBox);
            this.gbNetwork.Controls.Add(this.lblNetworkInterface);
            this.gbNetwork.Location = new System.Drawing.Point(12, 12);
            this.gbNetwork.Name = "gbNetwork";
            this.gbNetwork.Size = new System.Drawing.Size(525, 63);
            this.gbNetwork.TabIndex = 9;
            this.gbNetwork.TabStop = false;
            this.gbNetwork.Text = "Network";
            // 
            // netifsComboBox
            // 
            this.netifsComboBox.FormattingEnabled = true;
            this.netifsComboBox.Location = new System.Drawing.Point(64, 22);
            this.netifsComboBox.Name = "netifsComboBox";
            this.netifsComboBox.Size = new System.Drawing.Size(455, 21);
            this.netifsComboBox.TabIndex = 7;
            this.netifsComboBox.SelectedIndexChanged += new System.EventHandler(this.netifsComboBox_SelectedIndexChanged);
            // 
            // lblNetworkInterface
            // 
            this.lblNetworkInterface.AutoSize = true;
            this.lblNetworkInterface.Location = new System.Drawing.Point(6, 25);
            this.lblNetworkInterface.Name = "lblNetworkInterface";
            this.lblNetworkInterface.Size = new System.Drawing.Size(52, 13);
            this.lblNetworkInterface.TabIndex = 6;
            this.lblNetworkInterface.Text = "Interface:";
            // 
            // GeneralSettingsForm
            // 
            this.ClientSize = new System.Drawing.Size(549, 123);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnOK);
            this.Controls.Add(this.gbNetwork);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "GeneralSettingsForm";
            this.Text = "Settings";
            this.gbNetwork.ResumeLayout(false);
            this.gbNetwork.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.Button btnOK;
        private System.Windows.Forms.GroupBox gbNetwork;
        private System.Windows.Forms.ComboBox netifsComboBox;
        private System.Windows.Forms.Label lblNetworkInterface;
    }
}