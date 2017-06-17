using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DummyClient
{
    public partial class LoginForm : Form
    {
        public LoginForm()
        {
            InitializeComponent();
        }

        private void button_login_Click(object sender, EventArgs e)
        {
            PK_C_REQ_ID_PW packet = new PK_C_REQ_ID_PW();
            packet.id_ = textBox_id_.Text;
            packet.password_ = textBox_pw_.Text;
            Program.programState_.sendPacket(packet);
        }

        private void button_quit(object sender, EventArgs e)
        {
            this.Dispose();
            Application.Exit();
        }
    }
}
