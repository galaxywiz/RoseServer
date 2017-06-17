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
    partial class DummyClient : Form
    {
        public DummyClient()
        {
            InitializeComponent();
            Control.CheckForIllegalCrossThreadCalls = false;
        }

        private void DummyClient_Shown(object sender, EventArgs e)
        {
            Program.programState_.setState(PROGRAM_STATE.LOGIN, null, 0);
        }

        private void DummyClient_FormClosing(object sender, FormClosingEventArgs e)
        {
            Program.programState_.close();
            this.Dispose();
            Application.Exit();
        }

        public void removePanelForm(ref Form form)
        {
            mainPanel_.Controls.Remove(form);
        }
    }
}
