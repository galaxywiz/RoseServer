using System.Threading;
using System.Windows.Forms;

namespace DummyClient
{
    internal abstract class FormState
    {
        protected Form form_;
        protected Network network_;

        public abstract void open(string ip, uint port);

        protected void setForm()
        {
            form_.TopLevel = false;
            form_.Dock = System.Windows.Forms.DockStyle.Fill;
            Program.mainForm_.Invoke(new MethodInvoker(delegate()
            {
                form_.Parent = Program.mainForm_.mainPanel_;
                form_.Show();
            }));
        }

        public void close()
        {
            Program.mainForm_.removePanelForm(ref form_);
            if (network_ != null)
                network_.disConnect();

            Thread.Sleep(1);
            form_.Hide();
        }

        public bool connectToServer(string ip, uint port)
        {
            if (network_ == null)
                network_ = new Network();

            return network_.connect(ip, port);
        }

        public void sendPacket(ref PacketInterface packet)
        {
            network_.sendPacket(packet);
        }
    }

    internal class LoginFormState : FormState
    {
        //HACK : 이곳 매직코드는 처음 접속하는
        //       로그인 서버 ip랑 port 번호임
        private string loginIp_ = "127.0.0.1";
        private uint loginPort_ = 9000;

        public override void open(string ip, uint port)
        {
            form_ = new LoginForm();
            setForm();
            if (!base.connectToServer(loginIp_, loginPort_)) {
                var result = MessageBox.Show("로그인 서버 연결에 실패. 다시 연결 시도 해볼까요?",
                                             "error", MessageBoxButtons.RetryCancel);
                if (result != DialogResult.Retry) {
                    form_.Close();
                    Application.Exit();
                }
            }
            network_.setPacketProcess(new LoginPacketProcess());
        }
    }

    internal class ChattingFormState : FormState
    {
        public override void open(string ip, uint port)
        {
            form_ = new ChattingForm();
            setForm();
            if (!base.connectToServer(ip, port)) {
                MessageBox.Show("채팅 서버 연결에 실패 했습니다..",
                                "error", MessageBoxButtons.OK);
                Program.programState_.setState(PROGRAM_STATE.LOGIN, null, 0);
            }
            network_.setPacketProcess(new ChattingPacketProcess());
        }
    }
}