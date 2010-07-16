using System;
using System.Collections.Generic;
using System.Text;
using System.Xml.Serialization;
using System.Xml;
using System.IO;
using EIBVoice.KNX;

namespace EIBVoice
{
    [XmlRoot("DBRoot")]
    public class MessagesDB
    {
        private List<SingleCategory> _categories;
        private bool _db_loaded = false;

        public MessagesDB()
        {
            _categories = new List<SingleCategory>();
        }

        [XmlIgnore()]
        public bool DBLoaded
        {
            get { return _db_loaded; }
            set { _db_loaded = value; }
        }

        [XmlElement("Category")]
        public List<SingleCategory> Categories
        {
            get { return _categories; }
            set { _categories = value; }
        }

        public void UnLoad()
        {
            _categories = null;
            _db_loaded = false;
        }

        public void Load(string file_name)
        {
            FileStream fs = File.Open(file_name, FileMode.Open, FileAccess.Read);
            XmlSerializer ser = new XmlSerializer(typeof(MessagesDB));
            try
            {
                MessagesDB mdb = (MessagesDB)ser.Deserialize(fs);
                _categories = mdb._categories;
                _db_loaded = true;
            }
            catch (Exception)
            {
                fs.Close();
                _db_loaded = false;
            }
            fs.Close();
        }

        public void Save(string file_name)
        {
            FileStream fs = File.Open(file_name, FileMode.Create, FileAccess.Write);
            XmlSerializer ser = new XmlSerializer(typeof(MessagesDB));
            ser.Serialize(fs, this);
        }
    }

    [XmlRoot("Category")]
    public class SingleCategory
    {
        private List<SingleRecord> _records;
        private string _name;

        public SingleCategory()
        {
            _records = new List<SingleRecord>();
        }

        [XmlElement("Record")]
        public List<SingleRecord> Records
        {
            get { return _records;}
            set { _records = value;}
        }

        [XmlAttribute("Name")]
        public string Name
        {
            get { return _name; }
            set { _name = value; }
        }
    }

    [XmlRoot("Record")]
    public class SingleRecord
    {
        private string _phrase;
        private EIBTelegram _telegram;

        public SingleRecord()
        {
            _telegram = new EIBTelegram();
        }

        [XmlElement("Phrase")]
        public string Phrase
        {
            get { return _phrase; }
            set { _phrase = value; }
        }

        [XmlElement("EIBTelegram")]
        public EIBTelegram Telegram
        {
            get { return _telegram; }
            set { _telegram = value; }
        }
    }
}
