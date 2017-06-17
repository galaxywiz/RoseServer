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
    public partial class ChattingForm : Form
    {
        private ChatWnd chatWnd_ = null;

        public ChattingForm()
        {
            InitializeComponent();
            Control.CheckForIllegalCrossThreadCalls = false;
            chatWnd_ = new ChatWnd(richTextBox_view_);
            Program.programState_.putMessage_ += new ProgramState.putMessageDele(pushText);
        }

        ~ChattingForm()
        {
            base.Dispose();
            chatWnd_ = null;
        }

        private void textBox_input_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                String inputStr = textBox_input_.Text + Environment.NewLine;
                richTextBox_view_.Text += inputStr;
                textBox_input_.Clear();

                PK_C_REQ_CHATTING packet = new PK_C_REQ_CHATTING();
                packet.text_ = inputStr;

                Program.programState_.sendPacket(packet);
                packet = null;
            }
            e.Handled = true;
        }

        public void pushText(string text)
        {
            chatWnd_.pushText(text);
        }
    }
}
