using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DummyClient
{
    class ChattingContents : ContentsProcess
    {
        public void recvChatting(PacketInterface rowPacket)
        {
            PK_S_ANS_CHATTING packet = (PK_S_ANS_CHATTING)rowPacket;
            Program.programState_.putMessage(packet.name_ + packet.text_);
        }
    }
}
