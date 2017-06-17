using System;
using System.IO;
using System.Text;

namespace DummyClient
{
    // 패킷의 구성 성분을 만든다
    public static class PacketUtil
    {
        //-------------------------------------------------------------------//
        //--- encodeing 부분
        public static void encodeHeader(MemoryStream packet, Int64 headerType)
        {
            PacketUtil.encode(packet, headerType);
        }

        public static void encode(MemoryStream packet, Byte value)
        {
            packet.Write(BitConverter.GetBytes(value), 0, sizeof(Byte));
        }

        public static void encode(MemoryStream packet, Char value)
        {
            packet.Write(BitConverter.GetBytes(value), 0, sizeof(Char));
        }

        public static void encode(MemoryStream packet, Int16 value)
        {
            packet.Write(BitConverter.GetBytes(value), 0, sizeof(Int16));
        }

        public static void encode(MemoryStream packet, UInt16 value)
        {
            packet.Write(BitConverter.GetBytes(value), 0, sizeof(UInt16));
        }

        public static void encode(MemoryStream packet, Int32 value)
        {
            packet.Write(BitConverter.GetBytes(value), 0, sizeof(Int32));
        }

        public static void encode(MemoryStream packet, UInt32 value)
        {
            packet.Write(BitConverter.GetBytes(value), 0, sizeof(UInt32));
        }

        public static void encode(MemoryStream packet, Int64 value)
        {
            packet.Write(BitConverter.GetBytes(value), 0, sizeof(Int64));
        }

        public static void encode(MemoryStream packet, UInt64 value)
        {
            packet.Write(BitConverter.GetBytes(value), 0, sizeof(UInt64));
        }

        public static void encode(MemoryStream packet, string str)
        {
            PacketUtil.encode(packet, (Int32)str.Length);
            packet.Write(Encoding.UTF8.GetBytes(str), 0, str.Length);
        }
        //-------------------------------------------------------------------//
        //---decoding 부분
        public static Int32 decodePacketLen(Byte[] data, ref Int32 offset)
        {
            return PacketUtil.decodeInt32(data, ref offset);
        }

        public static Int64 decodePacketType(Byte[] data, ref Int32 offset)
        {
            return PacketUtil.decodeInt64(data, ref offset);
        }

        public static Byte decodeByte(Byte[] data, ref Int32 offset)
        {
            Byte val = data[offset];
            offset += sizeof(Byte);
            return val;
        }

        public static Char decodeInt8(Byte[] data, ref Int32 offset)
        {
            Char val = BitConverter.ToChar(data, offset);
            offset += sizeof(Char);
            return val;
        }

        public static Int16 decodeInt16(Byte[] data, ref Int32 offset)
        {
            Int16 val = BitConverter.ToInt16(data, offset);
            offset += sizeof(Int16);
            return val;
        }

        public static UInt16 decodeUInt16(Byte[] data, ref Int32 offset)
        {
            UInt16 val = BitConverter.ToUInt16(data, offset);
            offset += sizeof(UInt16);
            return val;
        }

        public static Int32 decodeInt32(Byte[] data, ref Int32 offset)
        {
            Int32 val = BitConverter.ToInt32(data, offset);
            offset += sizeof(Int32);
            return val;
        }

        public static UInt32 decodeUInt32(Byte[] data, ref Int32 offset)
        {
            UInt32 val = BitConverter.ToUInt32(data, offset);
            offset += sizeof(UInt32);
            return val;
        }

        public static Int64 decodeInt64(Byte[] data, ref Int32 offset)
        {
            Int64 val = BitConverter.ToInt64(data, offset);
            offset += sizeof(Int64);
            return val;
        }

        public static UInt64 decodeUInt64(Byte[] data, ref Int32 offset)
        {
            UInt64 val = BitConverter.ToUInt64(data, offset);
            offset += sizeof(UInt64);
            return val;
        }

        public static string decodestring(Byte[] data, ref Int32 offset)
        {
            Int32 strLen = PacketUtil.decodeInt32(data, ref offset);
            string str = System.Text.Encoding.ASCII.GetString(data, offset, strLen);
            offset += strLen;
            return str;
        }

        public static PacketInterface packetAnalyzer(Byte[] packetByte)
        {
            Int32 offset = 0;
            Int64 packetType = PacketUtil.decodePacketType(packetByte, ref offset);
            PacketInterface packet = PacketFactory.getPacket(packetType);
            if (packet == null) {
                return null;
            }

            // 데이터가 있으면 decoding 해서 넘기기
            if (offset < packetByte.Length) {
                packet.decode(packetByte, ref offset);
            }
            return packet;
        }
    }
}