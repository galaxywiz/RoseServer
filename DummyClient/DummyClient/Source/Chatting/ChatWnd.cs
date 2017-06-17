using System;
using System.Windows.Forms;

namespace DummyClient
{
    internal class ChatWnd
    {
        private enum TextView
        {
            LINE_MAX = 5000,
        }

        //--------------------------------------------//
        private RichTextBox outputView_;

        public ChatWnd(RichTextBox richTextBox)
        {
            this.outputView_ = richTextBox;
        }

        private void chattingMessageResize()
        {
            String str = outputView_.Text;
            int textLine = 0;

            while (true) {
                int idx = str.IndexOf(Environment.NewLine);
                if (idx < 0) {
                    break;
                }
                str = str.Substring(idx + Environment.NewLine.Length);
                ++textLine;
            }

            if (textLine > (int)TextView.LINE_MAX) {
                String originalStr = outputView_.Text;

                int idx = originalStr.IndexOf(Environment.NewLine);
                originalStr = originalStr.Substring(idx + Environment.NewLine.Length);

                outputView_.Text = originalStr;
            }
        }

        public void textChanged(object sender, EventArgs e)
        {
            chattingMessageResize();

            String str = outputView_.Text;
            int len = str.LastIndexOf('\n');
            if (len < 0) {
                return;
            }
            outputView_.SelectionStart = len;
            outputView_.ScrollToCaret();
        }

        public void pushText(string text)
        {
            if (outputView_ == null) {
                return;
            }
            outputView_.AppendText(text + Environment.NewLine);
            outputView_.ScrollToCaret();
        }
    }
}