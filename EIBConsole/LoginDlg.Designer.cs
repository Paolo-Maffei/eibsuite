namespace EIBConsole
{
    partial class LoginDlg
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
            this.tbServerAddress = new IPAddressControlLib.IPAddressControl();
            this.gbLogin = new System.Windows.Forms.GroupBox();
            this.tbPassword = new System.Windows.Forms.MaskedTextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.tbUserName = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.tbServerPort = new System.Windows.Forms.TextBox();
            this.lblEIBPort = new System.Windows.Forms.Label();
            this.lblEIBAddress = new System.Windows.Forms.Label();
            this.btnOK = new System.Windows.Forms.Button();
            this.btnCancel = new System.Windows.Forms.Button();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.gbLogin.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // tbServerAddress
            // 
            this.tbServerAddress.AllowInternalTab = false;
            this.tbServerAddress.AutoHeight = true;
            this.tbServerAddress.BackColor = System.Drawing.SystemColors.Window;
            this.tbServerAddress.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.tbServerAddress.Cursor = System.Windows.Forms.Cursors.IBeam;
            this.tbServerAddress.Location = new System.Drawing.Point(148, 27);
            this.tbServerAddress.MinimumSize = new System.Drawing.Size(87, 20);
            this.tbServerAddress.Name = "tbServerAddress";
            this.tbServerAddress.ReadOnly = false;
            this.tbServerAddress.Size = new System.Drawing.Size(124, 20);
            this.tbServerAddress.TabIndex = 0;
            this.tbServerAddress.Text = "192.168.1.102";
            // 
            // gbLogin
            // 
            this.gbLogin.Controls.Add(this.tbPassword);
            this.gbLogin.Controls.Add(this.label2);
            this.gbLogin.Controls.Add(this.tbUserName);
            this.gbLogin.Controls.Add(this.label1);
            this.gbLogin.Controls.Add(this.tbServerPort);
            this.gbLogin.Controls.Add(this.lblEIBPort);
            this.gbLogin.Controls.Add(this.lblEIBAddress);
            this.gbLogin.Controls.Add(this.tbServerAddress);
            this.gbLogin.Location = new System.Drawing.Point(12, 45);
            this.gbLogin.Name = "gbLogin";
            this.gbLogin.Size = new System.Drawing.Size(307, 196);
            this.gbLogin.TabIndex = 6;
            this.gbLogin.TabStop = false;
            this.gbLogin.Text = "Login";
            // 
            // tbPassword
            // 
            this.tbPassword.Location = new System.Drawing.Point(148, 144);
            this.tbPassword.Name = "tbPassword";
            this.tbPassword.PasswordChar = '*';
            this.tbPassword.Size = new System.Drawing.Size(124, 20);
            this.tbPassword.TabIndex = 3;
            this.tbPassword.Text = "eibadmin1";
            this.tbPassword.UseSystemPasswordChar = true;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(26, 147);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(56, 13);
            this.label2.TabIndex = 6;
            this.label2.Text = "Password:";
            // 
            // tbUserName
            // 
            this.tbUserName.Location = new System.Drawing.Point(148, 107);
            this.tbUserName.Name = "tbUserName";
            this.tbUserName.Size = new System.Drawing.Size(124, 20);
            this.tbUserName.TabIndex = 2;
            this.tbUserName.Text = "eibadmin";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(26, 110);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(63, 13);
            this.label1.TabIndex = 6;
            this.label1.Text = "User Name:";
            // 
            // tbServerPort
            // 
            this.tbServerPort.Location = new System.Drawing.Point(148, 68);
            this.tbServerPort.Name = "tbServerPort";
            this.tbServerPort.Size = new System.Drawing.Size(124, 20);
            this.tbServerPort.TabIndex = 1;
            this.tbServerPort.Text = "6000";
            // 
            // lblEIBPort
            // 
            this.lblEIBPort.AutoSize = true;
            this.lblEIBPort.Location = new System.Drawing.Point(26, 71);
            this.lblEIBPort.Name = "lblEIBPort";
            this.lblEIBPort.Size = new System.Drawing.Size(83, 13);
            this.lblEIBPort.TabIndex = 6;
            this.lblEIBPort.Text = "EIB Server Port:";
            // 
            // lblEIBAddress
            // 
            this.lblEIBAddress.AutoSize = true;
            this.lblEIBAddress.Location = new System.Drawing.Point(26, 30);
            this.lblEIBAddress.Name = "lblEIBAddress";
            this.lblEIBAddress.Size = new System.Drawing.Size(102, 13);
            this.lblEIBAddress.TabIndex = 6;
            this.lblEIBAddress.Text = "EIB Server Address:";
            // 
            // btnOK
            // 
            this.btnOK.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.btnOK.Location = new System.Drawing.Point(173, 247);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new System.Drawing.Size(80, 23);
            this.btnOK.TabIndex = 4;
            this.btnOK.Text = "OK";
            this.btnOK.UseVisualStyleBackColor = true;
            // 
            // btnCancel
            // 
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(60, 247);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(80, 23);
            this.btnCancel.TabIndex = 5;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            // 
            // pictureBox1
            // 
            this.pictureBox1.Dock = System.Windows.Forms.DockStyle.Top;
            this.pictureBox1.Image = global::EIBConsole.Properties.Resources.eib_logo;
            this.pictureBox1.Location = new System.Drawing.Point(0, 0);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(331, 40);
            this.pictureBox1.TabIndex = 8;
            this.pictureBox1.TabStop = false;
            // 
            // LoginDlg
            // 
            this.AcceptButton = this.btnOK;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.btnCancel;
            this.ClientSize = new System.Drawing.Size(331, 278);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnOK);
            this.Controls.Add(this.gbLogin);
            this.Name = "LoginDlg";
            this.Text = "Login";
            this.gbLogin.ResumeLayout(false);
            this.gbLogin.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private IPAddressControlLib.IPAddressControl tbServerAddress;
        private System.Windows.Forms.GroupBox gbLogin;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox tbUserName;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox tbServerPort;
        private System.Windows.Forms.Label lblEIBPort;
        private System.Windows.Forms.Label lblEIBAddress;
        private System.Windows.Forms.MaskedTextBox tbPassword;
        private System.Windows.Forms.Button btnOK;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.PictureBox pictureBox1;
    }
}