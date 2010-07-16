namespace EIBConsole
{
    partial class AddUserForm
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
            this.btnOK = new System.Windows.Forms.Button();
            this.btnCancel = new System.Windows.Forms.Button();
            this.cbReadAllowed = new System.Windows.Forms.CheckBox();
            this.cbWriteAllowed = new System.Windows.Forms.CheckBox();
            this.gbPolicy = new System.Windows.Forms.GroupBox();
            this.label4 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.tbDAMask = new System.Windows.Forms.TextBox();
            this.tbSAMask = new System.Windows.Forms.TextBox();
            this.cbConsoleAccss = new System.Windows.Forms.CheckBox();
            this.label1 = new System.Windows.Forms.Label();
            this.lblPassword = new System.Windows.Forms.Label();
            this.tbUserName = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.tbmaskPassword = new System.Windows.Forms.MaskedTextBox();
            this.tbmaskRetypePassword = new System.Windows.Forms.MaskedTextBox();
            this.gbPolicy.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnOK
            // 
            this.btnOK.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.btnOK.Location = new System.Drawing.Point(161, 259);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new System.Drawing.Size(75, 23);
            this.btnOK.TabIndex = 8;
            this.btnOK.Text = "OK";
            this.btnOK.UseVisualStyleBackColor = true;
            this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
            // 
            // btnCancel
            // 
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(46, 259);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(75, 23);
            this.btnCancel.TabIndex = 9;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            // 
            // cbReadAllowed
            // 
            this.cbReadAllowed.AutoSize = true;
            this.cbReadAllowed.Location = new System.Drawing.Point(9, 71);
            this.cbReadAllowed.Name = "cbReadAllowed";
            this.cbReadAllowed.Size = new System.Drawing.Size(92, 17);
            this.cbReadAllowed.TabIndex = 5;
            this.cbReadAllowed.Text = "Read Allowed";
            this.cbReadAllowed.UseVisualStyleBackColor = true;
            // 
            // cbWriteAllowed
            // 
            this.cbWriteAllowed.AutoSize = true;
            this.cbWriteAllowed.Location = new System.Drawing.Point(9, 94);
            this.cbWriteAllowed.Name = "cbWriteAllowed";
            this.cbWriteAllowed.Size = new System.Drawing.Size(91, 17);
            this.cbWriteAllowed.TabIndex = 6;
            this.cbWriteAllowed.Text = "Write Allowed";
            this.cbWriteAllowed.UseVisualStyleBackColor = true;
            // 
            // gbPolicy
            // 
            this.gbPolicy.Controls.Add(this.label4);
            this.gbPolicy.Controls.Add(this.label2);
            this.gbPolicy.Controls.Add(this.tbDAMask);
            this.gbPolicy.Controls.Add(this.tbSAMask);
            this.gbPolicy.Controls.Add(this.cbReadAllowed);
            this.gbPolicy.Controls.Add(this.cbConsoleAccss);
            this.gbPolicy.Controls.Add(this.cbWriteAllowed);
            this.gbPolicy.Location = new System.Drawing.Point(13, 107);
            this.gbPolicy.Name = "gbPolicy";
            this.gbPolicy.Size = new System.Drawing.Size(272, 146);
            this.gbPolicy.TabIndex = 2;
            this.gbPolicy.TabStop = false;
            this.gbPolicy.Text = "Policy";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(6, 47);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(130, 13);
            this.label4.TabIndex = 3;
            this.label4.Text = "Destination Address Mask";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 21);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(111, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "Source Address Mask";
            // 
            // tbDAMask
            // 
            this.tbDAMask.Location = new System.Drawing.Point(161, 44);
            this.tbDAMask.Name = "tbDAMask";
            this.tbDAMask.Size = new System.Drawing.Size(92, 20);
            this.tbDAMask.TabIndex = 4;
            this.tbDAMask.Text = "FFFF";
            // 
            // tbSAMask
            // 
            this.tbSAMask.Location = new System.Drawing.Point(161, 18);
            this.tbSAMask.Name = "tbSAMask";
            this.tbSAMask.Size = new System.Drawing.Size(92, 20);
            this.tbSAMask.TabIndex = 3;
            this.tbSAMask.Text = "FFFF";
            // 
            // cbConsoleAccss
            // 
            this.cbConsoleAccss.AutoSize = true;
            this.cbConsoleAccss.Location = new System.Drawing.Point(9, 117);
            this.cbConsoleAccss.Name = "cbConsoleAccss";
            this.cbConsoleAccss.Size = new System.Drawing.Size(142, 17);
            this.cbConsoleAccss.TabIndex = 7;
            this.cbConsoleAccss.Text = "Console Access Allowed";
            this.cbConsoleAccss.UseVisualStyleBackColor = true;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 22);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(60, 13);
            this.label1.TabIndex = 4;
            this.label1.Text = "User Name";
            // 
            // lblPassword
            // 
            this.lblPassword.AutoSize = true;
            this.lblPassword.Location = new System.Drawing.Point(12, 53);
            this.lblPassword.Name = "lblPassword";
            this.lblPassword.Size = new System.Drawing.Size(53, 13);
            this.lblPassword.TabIndex = 4;
            this.lblPassword.Text = "Password";
            // 
            // tbUserName
            // 
            this.tbUserName.Location = new System.Drawing.Point(141, 19);
            this.tbUserName.Name = "tbUserName";
            this.tbUserName.Size = new System.Drawing.Size(144, 20);
            this.tbUserName.TabIndex = 0;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 84);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(90, 13);
            this.label3.TabIndex = 4;
            this.label3.Text = "Retype Password";
            // 
            // tbmaskPassword
            // 
            this.tbmaskPassword.Location = new System.Drawing.Point(141, 50);
            this.tbmaskPassword.Name = "tbmaskPassword";
            this.tbmaskPassword.Size = new System.Drawing.Size(144, 20);
            this.tbmaskPassword.TabIndex = 1;
            this.tbmaskPassword.UseSystemPasswordChar = true;
            // 
            // tbmaskRetypePassword
            // 
            this.tbmaskRetypePassword.Location = new System.Drawing.Point(141, 81);
            this.tbmaskRetypePassword.Name = "tbmaskRetypePassword";
            this.tbmaskRetypePassword.Size = new System.Drawing.Size(144, 20);
            this.tbmaskRetypePassword.TabIndex = 2;
            this.tbmaskRetypePassword.UseSystemPasswordChar = true;
            // 
            // AddUserForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(297, 294);
            this.Controls.Add(this.tbmaskRetypePassword);
            this.Controls.Add(this.tbmaskPassword);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.lblPassword);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.tbUserName);
            this.Controls.Add(this.gbPolicy);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnOK);
            this.Name = "AddUserForm";
            this.Text = "AddUserForm";
            this.gbPolicy.ResumeLayout(false);
            this.gbPolicy.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnOK;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.CheckBox cbReadAllowed;
        private System.Windows.Forms.CheckBox cbWriteAllowed;
        private System.Windows.Forms.GroupBox gbPolicy;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label lblPassword;
        private System.Windows.Forms.TextBox tbUserName;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.MaskedTextBox tbmaskPassword;
        private System.Windows.Forms.MaskedTextBox tbmaskRetypePassword;
        private System.Windows.Forms.TextBox tbDAMask;
        private System.Windows.Forms.TextBox tbSAMask;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.CheckBox cbConsoleAccss;
    }
}