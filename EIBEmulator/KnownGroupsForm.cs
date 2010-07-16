using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using GlacialComponents.Controls;

namespace EIBEmulator
{
    public partial class KnownGroupsForm : Form
    {
        public KnownGroupsList Groups { get; set; }
        
        public KnownGroupsForm()
        {
            InitializeComponent();
            this.btnRemoveGroup.Enabled = false;
            this.lvGroups.SelectedIndexChanged +=new GlacialList.ClickedEventHandler(lvGroups_SelectedIndexChanged);
        }

        

        public void LoadData()
        {
            if (Groups == null)
            {
                return;
            }
            foreach (var item in Groups.List)
	        {
                GLItem i = new GLItem();
                GLSubItem si_addr = new GLSubItem();
                si_addr.Text = item.Address;
                si_addr.ChangedEvent += new ChangedEventHandler(subItemAddr_ChangedEvent);
                GLSubItem si_len = new GLSubItem();
                si_len.ChangedEvent += new ChangedEventHandler(subItemLen_ChangedEvent);
                si_len.Text = item.NumBytes.ToString();
                i.SubItems.Add(si_addr);
                i.SubItems.Add(si_len);
                i.Tag = item;
                this.lvGroups.Items.Add(i);
	        }
            
        }

        private void btnAddGroup_Click(object sender, EventArgs e)
        {
            GLItem i = new GLItem();
            GLSubItem si_addr = new GLSubItem();
            si_addr.Text = "0/0/0";
            si_addr.ChangedEvent += new ChangedEventHandler(subItemAddr_ChangedEvent);
            
            GLSubItem si_len = new GLSubItem();
            si_len.Text = "1";
            si_len.ChangedEvent += new ChangedEventHandler(subItemLen_ChangedEvent);

            i.SubItems.Add(si_addr);
            i.SubItems.Add(si_len);
            KnownGroup kg = new KnownGroup();
            kg.Address = "0/0/0";
            kg.NumBytes = 1;
            i.Tag = kg;
            this.lvGroups.Items.Add(i);
        }

        void subItemLen_ChangedEvent(object source, ChangedEventArgs e)
        {
            if (lvGroups.SelectedItems.Count == 0)
                return;
            KnownGroup kg = (KnownGroup)(((GLItem)lvGroups.SelectedItems[0]).Tag);
            GLSubItem si = (GLSubItem)source;
            kg.NumBytes = Int32.Parse(si.Text);
            
        }

        void subItemAddr_ChangedEvent(object source, ChangedEventArgs e)
        {
            if (lvGroups.SelectedItems.Count == 0)
                return;
            KnownGroup kg = (KnownGroup)(((GLItem)lvGroups.SelectedItems[0]).Tag);
            GLSubItem si = (GLSubItem)source;
            kg.Address = si.Text;
        }

        private void lvGroups_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (lvGroups.SelectedItems == null || lvGroups.SelectedItems.Count != 1)
            {
                this.btnRemoveGroup.Enabled = false;
            }
            else
            {
                this.btnRemoveGroup.Enabled = true;
            }
        }

        private void btnRemoveGroup_Click(object sender, EventArgs e)
        {
            if (lvGroups.SelectedItems != null && lvGroups.SelectedItems.Count == 1)
            {
                GLItem item = (GLItem)lvGroups.SelectedItems[0];
                lvGroups.Items.Remove(item);
            }
        }

        private void btnApply_Click(object sender, EventArgs e)
        {
            this.Groups = new KnownGroupsList();
            foreach (var item in lvGroups.Items)
            {
                GLItem i = (GLItem)item;
                KnownGroup kg = (KnownGroup)i.Tag;
                this.Groups.List.Add(kg);
            }
        }

        private void KnownGroupsForm_Load(object sender, EventArgs e)
        {
            LoadData();
        }
    }
}
