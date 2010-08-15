using System;
using System.IO;
using System.Collections.Generic;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Speech.Synthesis;
using System.Speech.Recognition;
using EIBNetWrapper;
using EIBVoice.Properties;
using System.Xml;
using System.Xml.Schema;

namespace EIBVoice
{
    public partial class MainForm : Form
    {
        private SpeechRecognitionEngine recognizer = null;
        private SpeechSynthesizer synthesizer = null;
        private CGenericServerWrapper _server = null;
        private bool _db_loaded = false;
        private Hashtable _db = new Hashtable();
        private EventHandler<LoadGrammarCompletedEventArgs> grammer_completed_handler = null;
        private ConfigMessages _cm_form = new ConfigMessages();
        //private TextBoxWriter _writer;

        public MainForm()
        {
            InitializeComponent();
            //general
            this.button1.Tag = false;
            this.button1.Enabled = false;
            this.label1.Text = "Speech Recognition: OFF";
            this.label2.Text = "";
            this.lblServerStatus.Text = "Disconneted";
            this.tsddbSettings.AutoToolTip = false;

            //_writer = new TextBoxWriter(this.tbLog);
            //Console.SetOut(_writer);
            
            DataSet ds = new DataSet();
            try
            {
                ds.ReadXml("MessagesDB.xml");
            }
            catch { }
            
            grammer_completed_handler = new EventHandler<LoadGrammarCompletedEventArgs>(this.recognizer_LoadGrammarCompleted);            
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            this.splitContainer1.Dock = DockStyle.Fill;
            this.splitContainer2.Dock = DockStyle.Fill;
            this.lvMessages.Dock = DockStyle.Fill;
            this.tbLog.Dock = DockStyle.Fill;
            this.btnDisconnect.Enabled = false;
            
            //generic server
            _server = new CGenericServerWrapper();
            _server.NetworkID = 1;
            _server.Init("Voice.log");

            //voice
            recognizer = new SpeechRecognitionEngine();
            synthesizer = new SpeechSynthesizer();
            
            System.Collections.ObjectModel.ReadOnlyCollection<InstalledVoice> voices = synthesizer.GetInstalledVoices();
            try
            {
                recognizer.SetInputToDefaultAudioDevice();
                recognizer.UnloadAllGrammars();
                recognizer.SpeechRecognized += new EventHandler<SpeechRecognizedEventArgs>(recognizer_SpeechRecognized);
                SetVoice();
            }
            catch (ArgumentException ae)
            {
                MessageBox.Show(ae.Message);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void MainForm_Shown(object sender, EventArgs e)
        {
            this.btnLogin_Click(null, null);
        }

        private void richTextBox_TextChanged(object sender, EventArgs e)
        {
            this.tbLog.SelectionStart = this.tbLog.Text.Length;
            this.tbLog.ScrollToCaret();
        }

        public void ShowCompileErrors(object sender, ValidationEventArgs args)
        {
            MessageBox.Show(args.Message, "XML File error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            _db_loaded = false;
        }

        #region Speech Recognition

        private bool TurnSpeechRecognitionOn()
        {
            try
            {
                ReadAloud("starting.");
                recognizer.RecognizeAsyncStop();
                recognizer.RecognizeAsync(RecognizeMode.Multiple);
                this.button1.Text = "Stop";
                this.label1.Text = "Speech Recognition: ON";
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message);
                return false;
            }
            return true;
        }

        private void TurnSpeechRecognitionOff()
        {
            if (recognizer != null)
            {
                ReadAloud("sleeping.");
                recognizer.RecognizeAsyncStop();
                this.button1.Text = "Start";
                this.label1.Text = "Speech Recognition: OFF";
            }
        }

        private void recognizer_SpeechRecognized(object sender, SpeechRecognizedEventArgs e)
        {
            string text = e.Result.Text;
            SpeechToAction(text);
        }

        void recognizer_LoadGrammarCompleted(object sender, LoadGrammarCompletedEventArgs e)
        {
            recognizer.LoadGrammarCompleted -= grammer_completed_handler;
            this.button1.Enabled = true;
        }

        private void SpeechToAction(string text)
        {
            if (_db == null) return;
            if (_db[text] == null) return;
            //text found in Data base
            this.label2.Text = text;
            //read the message + confirmed
            //ReadAloud(text + " confirmed");
            //send the telegram here...
            _server.SendEIBNetwork(((KNX.EIBTelegram)_db[text]).DestAddress.RawAddress, ((KNX.EIBTelegram)_db[text]).APCI, CGenericServerWrapper.SendMode.NonBlocking);
        }

        #endregion

        #region Text To Speech

        private void SetVoice()
        {
            try
            {
                synthesizer.SelectVoice("Microsoft Anna");
                synthesizer.Rate = -3;
                Console.WriteLine("Voice Libaray Initialized successfully.");
            }
            catch (Exception)
            {
                MessageBox.Show("The synthetic voice is not available.  The default voice will be used instead.");
            }
        }

        private void ReadAloud(string speakText)
        {
            try
            {
                SetVoice();
                synthesizer.Speak(speakText);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

        }

        #endregion

        #region Buttons Handlers

        private void btnStartListen_Click(object sender, EventArgs e)
        {
            if ((bool)((Button)sender).Tag)
            {
                ((Button)sender).Tag = false;
                TurnSpeechRecognitionOff();
                this.label2.Text = "";
                Console.WriteLine("Voice recognition turned off");
            }
            else
            {
                ((Button)sender).Tag = TurnSpeechRecognitionOn();
                Console.WriteLine("Voice recognition turned on");
            }
        }

        private void btnLogin_Click(object sender, EventArgs e)
        {
            LoginDlg dlg = new LoginDlg();
            
            DialogResult dlgresult = dlg.ShowDialog();
            if (dlgresult == DialogResult.OK)
            {
                Cursor = Cursors.WaitCursor;
                Console.WriteLine("Trying to connect to EIB Server...");
                bool result = dlg.Login(_server);
                if (result)
                {
                    UpdateUI();
                    Console.WriteLine("Connection to EIB Server Success!");
                }
                else
                {
                    Console.WriteLine("Connection to EIB Server Failed!");
                }
                Cursor = Cursors.Default;
            }
        }

        private void btnDisconnect_Click(object sender, EventArgs e)
        {
            _server.Close();
        }

        private void btnConfigMsgs_Click(object sender, EventArgs e)
        {
            _cm_form.Show();
        }

        private void btnDBFile_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.DefaultExt = "xml";
            ofd.Filter = "XML files (*.xml)|*.xml";
            if (ofd.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    _db_loaded = true;
                    //run the reader. 
                    XmlReaderSettings set = new XmlReaderSettings();
                    set.ValidationEventHandler += new ValidationEventHandler(this.ShowCompileErrors);
                    set.Schemas.Add(null, XmlReader.Create(Settings.Default.XMLSchemaFile));
                    set.ValidationType = ValidationType.Schema;

                    XmlReader reader = XmlReader.Create(ofd.FileName, set);

                    while (reader.Read()) { }
                    reader.Close();
                    _db.Clear();
                    if (_db_loaded)
                    {
                        this.lvMessages.Items.Clear();
                        MessagesDB db = new MessagesDB();
                        db.Load(ofd.FileName);
                        foreach (SingleCategory cat in db.Categories)
                        {
                            foreach (SingleRecord record in cat.Records)
                            {
                                _db[record.Phrase] = record.Telegram;
                                string confirmed_phrase = record.Phrase + " confirmed";
                                _db[confirmed_phrase] = null;
                                //Update the Main screen
                                ListViewItem item = new ListViewItem();
                                item.Text = record.Phrase;
                                this.lvMessages.Items.Add(item);
                            }
                        }

                        LoadGrammer();
                    }
                }
                catch (XmlException)
                {

                }
                catch (XmlSchemaException)
                {

                }
                catch (Exception)
                {

                }
            }
        }

        #endregion

        #region Private Methods

        protected override void OnPaint(PaintEventArgs e)
        {
            UpdateUI();
            base.OnPaint(e);
        }
        
        private void LoadGrammer()
        {
            GrammarBuilder grammarBuilder = new GrammarBuilder();
            string[] phrases = new string[_db.Keys.Count];
            _db.Keys.CopyTo(phrases, 0);
            grammarBuilder.Append(new Choices(phrases));
            Grammar customGrammar = new Grammar(grammarBuilder);
            recognizer.LoadGrammarCompleted += grammer_completed_handler;
            recognizer.UnloadAllGrammars();
            recognizer.LoadGrammarAsync(customGrammar);
        }

        private void Error(string error)
        {
            MessageBox.Show(error, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error, MessageBoxDefaultButton.Button1);
        }

        private void Warning(string warning)
        {
            MessageBox.Show(warning, "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning, MessageBoxDefaultButton.Button1);
        }

        private void UpdateUI()
        {
            if (_server != null && _server.IsConnected())
            {
                lblServerStatus.Text = "Connected";
                this.btnDisconnect.Enabled = true;
                this.btnLogin.Enabled = false;
            }
            else
            {
                lblServerStatus.Text = "Disconnected";
                this.btnDisconnect.Enabled = false;
                this.btnLogin.Enabled = true;
            }
        }

        #endregion 
    }
}
