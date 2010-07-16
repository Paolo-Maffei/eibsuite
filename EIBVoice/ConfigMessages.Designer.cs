namespace EIBVoice
{
    partial class ConfigMessages
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ConfigMessages));
            System.Windows.Forms.TreeNode treeNode1 = new System.Windows.Forms.TreeNode("Root");
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.splitContainer2 = new System.Windows.Forms.SplitContainer();
            this.toolStripCategories = new System.Windows.Forms.ToolStrip();
            this.btnLoadFile = new System.Windows.Forms.ToolStripButton();
            this.btnSaveFile = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.btnAddCategory = new System.Windows.Forms.ToolStripButton();
            this.btnDeleteCategory = new System.Windows.Forms.ToolStripButton();
            this.tvCategories = new System.Windows.Forms.TreeView();
            this.imageList = new System.Windows.Forms.ImageList(this.components);
            this.splitContainer3 = new System.Windows.Forms.SplitContainer();
            this.toolStripRecords = new System.Windows.Forms.ToolStrip();
            this.btnAddRecord = new System.Windows.Forms.ToolStripButton();
            this.btnRemoveRecord = new System.Windows.Forms.ToolStripButton();
            this.lvRecords = new EIBVoice.ListViewEx();
            this.columnHeaderPhrase = new System.Windows.Forms.ColumnHeader();
            this.columnHeaderMessageControl = new System.Windows.Forms.ColumnHeader();
            this.columnHeaderSourceAddress = new System.Windows.Forms.ColumnHeader();
            this.columnHeaderDestAddress = new System.Windows.Forms.ColumnHeader();
            this.columnHeaderTCPI = new System.Windows.Forms.ColumnHeader();
            this.columnHeaderACPI = new System.Windows.Forms.ColumnHeader();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.splitContainer2.Panel1.SuspendLayout();
            this.splitContainer2.Panel2.SuspendLayout();
            this.splitContainer2.SuspendLayout();
            this.toolStripCategories.SuspendLayout();
            this.splitContainer3.Panel1.SuspendLayout();
            this.splitContainer3.Panel2.SuspendLayout();
            this.splitContainer3.SuspendLayout();
            this.toolStripRecords.SuspendLayout();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            this.splitContainer1.Location = new System.Drawing.Point(70, 98);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.splitContainer2);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.splitContainer3);
            this.splitContainer1.Size = new System.Drawing.Size(692, 372);
            this.splitContainer1.SplitterDistance = 250;
            this.splitContainer1.TabIndex = 0;
            // 
            // splitContainer2
            // 
            this.splitContainer2.Location = new System.Drawing.Point(49, 72);
            this.splitContainer2.Name = "splitContainer2";
            this.splitContainer2.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer2.Panel1
            // 
            this.splitContainer2.Panel1.Controls.Add(this.toolStripCategories);
            // 
            // splitContainer2.Panel2
            // 
            this.splitContainer2.Panel2.Controls.Add(this.tvCategories);
            this.splitContainer2.Size = new System.Drawing.Size(150, 236);
            this.splitContainer2.SplitterDistance = 70;
            this.splitContainer2.TabIndex = 0;
            // 
            // toolStripCategories
            // 
            this.toolStripCategories.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btnLoadFile,
            this.btnSaveFile,
            this.toolStripSeparator1,
            this.btnAddCategory,
            this.btnDeleteCategory});
            this.toolStripCategories.Location = new System.Drawing.Point(0, 0);
            this.toolStripCategories.Name = "toolStripCategories";
            this.toolStripCategories.Size = new System.Drawing.Size(150, 25);
            this.toolStripCategories.TabIndex = 0;
            this.toolStripCategories.Text = "toolStrip1";
            // 
            // btnLoadFile
            // 
            this.btnLoadFile.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnLoadFile.Image = ((System.Drawing.Image)(resources.GetObject("btnLoadFile.Image")));
            this.btnLoadFile.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnLoadFile.Name = "btnLoadFile";
            this.btnLoadFile.Size = new System.Drawing.Size(23, 22);
            this.btnLoadFile.Text = "Open";
            this.btnLoadFile.ToolTipText = "Open";
            this.btnLoadFile.Click += new System.EventHandler(this.btnLoadFile_Click);
            // 
            // btnSaveFile
            // 
            this.btnSaveFile.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnSaveFile.Image = ((System.Drawing.Image)(resources.GetObject("btnSaveFile.Image")));
            this.btnSaveFile.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnSaveFile.Name = "btnSaveFile";
            this.btnSaveFile.Size = new System.Drawing.Size(23, 22);
            this.btnSaveFile.Text = "Save";
            this.btnSaveFile.ToolTipText = "Save";
            this.btnSaveFile.Click += new System.EventHandler(this.btnSaveFile_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
            // 
            // btnAddCategory
            // 
            this.btnAddCategory.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnAddCategory.Image = ((System.Drawing.Image)(resources.GetObject("btnAddCategory.Image")));
            this.btnAddCategory.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnAddCategory.Name = "btnAddCategory";
            this.btnAddCategory.Size = new System.Drawing.Size(23, 22);
            this.btnAddCategory.Text = "Add category";
            this.btnAddCategory.Click += new System.EventHandler(this.btnAddCategory_Click);
            // 
            // btnDeleteCategory
            // 
            this.btnDeleteCategory.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnDeleteCategory.Enabled = false;
            this.btnDeleteCategory.Image = ((System.Drawing.Image)(resources.GetObject("btnDeleteCategory.Image")));
            this.btnDeleteCategory.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnDeleteCategory.Name = "btnDeleteCategory";
            this.btnDeleteCategory.Size = new System.Drawing.Size(23, 22);
            this.btnDeleteCategory.Text = "toolStripButton2";
            this.btnDeleteCategory.ToolTipText = "Delete category";
            this.btnDeleteCategory.Click += new System.EventHandler(this.btnDeleteCategory_Click);
            // 
            // tvCategories
            // 
            this.tvCategories.HideSelection = false;
            this.tvCategories.ImageIndex = 0;
            this.tvCategories.ImageList = this.imageList;
            this.tvCategories.LabelEdit = true;
            this.tvCategories.Location = new System.Drawing.Point(39, 43);
            this.tvCategories.Name = "tvCategories";
            treeNode1.Name = "Root";
            treeNode1.Text = "Root";
            this.tvCategories.Nodes.AddRange(new System.Windows.Forms.TreeNode[] {
            treeNode1});
            this.tvCategories.SelectedImageIndex = 0;
            this.tvCategories.Size = new System.Drawing.Size(72, 80);
            this.tvCategories.TabIndex = 0;
            this.tvCategories.AfterLabelEdit += new System.Windows.Forms.NodeLabelEditEventHandler(this.tvCategories_AfterLabelEdit);
            this.tvCategories.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.tvRecords_AfterSelect);
            // 
            // imageList
            // 
            this.imageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList.ImageStream")));
            this.imageList.TransparentColor = System.Drawing.Color.Transparent;
            this.imageList.Images.SetKeyName(0, "db.ico");
            this.imageList.Images.SetKeyName(1, "VSProject_genericproject.ico");
            this.imageList.Images.SetKeyName(2, "batfile.png");
            // 
            // splitContainer3
            // 
            this.splitContainer3.Location = new System.Drawing.Point(162, 72);
            this.splitContainer3.Name = "splitContainer3";
            this.splitContainer3.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer3.Panel1
            // 
            this.splitContainer3.Panel1.Controls.Add(this.toolStripRecords);
            // 
            // splitContainer3.Panel2
            // 
            this.splitContainer3.Panel2.Controls.Add(this.lvRecords);
            this.splitContainer3.Size = new System.Drawing.Size(150, 236);
            this.splitContainer3.SplitterDistance = 67;
            this.splitContainer3.TabIndex = 0;
            // 
            // toolStripRecords
            // 
            this.toolStripRecords.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btnAddRecord,
            this.btnRemoveRecord});
            this.toolStripRecords.Location = new System.Drawing.Point(0, 0);
            this.toolStripRecords.Name = "toolStripRecords";
            this.toolStripRecords.Size = new System.Drawing.Size(150, 25);
            this.toolStripRecords.TabIndex = 0;
            this.toolStripRecords.Text = "toolStrip2";
            // 
            // btnAddRecord
            // 
            this.btnAddRecord.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnAddRecord.Image = ((System.Drawing.Image)(resources.GetObject("btnAddRecord.Image")));
            this.btnAddRecord.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnAddRecord.Name = "btnAddRecord";
            this.btnAddRecord.Size = new System.Drawing.Size(23, 22);
            this.btnAddRecord.Text = "Add record";
            this.btnAddRecord.Click += new System.EventHandler(this.btnAddRecord_Click);
            // 
            // btnRemoveRecord
            // 
            this.btnRemoveRecord.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnRemoveRecord.Image = ((System.Drawing.Image)(resources.GetObject("btnRemoveRecord.Image")));
            this.btnRemoveRecord.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnRemoveRecord.Name = "btnRemoveRecord";
            this.btnRemoveRecord.Size = new System.Drawing.Size(23, 22);
            this.btnRemoveRecord.Text = "Delete record";
            this.btnRemoveRecord.Click += new System.EventHandler(this.btnRemoveRecord_Click);
            // 
            // lvRecords
            // 
            this.lvRecords.AllowColumnReorder = true;
            this.lvRecords.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeaderPhrase,
            this.columnHeaderMessageControl,
            this.columnHeaderSourceAddress,
            this.columnHeaderDestAddress,
            this.columnHeaderTCPI,
            this.columnHeaderACPI});
            this.lvRecords.DoubleClickActivation = false;
            this.lvRecords.FullRowSelect = true;
            this.lvRecords.GridLines = true;
            this.lvRecords.LabelEdit = true;
            this.lvRecords.Location = new System.Drawing.Point(41, 50);
            this.lvRecords.Name = "lvRecords";
            this.lvRecords.Size = new System.Drawing.Size(78, 76);
            this.lvRecords.SmallImageList = this.imageList;
            this.lvRecords.TabIndex = 0;
            this.lvRecords.UseCompatibleStateImageBehavior = false;
            this.lvRecords.View = System.Windows.Forms.View.Details;
            this.lvRecords.AfterLabelEdit += new System.Windows.Forms.LabelEditEventHandler(this.lvRecords_AfterLabelEdit);
            this.lvRecords.SelectedIndexChanged += new System.EventHandler(this.lvRecords_SelectedIndexChanged);
            // 
            // columnHeaderPhrase
            // 
            this.columnHeaderPhrase.Text = "Phrase";
            // 
            // columnHeaderMessageControl
            // 
            this.columnHeaderMessageControl.Text = "Message Control";
            // 
            // columnHeaderSourceAddress
            // 
            this.columnHeaderSourceAddress.Text = "Source Address";
            // 
            // columnHeaderDestAddress
            // 
            this.columnHeaderDestAddress.Text = "Destination Address";
            // 
            // columnHeaderTCPI
            // 
            this.columnHeaderTCPI.Text = "TCPI";
            // 
            // columnHeaderACPI
            // 
            this.columnHeaderACPI.Text = "ACPI";
            // 
            // ConfigMessages
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(898, 573);
            this.Controls.Add(this.splitContainer1);
            this.Name = "ConfigMessages";
            this.Text = "ConfigMessages";
            this.Load += new System.EventHandler(this.ConfigMessages_Load);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.ResumeLayout(false);
            this.splitContainer2.Panel1.ResumeLayout(false);
            this.splitContainer2.Panel1.PerformLayout();
            this.splitContainer2.Panel2.ResumeLayout(false);
            this.splitContainer2.ResumeLayout(false);
            this.toolStripCategories.ResumeLayout(false);
            this.toolStripCategories.PerformLayout();
            this.splitContainer3.Panel1.ResumeLayout(false);
            this.splitContainer3.Panel1.PerformLayout();
            this.splitContainer3.Panel2.ResumeLayout(false);
            this.splitContainer3.ResumeLayout(false);
            this.toolStripRecords.ResumeLayout(false);
            this.toolStripRecords.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.SplitContainer splitContainer2;
        private System.Windows.Forms.SplitContainer splitContainer3;
        private System.Windows.Forms.ToolStrip toolStripCategories;
        private System.Windows.Forms.ToolStripButton btnAddCategory;
        private System.Windows.Forms.ToolStripButton btnDeleteCategory;
        private System.Windows.Forms.TreeView tvCategories;
        private System.Windows.Forms.ToolStrip toolStripRecords;
        private System.Windows.Forms.ToolStripButton btnAddRecord;
        private ListViewEx lvRecords;
        private System.Windows.Forms.ImageList imageList;
        private System.Windows.Forms.ToolStripButton btnLoadFile;
        private System.Windows.Forms.ToolStripButton btnSaveFile;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ColumnHeader columnHeaderPhrase;
        private System.Windows.Forms.ColumnHeader columnHeaderMessageControl;
        private System.Windows.Forms.ColumnHeader columnHeaderSourceAddress;
        private System.Windows.Forms.ColumnHeader columnHeaderDestAddress;
        private System.Windows.Forms.ColumnHeader columnHeaderTCPI;
        private System.Windows.Forms.ColumnHeader columnHeaderACPI;
        private System.Windows.Forms.ToolStripButton btnRemoveRecord;
    }
}