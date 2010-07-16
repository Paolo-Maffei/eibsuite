namespace EIBEmulator
{
    partial class KnownGroupsForm
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
            GlacialComponents.Controls.GLColumn glColumn1 = new GlacialComponents.Controls.GLColumn();
            GlacialComponents.Controls.GLColumn glColumn2 = new GlacialComponents.Controls.GLColumn();
            this.btnApply = new System.Windows.Forms.Button();
            this.btnCancel = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.lvGroups = new GlacialComponents.Controls.GlacialList();
            this.btnRemoveGroup = new System.Windows.Forms.Button();
            this.btnAddGroup = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnApply
            // 
            this.btnApply.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.btnApply.Location = new System.Drawing.Point(147, 303);
            this.btnApply.Name = "btnApply";
            this.btnApply.Size = new System.Drawing.Size(75, 23);
            this.btnApply.TabIndex = 0;
            this.btnApply.Text = "Apply";
            this.btnApply.UseVisualStyleBackColor = true;
            this.btnApply.Click += new System.EventHandler(this.btnApply_Click);
            // 
            // btnCancel
            // 
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(18, 303);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(75, 23);
            this.btnCancel.TabIndex = 1;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.lvGroups);
            this.groupBox1.Controls.Add(this.btnRemoveGroup);
            this.groupBox1.Controls.Add(this.btnAddGroup);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(215, 285);
            this.groupBox1.TabIndex = 2;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Known Groups";
            // 
            // lvGroups
            // 
            this.lvGroups.AllowColumnResize = true;
            this.lvGroups.AllowMultiselect = false;
            this.lvGroups.AlternateBackground = System.Drawing.Color.DarkGreen;
            this.lvGroups.AlternatingColors = false;
            this.lvGroups.AutoHeight = true;
            this.lvGroups.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.lvGroups.BackgroundStretchToFit = true;
            glColumn1.ActivatedEmbeddedType = GlacialComponents.Controls.GLActivatedEmbeddedTypes.TextBox;
            glColumn1.CheckBoxes = false;
            glColumn1.ImageIndex = -1;
            glColumn1.Name = "Column1";
            glColumn1.NumericSort = false;
            glColumn1.Text = "Address";
            glColumn1.TextAlignment = System.Drawing.ContentAlignment.MiddleLeft;
            glColumn1.Width = 100;
            glColumn2.ActivatedEmbeddedType = GlacialComponents.Controls.GLActivatedEmbeddedTypes.TextBox;
            glColumn2.CheckBoxes = false;
            glColumn2.ImageIndex = -1;
            glColumn2.Name = "Column2";
            glColumn2.NumericSort = false;
            glColumn2.Text = "Length";
            glColumn2.TextAlignment = System.Drawing.ContentAlignment.MiddleLeft;
            glColumn2.Width = 100;
            this.lvGroups.Columns.AddRange(new GlacialComponents.Controls.GLColumn[] {
            glColumn1,
            glColumn2});
            this.lvGroups.ControlStyle = GlacialComponents.Controls.GLControlStyles.Normal;
            this.lvGroups.FullRowSelect = true;
            this.lvGroups.GridColor = System.Drawing.Color.LightGray;
            this.lvGroups.GridLines = GlacialComponents.Controls.GLGridLines.gridBoth;
            this.lvGroups.GridLineStyle = GlacialComponents.Controls.GLGridLineStyles.gridSolid;
            this.lvGroups.GridTypes = GlacialComponents.Controls.GLGridTypes.gridOnExists;
            this.lvGroups.HeaderHeight = 22;
            this.lvGroups.HeaderVisible = true;
            this.lvGroups.HeaderWordWrap = false;
            this.lvGroups.HotColumnTracking = false;
            this.lvGroups.HotItemTracking = false;
            this.lvGroups.HotTrackingColor = System.Drawing.Color.LightGray;
            this.lvGroups.HoverEvents = false;
            this.lvGroups.HoverTime = 1;
            this.lvGroups.ImageList = null;
            this.lvGroups.ItemHeight = 17;
            this.lvGroups.ItemWordWrap = false;
            this.lvGroups.Location = new System.Drawing.Point(6, 49);
            this.lvGroups.Name = "lvGroups";
            this.lvGroups.Selectable = true;
            this.lvGroups.SelectedTextColor = System.Drawing.Color.White;
            this.lvGroups.SelectionColor = System.Drawing.Color.DarkBlue;
            this.lvGroups.ShowBorder = true;
            this.lvGroups.ShowFocusRect = false;
            this.lvGroups.Size = new System.Drawing.Size(204, 230);
            this.lvGroups.SortType = GlacialComponents.Controls.SortTypes.InsertionSort;
            this.lvGroups.SuperFlatHeaderColor = System.Drawing.Color.White;
            this.lvGroups.TabIndex = 2;
            this.lvGroups.Text = "glacialList1";
            // 
            // btnRemoveGroup
            // 
            this.btnRemoveGroup.Location = new System.Drawing.Point(134, 19);
            this.btnRemoveGroup.Name = "btnRemoveGroup";
            this.btnRemoveGroup.Size = new System.Drawing.Size(75, 23);
            this.btnRemoveGroup.TabIndex = 1;
            this.btnRemoveGroup.Text = "Remove";
            this.btnRemoveGroup.UseVisualStyleBackColor = true;
            this.btnRemoveGroup.Click += new System.EventHandler(this.btnRemoveGroup_Click);
            // 
            // btnAddGroup
            // 
            this.btnAddGroup.Location = new System.Drawing.Point(6, 20);
            this.btnAddGroup.Name = "btnAddGroup";
            this.btnAddGroup.Size = new System.Drawing.Size(75, 23);
            this.btnAddGroup.TabIndex = 1;
            this.btnAddGroup.Text = "Add";
            this.btnAddGroup.UseVisualStyleBackColor = true;
            this.btnAddGroup.Click += new System.EventHandler(this.btnAddGroup_Click);
            // 
            // KnownGroupsForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(233, 336);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnApply);
            this.Name = "KnownGroupsForm";
            this.Text = "KnownGroupsForm";
            this.Load += new System.EventHandler(this.KnownGroupsForm_Load);
            this.groupBox1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btnApply;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button btnRemoveGroup;
        private System.Windows.Forms.Button btnAddGroup;
        private GlacialComponents.Controls.GlacialList lvGroups;
    }
}