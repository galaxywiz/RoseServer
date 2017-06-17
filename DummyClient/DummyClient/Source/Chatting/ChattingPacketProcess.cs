using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DummyClient
{
    class ChattingPacketProcess : PacketProcess
    {
        ChattingContents contents_ = null;

        public ChattingPacketProcess()
        {
            contents_ = new ChattingContents();
        }

        public override void run(PacketInterface packet)
        {
            PacketType type = (PacketType)packet.type();
            switch (type) {
            case PacketType.E_S_ANS_CHATTING:
                contents_.recvChatting(packet);
                return;
            }

            if (base.defaultRun(packet) == false) {
#if DEBUG
                MessageBox.Show("잘못된 패킷이 수신되었습니다 : " + type.ToString(), "error", MessageBoxButtons.OK);
                Application.Exit();
#endif
            }
        }
    }
}
