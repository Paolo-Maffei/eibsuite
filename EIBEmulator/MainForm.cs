using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Net.Sockets;
using System.Net;
using System.IO;
using System.Threading;
using System.Net.NetworkInformation;
using System.Text.RegularExpressions;
using System.Xml.Serialization;
using System.Xml;


namespace EIBEmulator
{
    public partial class MainForm : Form
    {
        private const int DEFAULT_EIB_PORT = 3671;
        private const string DEFAULT_MULTICAST_ADDRESS = "224.0.23.12";
        private const string KNOWN_GROUP_FILE = "groups.xml";
        private Socket _sock;
        EndPoint _local_endpoint;
        EndPoint _remote_data_endpoint = null;
        bool _connected = false;
        byte _recv_sequence;
        byte _send_sequence;
        DEVICE_MODE _mode = DEVICE_MODE.MODE_TUNNELING;
        NetworkInterface _current_if = null;
        private object _syncObj = new object();
        private bool _restart_now = false;
        Regex hex_regex = new Regex(@"^[A-Fa-f0-9]*$",RegexOptions.Compiled);

        public MainForm()
        {
            InitializeComponent();

            this.btnDeviceOff.Enabled = false;
            this.gbPktGenerator.Enabled = false;

            this.ListenerThread.DoWork += new DoWorkEventHandler(ListenerThread_DoWork);
            this.ListenerThread.ProgressChanged += new ProgressChangedEventHandler(ListenerThread_ProgressChanged);
            this.ListenerThread.RunWorkerCompleted += new RunWorkerCompletedEventHandler(ListenerThread_RunWorkerCompleted);
            this.netifsComboBox.SelectedIndexChanged += new EventHandler(netifsComboBox_SelectedIndexChanged);

            this.netifsComboBox.Items.Clear();
            
            NetworkInterface[] interfaces = NetworkInterface.GetAllNetworkInterfaces();
            foreach (NetworkInterface n in interfaces)
            {
                foreach (UnicastIPAddressInformation info in n.GetIPProperties().UnicastAddresses)
                {
                    if (info.Address.AddressFamily == AddressFamily.InterNetwork)
                    {
                        this.netifsComboBox.Items.Add(n.Description);
                        break;
                    }
                }
            }
        }

        void netifsComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            NetworkInterface[] interfaces = NetworkInterface.GetAllNetworkInterfaces();
            foreach (NetworkInterface n in interfaces)
            {
                if (this.netifsComboBox.SelectedItem.ToString() == n.Description)
                {
                    foreach (UnicastIPAddressInformation info in n.GetIPProperties().UnicastAddresses)
                    {
                        if (info.Address.AddressFamily == AddressFamily.InterNetwork)
                        {
                            this.ipAddressControl1.Text = info.Address.ToString();
                            break;
                        }
                    }
                    _current_if = n;
                    break;
                }
            }
        }

        private bool InitDevice()
        {
            if (_current_if == null)
            {
                MessageBox.Show("Please select network interface first", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }
            
            if (rbModeRouting.Checked)
            {
                _mode = DEVICE_MODE.MODE_ROUTING;
            }
            if (rbModeTunneling.Checked)
            {
                _mode = DEVICE_MODE.MODE_TUNNELING;
            }

            if(_sock != null && _sock.IsBound)
                _sock.Close();

            string local_addr = this.ipAddressControl1.Text;
            _local_endpoint = new IPEndPoint(IPAddress.Parse(local_addr), (int)this.numericUpDown1.Value);
            _sock = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.IP);
            _sock.Bind(_local_endpoint);

            switch (_mode)
            {
                case DEVICE_MODE.MODE_ROUTING:
                    IPAddress multicast_addr = IPAddress.Parse(DEFAULT_MULTICAST_ADDRESS);
                    MulticastOption option = new MulticastOption(multicast_addr);
                    _sock.SetSocketOption(SocketOptionLevel.IP, SocketOptionName.AddMembership, option);
                    _remote_data_endpoint = new IPEndPoint(multicast_addr, DEFAULT_EIB_PORT);
                    this.ipAddressControl1.Text = DEFAULT_MULTICAST_ADDRESS;
                    break;
                case DEVICE_MODE.MODE_TUNNELING:
                    IPAddress maddr = IPAddress.Parse(DEFAULT_MULTICAST_ADDRESS);
                    MulticastOption opt = new MulticastOption(maddr);
                    _sock.SetSocketOption(SocketOptionLevel.IP, SocketOptionName.AddMembership, opt);
                    _recv_sequence = 0;
                    _send_sequence = 0;
                    this.ipAddressControl1.Text = local_addr;
                    break;
            }

            return true;
        }

