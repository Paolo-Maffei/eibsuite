namespace EIBMobile
{
    partial class LoginDlg
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;
        private System.Windows.Forms.MainMenu mainMenu1;

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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(LoginDlg));
            this.mainMenu1 = new System.Windows.Forms.MainMenu();
            this.pbLogo = new System.Windows.Forms.PictureBox();
            this.lblEIBAddress = new System.Windows.Forms.Label();
            this.lblEIBPort = new System.Windows.Forms.Label();
            this.tbServerPort = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.tbUserName = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.tbPassword = new System.Windows.Forms.TextBox();
            this.btnOK = new System.Windows.Forms.Button();
            this.btnCancel = new System.Windows.Forms.Button();
            this.tbServerAddress = new EIBMobile.Controls.IPAddressControl();
            this.SuspendLayout();
            // 
            // pbLogo
            // 
            this.pbLogo.Dock = System.Windows.Forms.DockStyle.Top;
            this.pbLogo.Image = ((System.Drawing.Image)(resources.GetObject("pbLogo.Image")));
            this.pbLogo.Location = new System.Drawing.Point(0, 0);
            this.pbLogo.Name = "pbLogo";
            this.pbLogo.Size = new System.Drawing.Size(240, 40);
            // 
            // lblEIBAddress
            // 
            this.lblEIBAddress.Location = new System.Drawing.Point(3, 43);
            this.lblEIBAddress.Name = "lblEIBAddress";
            this.lblEIBAddress.Size = new System.Drawing.Size(121, 16);
            this.lblEIBAddress.Text = "EIB Server Address:";
            // 
            // lblEIBPort
            // 
            this.lblEIBPort.Location = new System.Drawing.Point(3, 95);
            this.lblEIBPort.Name = "lblEIBPort";
            this.lblEIBPort.Size = new System.Drawing.Size(101, 19);
            this.lblEIBPort.Text = "EIB Server Port:";
            // 
            // tbServerPort
            // 
            this.tbServerPort.Location = new System.Drawing.Point(3, 110);
            this.tbServerPort.Name = "tbServerPort";
            this.tbServerPort.Size = new System.Drawing.Size(124, 21);
            this.tbServerPort.TabIndex = 2;
            this.tbServerPort.Text = "6000";
            // 
            // label1
            // 
            this.label1.Location = new System.Drawing.Point(3, 143);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(79, 17);
            this.label1.Text = "User Name:";
            // 
            // tbUserName
            // 
            this.tbUserName.Location = new System.Drawing.Point(3, 160);
            this.tbUserName.Name = "tbUserName";
            this.tbUserName.Size = new System.Drawing.Size(124, 21);
            this.tbUserName.TabIndex = 3;
            this.tbUserName.Text = "Yosi";
            // 
            // label2
            // 
            this.label2.Location = new System.Drawing.Point(3, 194);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(67, 17);
            this.label2.Text = "Password:";
            // 
            // tbPassword
            // 
            this.tbPassword.Location = new System.Drawing.Point(4, 210);
            this.tbPassword.Name = "tbPassword";
            this.tbPassword.PasswordChar = '*';
            this.tbPassword.Size = new System.Drawing.Size(123, 21);
            this.tbPassword.TabIndex = 4;
            this.tbPassword.Text = "yosi";
            // 
            // btnOK
            // 
            this.btnOK.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.btnOK.Location = new System.Drawing.Point(22, 242);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new System.Drawing.Size(72, 20);
            this.btnOK.TabIndex = 5;
            this.btnOK.Text = "OK";
            // 
            // btnCancel
            // 
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(139, 242);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(72, 20);
            this.btnCancel.TabIndex = 6;
            this.btnCancel.Text = "Cancel";
            // 
            // tbServerAddress
            // 
            this.tbServerAddress.BackColor = System.Drawing.SystemColors.ControlLight;
            this.tbServerAddress.Location = new System.Drawing.Point(4, 62);
            this.tbServerAddress.Name = "tbServerAddress";
            this.tbServerAddress.Size = new System.Drawing.Size(155, 21);
            this.tbServerAddress.TabIndex = 1;
            // 
            // LoginDlg
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.AutoScroll = true;
            this.ClientSize = new System.Drawing.Size(240, 268);
            this.Controls.Add(this.tbServerAddress);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnOK);
            this.Controls.Add(this.tbPassword);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.tbUserName);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.tbServerPort);
            this.Controls.Add(this.lblEIBPort);
            this.Controls.Add(this.lblEIBAddress);
            this.Controls.Add(this.pbLogo);
            this.Menu = this.mainMenu1;
            this.Name = "LoginDlg";
            this.Text = "LoginDlg";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PictureBox pbLogo;
        private System.Windows.Forms.Label lblEIBAddress;
        private System.Windows.Forms.Label lblEIBPort;
        private System.Windows.Forms.TextBox tbServerPort;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox tbUserName;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox tbPassword;
        private System.Windows.Forms.Button btnOK;
        private System.Windows.Forms.Button btnCancel;
        private EIBMobile.Controls.IPAddressControl tbServerAddress;
    }
}