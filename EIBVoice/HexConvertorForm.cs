using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.Text;

namespace HexConverter
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class HexConvertorForm : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.TextBox tbDecimal;
		private System.Windows.Forms.TextBox tbHex;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label label4;
        private Button btnOK;
        private Button btnCancel;

        private FormHexType _hex_type = FormHexType.UShort;
        private string _current_val;

		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public HexConvertorForm()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

            this.AcceptButton = this.btnOK;
            this.CancelButton = this.btnCancel;
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.tbDecimal = new System.Windows.Forms.TextBox();
            this.tbHex = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.btnOK = new System.Windows.Forms.Button();
            this.btnCancel = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(8, 8);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(58, 16);
            this.label1.TabIndex = 0;
            this.label1.Text = "Decimal";
            // 
            // label2
            // 
            this.label2.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(8, 55);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(80, 16);
            this.label2.TabIndex = 1;
            this.label2.Text = "Hexadecimal";
            // 
            // tbDecimal
            // 
            this.tbDecimal.Location = new System.Drawing.Point(11, 27);
            this.tbDecimal.Name = "tbDecimal";
            this.tbDecimal.Size = new System.Drawing.Size(144, 20);
            this.tbDecimal.TabIndex = 2;
            this.tbDecimal.TextChanged += new System.EventHandler(this.tbDecimal_TextChanged);
            this.tbDecimal.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.tbDecimal_KeyPress);
            this.tbDecimal.Enter += new System.EventHandler(this.tbDecimal_Enter);
            // 
            // tbHex
            // 
            this.tbHex.Location = new System.Drawing.Point(11, 74);
            this.tbHex.Name = "tbHex";
            this.tbHex.Size = new System.Drawing.Size(144, 20);
            this.tbHex.TabIndex = 3;
            this.tbHex.TextChanged += new System.EventHandler(this.tbHex_TextChanged);
            this.tbHex.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.tbHex_KeyPress);
            this.tbHex.Enter += new System.EventHandler(this.tbHex_Enter);
            // 
            // label3
            // 
            this.label3.Location = new System.Drawing.Point(62, 8);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(104, 16);
            this.label3.TabIndex = 4;
            this.label3.Text = "(0 to 4294967295)";
            // 
            // label4
            // 
            this.label4.Location = new System.Drawing.Point(86, 55);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(96, 16);
            this.label4.TabIndex = 5;
            this.label4.Text = "(0 to FFFFFFFF)";
            // 
            // btnOK
            // 
            this.btnOK.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.btnOK.Location = new System.Drawing.Point(12, 110);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new System.Drawing.Size(61, 23);
            this.btnOK.TabIndex = 6;
            this.btnOK.Text = "OK";
            this.btnOK.UseVisualStyleBackColor = true;
            // 
            // btnCancel
            // 
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(94, 110);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(61, 23);
            this.btnCancel.TabIndex = 6;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            // 
            // HexConvertorForm
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
            this.ClientSize = new System.Drawing.Size(178, 145);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnOK);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.tbHex);
            this.Controls.Add(this.tbDecimal);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "HexConvertorForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Hex Converter";
            this.ResumeLayout(false);
            this.PerformLayout();

		}
		#endregion

		private void tbDecimal_TextChanged(object sender, System.EventArgs e)
		{
			// Clear string in another textbox if there are non here
            if (tbDecimal.Text.Length <= 0)
            {
                tbHex.Text = "";
                this.btnOK.Enabled = false;
            }
            // Textbox is not empty
            else
            {
                // If another control is simulating the text change,
                // then do not proceed with the text conversion for this control
                if (tbDecimal.Focused == false)
                    return;

                // To hold our converted unsigned type value

                uint uiDecimal = 0;

                try
                {
                    // Convert text string to unsigned integer
                    switch (_hex_type)
                    {
                        case FormHexType.Byte:
                            uiDecimal = checked((uint)System.Convert.ToByte(tbDecimal.Text));
                            break;
                        case FormHexType.UShort:
                            uiDecimal = checked((uint)System.Convert.ToUInt16(tbDecimal.Text));
                            break;
                        default:
                            throw new System.OverflowException();
                    }
                }

                catch (System.OverflowException)
                {
                    // Show overflow message and return
                    tbHex.Text = "Overflow";
                    this.btnOK.Enabled = false;
                    return;
                }

                // Format unsigned integer value to hex and show in another textbox
                tbHex.Text = String.Format("{0:x2}", uiDecimal);
                this.btnOK.Enabled = true;
                _current_val = this.tbHex.Text;
            }
		}

		private void tbHex_TextChanged(object sender, System.EventArgs e)
		{
			// Clear string in another textbox if there are non here
			if (tbHex.Text.Length <= 0)
            {
				tbDecimal.Text = "";
                this.btnOK.Enabled = false;
            }
			// Textbox is not empty
			else
			{
				// If another control is simulating the text change,
				// then do not proceed with the text conversion for this control
				if (tbHex.Focused == false)
					return;
				
				// To hold our converted unsigned integer32 value
				uint uiHex = 0;

				try
				{
					// Convert text string to unsigned integer
                    switch (_hex_type)
                    {
                        case FormHexType.Byte:
                            uiHex = checked((uint)System.Convert.ToByte(tbHex.Text,16));
                            break;
                        case FormHexType.UShort:
                            uiHex = checked((uint)System.Convert.ToUInt16(tbHex.Text,16));
                            break;
                        default:
                            throw new System.OverflowException();
                    }
				}
				
				catch (System.OverflowException) 
				{
					// Show overflow message and return
					tbDecimal.Text = "Overflow";
                    this.btnOK.Enabled = false;
					return;
				}
				
				// Format it and show as a string
				tbDecimal.Text = uiHex.ToString();
                this.btnOK.Enabled = true;
                _current_val = this.tbHex.Text;
			}
		}

		private void tbDecimal_KeyPress(object sender, System.Windows.Forms.KeyPressEventArgs e)
		{
			// Process only digits and backspace
			if (Char.IsDigit(e.KeyChar) == true || e.KeyChar == '\b')
				e.Handled = false;

			// Do not process other keys
			else
				e.Handled = true;
		}

		private void tbHex_KeyPress(object sender, System.Windows.Forms.KeyPressEventArgs e)
		{
			// Process only digits, alphabets and backspace
			if (Char.IsLetterOrDigit(e.KeyChar) == true || e.KeyChar == '\b')
			{
				// If it is an alphabet, process only A-F
				if (Char.IsLetter(e.KeyChar) == true)
				{
					// If alphabet is from A-F, process it
					if (Char.ToUpper(e.KeyChar) == 'A' || Char.ToUpper(e.KeyChar) == 'B' ||
						Char.ToUpper(e.KeyChar) == 'C' || Char.ToUpper(e.KeyChar) == 'D' ||
						Char.ToUpper(e.KeyChar) == 'E' || Char.ToUpper(e.KeyChar) == 'F')
						e.Handled = false;

						// Alphabet is not from A-F, so do not process it
					else
						e.Handled = true;
				}

				// It is not an alphabet, so process it
				else
					e.Handled = false;
			}

			// Do not process other keys
			else
				e.Handled = true;
		}

		private void tbDecimal_Enter(object sender, System.EventArgs e)
		{
			// Check if we have an "Overflow" message here, remove it
			// when we have a focus cos we are not and should not be
			// handling alphabets here.
			if (tbDecimal.Text == "Overflow")
				tbDecimal.Text = "";
		}

		private void tbHex_Enter(object sender, System.EventArgs e)
		{
			// Check if we have an "Overflow" message here, remove it
			// when we have a focus cos we are not and should not be
			// handling alphabets other than 'a' to 'f' here.
			if (tbHex.Text == "Overflow")
				tbHex.Text = "";
		}

        public FormHexType FormHexType
        {
            get { return _hex_type; }
            set 
            {
                _hex_type = value; 
                uint dec_max = 0;
                string hex_max;

                switch (_hex_type)
                {
                    case FormHexType.Byte:
                        dec_max = byte.MaxValue;
                        hex_max = String.Format("{0:x2}", byte.MaxValue);
                        break;
                    case FormHexType.UShort:
                        dec_max = ushort.MaxValue;
                        hex_max = String.Format("{0:x2}", ushort.MaxValue);
                        break;
                    default:
                        throw new Exception();
                }
                this.label3.Text = String.Format("(0 to {0})", dec_max);
                this.label4.Text = String.Format("(0 to 0x{0})", hex_max.ToUpper());
            }
        }

        public void SetDecimalValue(int value)
        {
            this.tbDecimal.Text = value.ToString();
            this.tbHex.Text = value.ToString("X");

            _current_val = this.tbHex.Text;
        }

        public string CurrentValue
        {
            get { return _current_val; }
            set { _current_val = value; }
        }
	}

    public enum FormHexType
    {
        Byte,
        UShort
    }
}
