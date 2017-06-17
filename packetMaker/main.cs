using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace packetMaker
{
    class main
    {
        //프로젝트 놓은 디렉토리 설정
        static string patch_ = "D:\\Work\\2014년\\roseserver\\trunk";
        static string serverProjectName_ = "\\ServerCore\\ServerLibrary";
        static string serverProjectPath_ = patch_ + serverProjectName_ + "\\Net\\Packet";

        static string clientProjectName_ = "\\DummyClient";
        static string clientProjectPath_ = patch_ + clientProjectName_ + "\\DummyClient\\Source\\PacketGen";

        static string excelPath_ = patch_ + "\\packetMaker\\packetData.xlsx";
        static string dataSheet_ = "packetData";
        static void Main(string[] args)
        {
            Console.WriteLine("* packet maker run!!! ");
            ClassMakePacket serverClass = new ClassMakePacket(excelPath_, "PacketClass");
            serverClass.make(dataSheet_, serverProjectPath_ + "\\PacketClass.h");

            HeaderMakePacket serverHeader = new HeaderMakePacket(excelPath_, "PacketHeader");
            serverHeader.make(dataSheet_, serverProjectPath_ + "\\PacketHeader.h");

            FactoryMakePacket serverFactory = new FactoryMakePacket(excelPath_, "PacketFactory");
            serverFactory.make(dataSheet_, serverProjectPath_ + "\\PacketFactory.h");

            HeaderMakePacket clientHeader = new HeaderMakePacket(excelPath_, "clientPacketHeader");
            clientHeader.make(dataSheet_, clientProjectPath_ + "\\PacketHeader.cs");

            ClientClassMakePacket clientClass = new ClientClassMakePacket(excelPath_, "clientPacketClass");
            clientClass.make(dataSheet_, clientProjectPath_ + "\\PacketClass.cs");

            FactoryMakePacket clientFactory = new FactoryMakePacket(excelPath_, "clientPacketFactory");
            clientFactory.make(dataSheet_, clientProjectPath_ + "\\PacketFactory.cs");
        }
    }
}
