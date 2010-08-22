using System;
using System.Windows.Forms;
namespace EIBVoice
{
    public class Logger
    {
        TextBoxBase _target;
        object _sync = new object();

        public Logger(TextBoxBase target)
        {
            _target = target;
        }

        public void Log(string msg)
        {
            lock (_sync)
            {
                _target.Text += msg;
                _target.Text += Environment.NewLine;
            }
        }

        public void Clear()
        {
            lock (_sync)
            {
                _target.Text += string.Empty;
            }
        }
    }
}