        void ListenerThread_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            this.btnDeviceOff.Enabled = false;
            this.btnDeviceOn.Enabled = true;
            this.gbPktGenerator.Enabled = false;
            this.gbDeviceMode.Enabled = true;

            _sock.Close();

            Cursor = Cursors.Default;

            if (_restart_now)
            {
                _restart_now = false;
                InitDevice();
            }
        }

        void ListenerThread_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            this.textBox1.Text += (string)e.UserState;
        }

        private void btnDeviceOff_Click(object sender, EventArgs e)
        {
            Cursor = Cursors.WaitCursor;
            DisconnectRequest req = new DisconnectRequest(new HPAI(_remote_data_endpoint));
            try
            {
                _sock.SendTo(req.ToByteArray(), _remote_data_endpoint);
            }
            catch { }
            this.ListenerThread.CancelAsync();
        }

        private void LogSend(string msg)
        {
            Log("Sent", msg);
        }

        private void LogReceive(string msg)
        {
            Log("Received", msg);
        }

        private void Log(string prefix,string msg)
        {
            string res = string.Empty;
            //res += '[' + DateTime.Now.ToString() + ']';
            res += '[' + prefix + ']';
            res += " : ";
            res += msg;
            res += "\r\n";
            this.ListenerThread.ReportProgress(0, res);
        }

        private static byte[] StringToByteArray(string hex)
        {
            if (hex.Length % 2 == 1)
            {
                hex = '0' + hex;
            }

            int NumberChars = hex.Length;
            byte[] bytes = new byte[NumberChars / 2];
            for (int i = 0; i < NumberChars; i += 2)
                bytes[i / 2] = Convert.ToByte(hex.Substring(i, 2), 16);
            return bytes;
        }

        private void btnSendPkt_Click(object sender, EventArgs e)
        {
            if (_remote_data_endpoint == null)
            {
                MessageBox.Show("No EIB Client is connected. cannot sent packets.", "Error", MessageBoxButtons.OK);
                return;
            }

            if (this.tbValue.Text == null || this.tbValue.Text.Length == 0)
            {
                MessageBox.Show("No value was provided. you must enter the cEMI frame value to send", "Error", MessageBoxButtons.OK);
                return;
            }
            if (!hex_regex.IsMatch(this.tbValue.Text))
            {
                MessageBox.Show("The cEMI frame value is not valid HEX format.", "Error", MessageBoxButtons.OK);
                return;
            }

            for (int i = 0; i < this.nudNumPkts.Value; ++i)
            {
                EIBAddress dst_addr;
                EIBAddress src_addr = new EIBAddress(this.tbSrcAddress.Text);
                if (this.checkBox1.Checked)
                    dst_addr = EIBAddress.GetRandomAddress();
                else
                    dst_addr = new EIBAddress(this.tbDestAddress.Text);
                CemiFrame frame = new CemiFrame(src_addr,
                                                dst_addr,
                                                StringToByteArray(this.tbValue.Text),
                                                cbNeedAck.Checked);
                byte[] data = null;
                switch (_mode)
                {
                    case DEVICE_MODE.MODE_TUNNELING:
                        TunnelRequest req = new TunnelRequest(frame, _send_sequence);
                        data = req.ToByteArray();
                        break;
                    case DEVICE_MODE.MODE_ROUTING:
                        RoutingIndication roi = new RoutingIndication(frame);
                        data = roi.ToByteArray();
                        break;
                }

                _sock.SendTo(data, _remote_data_endpoint);
                LogSend(String.Format("Data packet. Dest addr: {0} Value 0x{1}", dst_addr.ToString(), this.tbValue.Text));
                System.Threading.Thread.Sleep(10);
            }
        }

        private void btnDeviceOn_Click(object sender, EventArgs e)
        {
            if (InitDevice())
            {
                this.btnDeviceOn.Enabled = false;
                this.btnDeviceOff.Enabled = true;
                this.gbPktGenerator.Enabled = true;
                this.gbDeviceMode.Enabled = false;

                //start listener thread
                this.ListenerThread.RunWorkerAsync();
            }
        }

        void ListenerThread_DoWork(object sender, DoWorkEventArgs e)
        {
            EndPoint endpoint = new IPEndPoint(IPAddress.Loopback, 0);
            TunnelRequest orig_req = null;
            byte[] buffer = new byte[256];
            bool send_ind = false;
            int i = 0;

            while (!this.ListenerThread.CancellationPending)
            {
                if (!_sock.Poll(1000000, SelectMode.SelectRead))
                {
                    continue;
                }
                try
                {
                    _sock.ReceiveFrom(buffer, ref endpoint);
                }
                catch (SocketException ex)
                {
                    this.Log("Socket Exception", ex.Message);
                    return;
                }

                MemoryStream stream = new MemoryStream(buffer);
                send_ind = SendReply(stream);
                if (send_ind || i > 0)
                {
                    if (send_ind)
                    {
                        stream.Position = 0;
                        orig_req = new TunnelRequest(stream);
                    }
                    i++;
                }

                if (_mode == DEVICE_MODE.MODE_TUNNELING && i == 2)
                {
                    i = 0;
                    TunnelRequest tunnel_request = new TunnelRequest(orig_req._frame, _send_sequence);
                    tunnel_request._frame._mc = EIBMessages.MC_LDATA_IND;
                    tunnel_request._frame._apci = 0x41;
                    if (_cache != null)
                    {
                        lock (_sync)
                        {
                            foreach (var item in _cache.List)
                            {
                                if (tunnel_request._frame._dst.ToString() == item.Address && item.NumBytes > 1)
                                {
                                    tunnel_request._frame._addil_data = new byte[item.NumBytes - 1];
                                    tunnel_request._frame._apci_length = (byte)item.NumBytes;
                                    break;
                                }
                            }
                        }
                    }

                    _sock.SendTo(tunnel_request.ToByteArray(), _remote_data_endpoint);
                    LogSend(String.Format("Tunnel Data indication: {0} Value length: {1}", 
                        tunnel_request._frame._dst.ToString(), tunnel_request._frame._apci_length));
                }

                if (!_connected && _mode != DEVICE_MODE.MODE_ROUTING)
                {
                    break;
                }
            }
        }

        private bool SendReply(Stream s)
        {
            EIBHeader header = new EIBHeader(s);
            s.Position = 0;
            bool res = false;
            switch (header.mc)
            {
                case EIBMessages.EIB_MC_CONNECT_REQUEST:
                    ConnectRequest creq = new ConnectRequest(s);
                    _remote_data_endpoint = creq.data_endpoint.endpoint;
                    LogReceive("Connect Request");
                    ConnectRepsonse cresp = new ConnectRepsonse(_local_endpoint);
                    _sock.SendTo(cresp.ToByteArray(),creq.control_endpoint.endpoint);
                    LogSend("Connect Response");
                    _connected = true;
                    break;
                case EIBMessages.EIB_MC_CONNECTION_STATE_REQUEST:
                    ConnectionStateRequest csreq = new ConnectionStateRequest(s);
                    LogReceive("Connection State Request");
                    ConnectionStateResponse csresp = new ConnectionStateResponse();
                    _sock.SendTo(csresp.ToByteArray(), csreq.control_endpoint.endpoint);
                    LogSend("Connection State Response");
                    break;
                case EIBMessages.EIB_MC_DISCONNECT_REQUEST:
                    DisconnectRequest disreq = new DisconnectRequest(s);
                    LogReceive("Disconnect Request");
                    DisconnectResponse disresp = new DisconnectResponse();
                    _sock.SendTo(disresp.ToByteArray(), disreq.control_endpoint.endpoint);
                    LogSend("Dissconnect Response");
                    _restart_now = true;
                    _connected = false;
                    break;
                case EIBMessages.EIB_MC_SEARCH_REQUEST:
                    LogReceive("Search Request");
                    SearchRequest sreq = new SearchRequest(s);
                    SearchResponse sresp = new SearchResponse(_local_endpoint);
                    _sock.SendTo(sresp.ToByteArray(), sreq._discovery_endpoint.endpoint);
                    LogSend("Search Response");
                    _connected = true;
                    break;
                case EIBMessages.EIB_MC_TUNNEL_REQUEST:
                    TunnelRequest req = new TunnelRequest(s);
                    if (req._frame._apci == 0x0)
                    {
                        string log = String.Format("[Group Read Request] Dest: {0}", req._frame._dst.ToString());
                        LogReceive(log);
                    }
                    else
                    {
                        string log = String.Format("[Tunnel Request] Dest: {0}, Value: 0x{1}", req._frame._dst.ToString(), String.Format("{0:X}", req._frame._apci));
                        LogReceive(log);
                    }
                    if (req._frame.IsAckNeeded)
                    {
                        TunnelAck ack = new TunnelAck(_recv_sequence);
                        _sock.SendTo(ack.ToByteArray(), _remote_data_endpoint);
                        ++_recv_sequence;
                        LogSend("Tunnel Ack");
                    }
                    TunnelRequest tunnel_request = new TunnelRequest(req._frame, _send_sequence);
                    tunnel_request._frame._mc = EIBMessages.L_DATA_CON;
                    if (_cache != null)
                    {
                        lock (_sync)
                        {
                            foreach (var item in _cache.List)
                            {
                                if (tunnel_request._frame._dst.ToString() == item.Address && item.NumBytes > 1)
                                {
                                    tunnel_request._frame._addil_data = new byte[item.NumBytes - 1];
                                    tunnel_request._frame._apci_length = (byte)item.NumBytes;
                                    break;
                                }
                            }
                        }
                    }
                    if (req._frame._apci == 0x0)
                    {
                        res = true;
                    }
                    LogSend(String.Format("Tunnel Data confirmation: {0}", tunnel_request._frame._dst.ToString()));
                    _sock.SendTo(tunnel_request.ToByteArray(), _remote_data_endpoint);
                    break;
                case EIBMessages.EIB_MC_TUNNEL_ACK:
                    LogReceive("Ack");
                    ++_send_sequence;
                    break;
                case EIBMessages.EIB_MC_ROUTING_INDICATION:
                    LogReceive("routing indication");
                    break;
                default:
                    LogReceive("Unknown message code!");
                    break;
                    //throw new NotImplementedException();
            }
            return res;
        }

        private void btnClearLog_Click(object sender, EventArgs e)
        {
            this.textBox1.Text = string.Empty;
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            this.tbDestAddress.Enabled = !this.checkBox1.Checked;
        }

        private void btnEditKnownGroups_Click(object sender, EventArgs e)
        {
            KnownGroupsList res = LoadKnownGroupsFile();
            KnownGroupsForm f = new KnownGroupsForm();
            f.Groups = res;
            DialogResult dr = f.ShowDialog();

            if (dr == System.Windows.Forms.DialogResult.OK)
            {
                res = f.Groups;
                lock (_sync)
                {
                    _cache = res;
                }
                SaveKnownGroupsToFile(f.Groups);
            }
        }

        private void LoadKnownGroupsToCache()
        {
            KnownGroupsList res = LoadKnownGroupsFile();
            lock (_sync)
            {
                _cache = res;
            }
        }

        private object _sync = new object();
        private KnownGroupsList _cache;

        private void SaveKnownGroupsToFile(KnownGroupsList list)
        {
            XmlWriterSettings settings = new XmlWriterSettings();
            settings.Indent = true;
            FileStream fs = new FileStream(KNOWN_GROUP_FILE, FileMode.OpenOrCreate);
            XmlWriter writer = XmlWriter.Create(fs, settings);
            XmlSerializer ser = new XmlSerializer(typeof(KnownGroupsList));
            ser.Serialize(writer, _cache);
            writer.Flush();
            fs.Close();
        }

        private KnownGroupsList LoadKnownGroupsFile()
        {
            FileStream fs = null;
            XmlTextReader reader = null;
            try
            {
                fs = new FileStream(KNOWN_GROUP_FILE, FileMode.Open);
                reader = new XmlTextReader(fs);
                XmlSerializer ser = new XmlSerializer(typeof(KnownGroupsList));
                KnownGroupsList res = (KnownGroupsList)ser.Deserialize(reader);
                reader.Close();
                return res;
            }
            catch
            {
                return null;
            }
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            LoadKnownGroupsToCache();
        }
    }

    [XmlRoot("KnownGroupsList")]
    public class KnownGroupsList
    {
        [XmlElement("Item")]
        public List<KnownGroup> List;

        public KnownGroupsList()
        {
            List = new List<KnownGroup>();
        }
    }

    [XmlRoot("Item")]
    public class KnownGroup
    {
        [XmlAttribute("Length")]
        public int NumBytes { get; set; }

        [XmlAttribute("Address")]
        public String Address { get; set; }
    }
}