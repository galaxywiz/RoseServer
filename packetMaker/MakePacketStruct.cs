using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Data;

namespace packetMaker
{
    class MakePacketStruct : MakePacket
    {
        private string header_;                 //file header macro
        private string classHeader_;            //class macro
        private string packetType_;             //..
        private string encode_;
        private string streamIn_;
        private string stringIn_;
        private string decode_;
        private string streamOut_;
        private string stringOut_;
      
        public MakePacketStruct(string filePatch, string formSheet)
            : base(filePatch, formSheet)
        {
        }

        public override void initialize(DataTable excelData)
        {
            foreach (DataRow row in excelData.Rows)
            {
                const int titleIdx = 0;
                const int parseIdx = 1;
                string parseToken = row[titleIdx].ToString();
                if (parseToken.CompareTo("header") == 0)
                    header_ = row[parseIdx].ToString();
                else if (parseToken.CompareTo("classHeader") == 0)
                    classHeader_ = row[parseIdx].ToString();
                else if (parseToken.CompareTo("packetType") == 0)
                    packetType_ = row[parseIdx].ToString();
                else if (parseToken.CompareTo("encode") == 0)
                    encode_ = row[parseIdx].ToString();
                else if (parseToken.CompareTo("streamIn") == 0)
                    streamIn_ = row[parseIdx].ToString();
                else if (parseToken.CompareTo("stringIn") == 0)
                    stringIn_ = row[parseIdx].ToString();
                else if (parseToken.CompareTo("decode") == 0)
                    decode_ = row[parseIdx].ToString();
                else if (parseToken.CompareTo("streamOut") == 0)
                    streamOut_ = row[parseIdx].ToString();
                else if (parseToken.CompareTo("stringOut") == 0)
                    stringOut_ = row[parseIdx].ToString();
            }
        }

        private string parseMemberVal(DataRow classRow)
        {
            string parseStr = "";

            int columnIdx = 2;
            foreach (DataColumn Col in excelData_.Columns)
            {
                if (columnIdx++ == 2)
                {
                    continue;
                }
                string token = classRow[Col].ToString();
                if (token == "")
                {
                    break;
                }
                parseStr += tab_ + token;
                if ((columnIdx % 2) == 0)
                {
                    parseStr += " ";
                }
                else
                {
                    parseStr += ";" + nextLine_;
                }
            }
            return parseStr;
        }

        private string parseFunction(DataRow classRow, string streamStr, string stringStr)
        {
            string parseStr = "";
            int columnIdx = 2;
            string typeToken = "";
            foreach (DataColumn Col in excelData_.Columns)
            {
                if ((columnIdx++ % 2) != 0 || columnIdx == 3)
                {
                    typeToken = classRow[Col].ToString();
                    continue;
                }
                string valToken = classRow[Col].ToString();
                if (valToken == "")
                {
                    break;
                }

                parseStr += string.Format(streamStr, valToken);
                parseStr += nextLine_;
            }
            return parseStr;
        }

        private string parseEncode(DataRow classRow)
        {
            string parseStr = nextLine_;
            parseStr += encode_ + scopeIn_ + nextLine_;
            parseStr += parseFunction(classRow, streamIn_, stringIn_);
            parseStr += tab_ + scopeOut_ + nextLine_;
            return parseStr;
        }

        private string parseDecode(DataRow classRow)
        {
            string parseStr = nextLine_;
            parseStr += decode_ + scopeIn_ + nextLine_;
            parseStr += parseFunction(classRow, streamOut_, stringOut_);
            parseStr += tab_ + scopeOut_ + nextLine_;
            return parseStr;
        }

        public override string parse()
        {
            string classStatrMacro = classHeader_ + nextLine_
                                    + scopeIn_ + nextLine_
                                    + packetType_ + nextLine_;
            string classEndMacro = scopeOut_ + ";" + nextLine_ + nextLine_;

            string parseStr = header_ + nextLine_ + nextLine_;
            foreach (DataRow classRow in excelData_.Rows)
            {
                const int classNameCol = 0;
                const int valTokenCol = 1;

                parseStr += string.Format(classStatrMacro, classRow[classNameCol].ToString());
                string token = classRow[valTokenCol].ToString();
                if (token == "")
                {
                    parseStr += classEndMacro;
                    continue;
                }
                parseStr += parseMemberVal(classRow);
                parseStr += parseEncode(classRow);
                parseStr += parseDecode(classRow);
                parseStr += classEndMacro;
            }
            return parseStr;
        }
    }
}
