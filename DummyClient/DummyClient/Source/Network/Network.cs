using System;
using System.IO;
using System.Net.Sockets;
using System.Threading;
using System.Windows.Forms;

namespace DummyClient
{
    internal enum NET_STATE
    {
        START,
        CONNECTED,
        DISCONNECT,
        DISCONNECTED,
    }

    internal class Network
    {
        private NetworkStream stream_;
        
        private Thread readWorker_;
        private Thread heartBeatWorker_;

        private TcpClient client_;
        private NET_STATE state_ = NET_STATE.START;

        private PacketProcess packetProcee_;
        ~Network()
        {
            if (this.isConnected()) {
                this.disConnect();
            }
        }

        NET_STATE state()
        {
            return state_;
        }

        public void close()
        {
            state_ = NET_STATE.DISCONNECTED;

            stream_.Close();
            stream_.Flush();
            client_.Close();
            readWorker_.Abort();
            heartBeatWorker_.Abort();
            packetProcee_ = null;
        }

        public bool connect(string ip, uint port)
        {
            client_ = new TcpClient();
            try {
                client_.Connect(ip, Convert.ToInt32(port));
            } catch {
                MessageBox.Show("서버 연결 실패", "error", MessageBoxButtons.OK);
                return false;
            }
            state_ = NET_STATE.CONNECTED;

            stream_ = client_.GetStream();

            readWorker_ = new Thread(new ThreadStart(receive));
            readWorker_.Start();

            heartBeatWorker_ = new Thread(new ThreadStart(heartBeat));
            heartBeatWorker_.Start();
            return true;
        }

        public void disConnect()
        {
            state_ = NET_STATE.DISCONNECT;
            PK_C_REQ_EXIT packet = new PK_C_REQ_EXIT();
            this.sendPacket(packet);
        }

        public void setPacketProcess(PacketProcess packetProcess)
        {
            packetProcee_ = packetProcess;
        }

        private bool isConnected()
        {
            return state_ == NET_STATE.CONNECTED ? true : false;
        }

        public void receive()
        {
            try {
                while (this.isConnected()) {
                    Byte[] packetByte = new Byte[client_.ReceiveBufferSize];

                    Int32 offset = 0;
                    Int32 readLen = stream_.Read(packetByte, offset, packetByte.Length);

                    PacketObfuscation.decodingHeader(ref packetByte, sizeof(Int32));
                    Int32 packetLen = PacketUtil.decodePacketLen(packetByte, ref offset);
                    
                    while (readLen < packetLen) {
                        Byte[] remainPacket = new Byte[client_.ReceiveBufferSize];
                        Int32 remainLen = 0;
                        remainLen = stream_.Read(remainPacket, 0, remainPacket.Length);
                        Buffer.BlockCopy(remainPacket, 0, packetByte, readLen, remainLen);
                        readLen += remainLen;
                    }

                    Byte[] packetData = new Byte[client_.ReceiveBufferSize];
                    Buffer.BlockCopy(packetByte, offset, packetData, 0, readLen - offset);
                    PacketObfuscation.decodingData(ref packetData, packetData.Length);

                    PacketInterface rowPacket = PacketUtil.packetAnalyzer(packetData);
                    if (rowPacket == null && this.isConnected()) {
                        MessageBox.Show("잘못된 패킷이 수신되었습니다", "error", MessageBoxButtons.OK);
                        Application.Exit();
                    }
                    packetProcee_.run(rowPacket);
                }
                this.close();
            } catch (Exception e) {
                if (this.isConnected()) {
                    MessageBox.Show("잘못된 처리 : " + e.ToString(), "error", MessageBoxButtons.OK);
                    Application.Exit();
                }
            }
        }

        public void sendPacket(PacketInterface packet)
        {
            try {
                packet.encode();
                MemoryStream packetBlock = new MemoryStream();

                Int32 packetLen = sizeof(Int32) + (Int32)packet.getStream().Length;

                Byte[] packetHeader = BitConverter.GetBytes(packetLen);                
                PacketObfuscation.encodingHeader(ref packetHeader, (int)packetHeader.Length);
                packetBlock.Write(packetHeader, 0, (Int32)packetHeader.Length);

                Byte[] packetData = packet.getStream().ToArray();
                PacketObfuscation.encodingData(ref packetData, (int)packetData.Length);
                packetBlock.Write(packetData, 0, (Int32)packetData.Length);

                Byte[] packetBytes = packetBlock.ToArray();                
                stream_.Write(packetBytes, 0, (int) packetBlock.Length);
                stream_.Flush();

                packetBlock = null;
            } catch (Exception e) {
                if (this.isConnected()) {
                    MessageBox.Show("잘못된 처리 : " + e.ToString(), "error", MessageBoxButtons.OK);
                    Application.Exit();
                }
            }
        }

        private void heartBeat()
        {
            while (this.isConnected()) {
                PK_C_NOTIFY_HEARTBEAT heartBeatPacket = new PK_C_NOTIFY_HEARTBEAT();
                this.sendPacket(heartBeatPacket);
                Thread.Sleep(1000);
            }
        }
    }
}