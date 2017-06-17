using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DummyClient
{
    class LoginContents : ContentsProcess
    {
        public void S_ANS_ID_PW_FAIL(PacketInterface rowPacket)
        {
            MessageBox.Show("로그인 실패", "로그인", MessageBoxButtons.OK);
        }

        public void S_ANS_ID_PW_SUCCESS(PacketInterface rowPacket)
        {
            PK_S_ANS_ID_PW_SUCCESS packet = (PK_S_ANS_ID_PW_SUCCESS)rowPacket;
                        
            Program.programState_.setState(PROGRAM_STATE.CHATTING, packet.ip_, packet.port_);
            Program.programState_.setName(packet.name_);

            PK_C_REQ_REGIST_CHATTING_NAME rPacket = new PK_C_REQ_REGIST_CHATTING_NAME();
            rPacket.name_ = packet.name_;
            Program.programState_.sendPacket(rPacket);
        }
    }
}
