using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DummyClient
{
    static class PacketObfuscation
    {
        static Byte[] key_ = Encoding.ASCII.GetBytes(PacketMakeDate.stamp());
        
        static private void CalcXor(ref Byte[] packet, int packetOffset, int packetLen)
        {
            int keyIdx = packetOffset % key_.Length;
            for (int packetIdx = 0; packetIdx < packetLen; ++packetIdx)
            {
                packet[packetIdx] ^= key_[keyIdx++];
                if (keyIdx == key_.Length)
                {
                    keyIdx = 0;
                }
            }
        }

        static public void encodingHeader(ref Byte[] packet, int packetLen)
        {
            CalcXor(ref packet, 0, packetLen);
        }
        static public void encodingData(ref Byte[] packet, int packetLen)
        {
            CalcXor(ref packet, sizeof(Int32), packetLen);
        }

        static public void decodingHeader(ref Byte[] packet, int packetLen)
        {
            CalcXor(ref packet, 0, packetLen);
        }
        static public void decodingData(ref Byte[] packet, int packetLen)
        {
            CalcXor(ref packet, sizeof(Int32), packetLen);
        }
    }
}
