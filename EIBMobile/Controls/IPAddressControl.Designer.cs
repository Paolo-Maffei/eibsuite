namespace EIBMobile.Controls
{
    partial class IPAddressControl
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
            this.label1 = new System.Windows.Forms.Label();
            this.tb1 = new System.Windows.Forms.TextBox();
            this.tb2 = new System.Windows.Forms.TextBox();
            this.tb3 = new System.Windows.Forms.TextBox();
            this.tb4 = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.Location = new System.Drawing.Point(32, 3);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(7, 12);
            this.label1.Text = ".";
            // 
            // tb1
            // 
            this.tb1.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tb1.Location = new System.Drawing.Point(1, 3);
            this.tb1.MaxLength = 3;
            this.tb1.Name = "tb1";
            this.tb1.Size = new System.Drawing.Size(30, 21);
            this.tb1.TabIndex = 1;
            this.tb1.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.tb1_KeyPress);
            // 
            // tb2
            // 
            this.tb2.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tb2.Location = new System.Drawing.Point(40, 3);
            this.tb2.MaxLength = 3;
            this.tb2.Name = "tb2";
            this.tb2.Size = new System.Drawing.Size(30, 21);
            this.tb2.TabIndex = 2;
            this.tb2.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.tb2_KeyPress);
            // 
            // tb3
            // 
            this.tb3.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tb3.Location = new System.Drawing.Point(79, 3);
            this.tb3.MaxLength = 3;
            this.tb3.Name = "tb3";
            this.tb3.Size = new System.Drawing.Size(30, 21);
            this.tb3.TabIndex = 3;
            this.tb3.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.tb3_KeyPress);
            // 
            // tb4
            // 
            this.tb4.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tb4.Location = new System.Drawing.Point(118, 3);
            this.tb4.MaxLength = 3;
            this.tb4.Name = "tb4";
            this.tb4.Size = new System.Drawing.Size(30, 21);
            this.tb4.TabIndex = 4;
            this.tb4.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.tb4_KeyPress);
            // 
            // label2
            // 
            this.label2.Location = new System.Drawing.Point(71, 3);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(7, 12);
            this.label2.Text = ".";
            // 
            // label3
            // 
            this.label3.Location = new System.Drawing.Point(110, 3);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(7, 12);
            this.label3.Text = ".";
            // 
            // IPAddressControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.BackColor = System.Drawing.SystemColors.ControlLight;
            this.Controls.Add(this.tb4);
            this.Controls.Add(this.tb3);
            this.Controls.Add(this.tb2);
            this.Controls.Add(this.tb1);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Name = "IPAddressControl";
            this.Size = new System.Drawing.Size(151, 20);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox tb1;
        private System.Windows.Forms.TextBox tb2;
        private System.Windows.Forms.TextBox tb3;
        private System.Windows.Forms.TextBox tb4;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
    }
}
