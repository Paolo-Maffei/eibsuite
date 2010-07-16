using System;
using System.Linq;
using System.Collections.Generic;
using System.Text;

namespace EIBMobile.Connection
{
    public class Cookie
    {
        private const string PATH_STR = "path";

        public Cookie(string raw)
        {
            try
            {
                Parse(raw);
            }
            catch(Exception)
            {
                Reset();
            }
        }

        private void Reset()
        {
        }

        private void Parse(string raw)
        {
            string[] tokens = raw.Split(';');

            for (int i = 0; i < tokens.Length; ++i)
            {
                if(tokens[i].StartsWith(PATH_STR))
                {
                    Path = tokens[i].Split('=')[1];
                }
                else
                {
                    Name = tokens[i].Split('=')[0];
                    Value = tokens[i].Split('=')[1];
                }
            }
        }

        public string Name { get; set; }
        public string Value { get; set; }
        public string Path { get; set; }
        public string Domain { get; set; }
        public DateTime Expires { get; set; }
        public bool HttpOnly { get; set; }
        public bool Expired
        {
            get
            {
                return (DateTime.Now < Expires);
            }
        }
    }
}
