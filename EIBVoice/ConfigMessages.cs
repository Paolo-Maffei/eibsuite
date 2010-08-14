using System;
using System.Configuration;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using HexConverter;
using EIBVoice.Properties;

namespace EIBVoice
{
    public partial class ConfigMessages : Form
    {
        private DataSet _ds = new DataSet();
        private HexConvertorForm _hex_form = new HexConvertorForm();

        public ConfigMessages()
        {
            InitializeComponent();
            this.lvRecords.SubItemClicked += new SubItemEventHandler(lvRecords_SubItemClicked);
            this.lvRecords.DoubleClickActivation = true;
            
            this.btnRemoveRecord.Enabled = false;
            this.btnAddRecord.Enabled = false;

            this.lvRecords.AutoResizeColumns(ColumnHeaderAutoResizeStyle.HeaderSize);
            
            InitDataSet();
        }

       
        private void ConfigMessages_Load(object sender, EventArgs e)
        {
            this.splitContainer1.Dock = DockStyle.Fill;
            this.splitContainer2.Dock = DockStyle.Fill;
            this.splitContainer3.Dock = DockStyle.Fill;

            this.splitContainer2.SplitterDistance = this.toolStripCategories.Height;
            this.splitContainer3.SplitterDistance = this.toolStripRecords.Height;

            this.splitContainer2.IsSplitterFixed = true;
            this.splitContainer3.IsSplitterFixed = true;

            this.tvCategories.Dock = DockStyle.Fill;
            this.lvRecords.Dock = DockStyle.Fill;

            this.lvRecords.Columns[0].Width += this.lvRecords.Width - (this.lvRecords.Columns[1].Width +
                                                                       this.lvRecords.Columns[2].Width +
                                                                       this.lvRecords.Columns[3].Width +
                                                                       this.lvRecords.Columns[4].Width +
                                                                       this.lvRecords.Columns[5].Width + 60);

            this.btnSaveFile.Enabled = false;

            this.tvCategories.Nodes[0].Text = Settings.Default.RootElementName;
        }
       
        void lvRecords_SubItemClicked(object sender, SubItemEventArgs e)
        {
            if (e.SubItem == 0)
            {
                e.Item.BeginEdit();
                return;
            }

            if (e.SubItem == 2 || e.SubItem == 3)
            {
                _hex_form.FormHexType = FormHexType.UShort;
            }
            else
            {
                _hex_form.FormHexType = FormHexType.Byte;
            }
            string hex_str = e.Item.SubItems[e.SubItem].Text.Remove(0,2);
            int int_val = Convert.ToInt32(hex_str, 16);

            _hex_form.SetDecimalValue(int_val);
            
            if (_hex_form.ShowDialog() == DialogResult.OK)
            {
                e.Item.SubItems[e.SubItem].Text = String.Format("0x{0}", _hex_form.CurrentValue.ToUpper());
                DataRow telegram_row = ((DataRow)e.Item.Tag).GetChildRows(_ds.Relations[0])[0];
                
                telegram_row[e.SubItem - 1] = Convert.ToInt16(_hex_form.CurrentValue, 16);
            }
        }

        public void LoadData(string file_name)
        {
            _ds.ReadXml(file_name);
            this.btnSaveFile.Enabled = true;

            foreach (DataRow cat in _ds.Tables["Category"].Rows)
            {
                TreeNode node = new TreeNode(cat["Name"].ToString(), 1, 1);
                node.Name = cat["Name"].ToString();
                node.Tag = cat;
                this.tvCategories.Nodes[0].Nodes.Add(node);
            }

            this.tvCategories.ExpandAll();
        }

        public void SaveData(string file_name)
        {
            _ds.WriteXml(file_name);
        }

        public void LoadCategoryRecords(TreeNode CategoryNode)
        {
            if (CategoryNode.Tag == null) return;
            
            DataRow category_row = (DataRow)CategoryNode.Tag;
            DataRow[] child_rows = category_row.GetChildRows(_ds.Relations[1]);

            foreach (DataRow row in child_rows)
            {
                //only one telegram per record
                DataRow data = row.GetChildRows(_ds.Relations[0])[0];
                
                ListViewItem item = new ListViewItem();
                item.Tag = row;

                item.ImageIndex = 2;
                //phrase
                item.Text = row["Phrase"].ToString();;
                
                item.SubItems.Add("0x" + ((byte)data["MessageControl"]).ToString("X"));
                item.SubItems.Add("0x" + ((ushort)data["SourceAddress"]).ToString("X"));
                item.SubItems.Add("0x" + ((ushort)data["DestinationAddress"]).ToString("X"));
                item.SubItems.Add("0x" + ((byte)data["TCPI"]).ToString("X"));
                item.SubItems.Add("0x" + ((byte)data["APCI"]).ToString("X"));

                this.lvRecords.Items.Add(item);
            }
        }

        private void tvRecords_AfterSelect(object sender, TreeViewEventArgs e)
        {
            this.lvRecords.Items.Clear();

            if (e.Node.Parent == null)
            {
                this.btnDeleteCategory.Enabled = false;
                this.btnAddRecord.Enabled = false;
                this.btnRemoveRecord.Enabled = false;
                return;
            }

            this.btnAddRecord.Enabled = true;
            this.btnDeleteCategory.Enabled = true;
            LoadCategoryRecords(e.Node);
        }

