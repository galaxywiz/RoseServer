using System.Windows.Forms;

namespace DummyClient
{
    internal abstract class PacketProcess
    {
        public bool defaultRun(PacketInterface packet)
        {
            PacketType type = (PacketType)packet.type();
            //Todo : 공통 처리 패킷에 대한 정의
            //switch (type) {
            
            //}

            return false;
        }

        public abstract void run(PacketInterface packet);
    }
}