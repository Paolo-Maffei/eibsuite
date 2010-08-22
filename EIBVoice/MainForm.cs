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
        private GeneralSettingsForm _gen_settings_form = new GeneralSettingsForm();
        private Logger _logger;

        private Font _nonrecognized_font = new Font("Microsoft Sans Serif", 14, FontStyle.Bold | FontStyle.Strikeout);
        private Font _recognized_font = new Font("Microsoft Sans Serif", 14, FontStyle.Bold);

        public MainForm()
        {
            InitializeComponent();
            //general
            this.button1.Tag = false;
            this.button1.Enabled = false;
            this.pboxVoiceStatus.Image = this.imageListVoiceStatus.Images[1];
            this.label2.Text = "";
            this.lblServerStatus.Text = "Disconneted";
            this.tsddbSettings.AutoToolTip = false;
            this._logger = new Logger(this.tbLog);

            DataSet ds = new DataSet();
            string s_file = "MessagesDB.xml";
            try
            {
                _logger.Log(String.Format("Loading Schema file: {0}...", s_file));
                ds.ReadXml(s_file);
                _logger.Log("Schema file loaded successfuly");
            }
            catch (Exception e)
            {
                _logger.Log(String.Format("Error in shcema loading: {0}. skipping.", e.Message));
            }
            
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
            _logger.Log("Initializing Client Module...");
            _server = new CGenericServerWrapper();
            _server.NetworkID = 1;
            _server.Init("Voice.log");
            _logger.Log("Client Module initialized successfuly");

            //voice
            bool error = false;
            _logger.Log("Initializing Voice recognition Module...");
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
                error = true;
                _logger.Log(String.Format("Error in Voice recognition module initialization: {0}. ", ae.Message));
                MessageBox.Show(ae.Message);
            }
            catch (Exception ex)
            {
                error = true;
                _logger.Log(String.Format("Error in Voice recognition module initialization: {0}. ", ex.Message));
                MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            if (!error)
            {
                _logger.Log("Voice recognition Module initialized successfuly");
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
                Cursor.Current = Cursors.WaitCursor;
                _logger.Log("Turning Voice recognition on.");
                ReadAloud("starting.");
                recognizer.RecognizeAsyncStop();
                recognizer.RecognizeAsync(RecognizeMode.Multiple);
                this.button1.Text = "Stop";
                this.pboxVoiceStatus.Image = this.imageListVoiceStatus.Images[2];
                Cursor.Current = Cursors.Arrow;
            }
            catch (Exception e)
            {
                Cursor.Current = Cursors.Arrow;
                MessageBox.Show(e.Message);
                return false;
            }
            return true;
        }

        private void TurnSpeechRecognitionOff()
        {
            if (recognizer != null)
            {
                Cursor.Current = Cursors.WaitCursor;
                _logger.Log("Turning Voice recognition off.");
                ReadAloud("stopping.");
                recognizer.RecognizeAsyncStop();
                this.button1.Text = "Start";
                this.pboxVoiceStatus.Image = this.imageListVoiceStatus.Images[0];
                Cursor.Current = Cursors.Arrow;
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
            if (_db[text] == null)
            {
                this.label2.ForeColor = Color.Red;
                this.label2.Font = _nonrecognized_font;
                this.label2.Text = text;
                return;
            }
            //text found in Data base
            this.label2.ForeColor = Color.Black;
            this.label2.Font = _recognized_font;
            this.label2.Text = text;
            //read the message + confirmed
            //ReadAloud(text + " confirmed");
            //send the telegram here...
            int res = _server.SendEIBNetwork(((KNX.EIBTelegram)_db[text]).DestAddress.RawAddress, ((KNX.EIBTelegram)_db[text]).APCI, CGenericServerWrapper.SendMode.NonBlocking);
            if (res == 0)
            {
                _logger.Log(String.Format("Sending command to {0} Failed.", ((KNX.EIBTelegram)_db[text]).DestAddress.ToString()));
            }
            else
            {
                _logger.Log(String.Format("Sending command to {0} Success.", ((KNX.EIBTelegram)_db[text]).DestAddress.ToString()));
            }
        }

        #endregion

        #region Text To Speech

        private void SetVoice()
        {
            try
            {
                synthesizer.SelectVoice("Microsoft Anna");
                synthesizer.Rate = -3;
            }
            catch (Exception)
            {
                _logger.Log("The synthetic voice is not available.  The default voice will be used instead.");
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
                this.label2.Text = string.Empty;
            }
            else
            {
                ((Button)sender).Tag = TurnSpeechRecognitionOn();
            }
        }

        private void btnLogin_Click(object sender, EventArgs e)
        {
            LoginDlg dlg = new LoginDlg(_logger);
            
            DialogResult dlgresult = dlg.ShowDialog();
            if (dlgresult == DialogResult.OK)
            {
                Cursor = Cursors.WaitCursor;
                _logger.Log("Trying to connect to EIB Server...");
                bool result = dlg.Login(_server);
                if (result)
                {
                    UpdateUI();
                    _logger.Log("Connection to EIB Server Success!");
                }
                else
                {
                    _logger.Log("Connection to EIB Server Failed!");
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
                    _logger.Log(String.Format("Loading XML data file: {0}", ofd.FileName));
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
                        this.pboxVoiceStatus.Image = this.imageListVoiceStatus.Images[0];
                        _logger.Log(String.Format("XML data file loaded successfuly"));
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

        private void generalToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (_gen_settings_form.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
            }
        }

    }
}