        private void InitDataSet()
        {
            try
            {
                _ds.ReadXmlSchema(Settings.Default.XMLSchemaFile);
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                OpenFileDialog ofd = new OpenFileDialog();
                ofd.InitialDirectory = System.IO.Directory.GetCurrentDirectory();
                ofd.Multiselect = false;
                ofd.DefaultExt = "xsd";
                ofd.Filter = "XML Schema files (*.xsd)|*.xsd";
                if (ofd.ShowDialog() == DialogResult.OK)
                {
                    Settings.Default.XMLSchemaFile = ofd.FileName;
                    Settings.Default.Save();
                    _ds.ReadXmlSchema(Settings.Default.XMLSchemaFile);
                }
            }
        }

        private void btnAddCategory_Click(object sender, EventArgs e)
        {
            TreeNode node = new TreeNode();
            node.Text = "Category1";
            node.ImageIndex = 1;
            node.SelectedImageIndex = 1;
            this.tvCategories.Nodes[0].Nodes.Add(node);
            
            this.btnSaveFile.Enabled = true;
            this.btnDeleteCategory.Enabled = true;

            DataRow category_row = _ds.Tables["Category"].Rows.Add(node.Text);
            
            node.Tag = category_row;

            this.tvCategories.ExpandAll();
            this.tvCategories.SelectedNode = node;
        }

        private void btnLoadFile_Click(object sender, EventArgs e)
        {         
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.DefaultExt = "xml";
            ofd.Filter = "XML files (*.xml)|*.xml";
            if (ofd.ShowDialog() == DialogResult.OK)
            {
                this.tvCategories.Nodes[0].Nodes.Clear();
                this.lvRecords.Items.Clear();
                _ds.Clear();
                LoadData(ofd.FileName);
            }
                        
        }

        private void tvCategories_AfterLabelEdit(object sender, NodeLabelEditEventArgs e)
        {
            if (e.Label != null)
            {
                e.Node.Name = e.Label;
                e.Node.Text = e.Label;
                DataRow cat_row = (DataRow)e.Node.Tag;
                cat_row["Name"] = e.Label;
            }
        }

        private void btnSaveFile_Click(object sender, EventArgs e)
        {
            SaveFileDialog sfd = new SaveFileDialog();
            sfd.Filter = "XML files (*.xml)|*.xml";
            if (sfd.ShowDialog() == DialogResult.OK)
            {
                SaveData(sfd.FileName);
            }
        }

        private void btnDeleteCategory_Click(object sender, EventArgs e)
        {
            DataRow cat_row = (DataRow)this.tvCategories.SelectedNode.Tag;
            _ds.Tables["Category"].Rows.Remove(cat_row);

            this.tvCategories.Nodes[0].Nodes.Remove(this.tvCategories.SelectedNode);
            if (this.tvCategories.Nodes[0].Nodes.Count == 0)
            {
                this.btnRemoveRecord.Enabled = false;
            }
        }

        private void btnAddRecord_Click(object sender, EventArgs e)
        {
            ListViewItem item = new ListViewItem();
            //item.Tag = row;

            item.ImageIndex = 2;
            //phrase
            item.Text = "Phrase1";
            //control field
            item.SubItems.Add("0x29");
            //source address
            item.SubItems.Add("0x1102");
            //destination address
            item.SubItems.Add("0x1004");
            //TPCI
            item.SubItems.Add("0x0");
            //ACPI
            item.SubItems.Add("0x80");

            this.lvRecords.Items.Add(item);

            DataRow cat_row = (DataRow)this.tvCategories.SelectedNode.Tag;
            DataRow record_row = _ds.Tables["Record"].Rows.Add(item.Text,null,cat_row["Category_Id"]);
            DataRow telegram_row = _ds.Tables["EIBTelegram"].Rows.Add(0, 0, 0, 0, 0, record_row["Record_Id"]);

            item.Tag = record_row;

            if (this.lvRecords.SelectedItems.Count > 0)
            {
                this.btnRemoveRecord.Enabled = true;
            }
        }

        private void btnRemoveRecord_Click(object sender, EventArgs e)
        {
            if (this.lvRecords.SelectedItems == null || this.lvRecords.SelectedItems.Count == 0)
            {
                return;
            }

            ListViewItem[] items_to_remove = new ListViewItem[this.lvRecords.SelectedItems.Count];
            this.lvRecords.SelectedItems.CopyTo(items_to_remove, 0);
            foreach (ListViewItem item in items_to_remove)
            {
                DataRow record_row = (DataRow)item.Tag;
                _ds.Tables["Record"].Rows.Remove(record_row);
                this.lvRecords.Items.Remove(item);
            }

            if (this.lvRecords.Items.Count == 0)
            {
                this.btnRemoveRecord.Enabled = false;
            }
        }

        private void lvRecords_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (this.lvRecords.SelectedItems != null && this.lvRecords.SelectedItems.Count > 0)
            {
                this.btnRemoveRecord.Enabled = true;
            }
        }

        private void lvRecords_AfterLabelEdit(object sender, LabelEditEventArgs e)
        {
            if (e.Label != null)
            {
                DataRow record_row = (DataRow)this.lvRecords.Items[e.Item].Tag;
                record_row["Phrase"] = e.Label;
            }
        }

    }
}
