using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Data;

namespace packetMaker
{
    class ClientClassMakePacket : MakePacket
    {
        private string header_;
        private string classHeader_;
        private string classTailer_;
        private string encodeFuncHead_;
        private string encodeFunction_;
        private string decodeFuncHead_;
        private string decodeFunction_;
        private string tailer_;

        public ClientClassMakePacket(string filePath, string formSheet)
            : base(filePath, formSheet)
        {
        }

        protected override void initialize(DataTable excelData)
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
                else if (parseToken.CompareTo("classTailer") == 0)
                    classTailer_ = row[parseIdx].ToString();
                else if (parseToken.CompareTo("encodeFuncHead") == 0)
                    encodeFuncHead_ = row[parseIdx].ToString();
                else if (parseToken.CompareTo("encodeFunction") == 0)
                    encodeFunction_ = row[parseIdx].ToString();
                else if (parseToken.CompareTo("decodeFuncHead") == 0)
                    decodeFuncHead_ = row[parseIdx].ToString();
                else if (parseToken.CompareTo("decodeFunction") == 0)
                    decodeFunction_ = row[parseIdx].ToString();
                else if (parseToken.CompareTo("tailer") == 0)
                    tailer_ = row[parseIdx].ToString();
            }
        }

        private string parseEncodeFunction(DataRow classRow)
        {
            string parseStr = tab_ + encodeFuncHead_;
            parseStr += nextLine_;
            
            int columnIdx = 2;
            foreach (DataColumn Col in excelData_.Columns)
            {
                if (columnIdx++ == 2)
                    continue;

                string token = classRow[Col].ToString();
                if (token == "")
                    break;

                if (System.Convert.ToBoolean(columnIdx % 2))
                {
                    parseStr += string.Format(encodeFunction_, token);
                    parseStr += nextLine_;
                }
            }
            parseStr += tab_ + tab_ + "}" + nextLine_;
            return parseStr;
        }

        private string parseDecodeFunction(DataRow classRow)
        {
            string parseStr = tab_ + decodeFuncHead_ + nextLine_;

            int columnIdx = 2;
            string typeStr = "";
            foreach (DataColumn Col in excelData_.Columns)
            {
                if (columnIdx++ == 2)
                    continue;

                string token = classRow[Col].ToString();
                if (token == "")
                    break;

                if (System.Convert.ToBoolean(columnIdx % 2))
                {
                    parseStr += tab_;
                    parseStr += string.Format(decodeFunction_, token, typeStr);
                    parseStr += nextLine_;
                }
                else
                {
                    typeStr = classRow[Col].ToString();
                    typeStr = typeStr.Replace("std::", "");
                }
            }
            parseStr += tab_ + tab_ + "}" + nextLine_;
            return parseStr;
        }

        private string parseMemberVal(DataRow classRow)
        {
            string parseStr = nextLine_;
            int columnIdx = 2;
            foreach (DataColumn Col in excelData_.Columns)
            {
                if (columnIdx++ == 2)
                    continue;

                string token = classRow[Col].ToString();
                if (token == "")
                    break;

                token = token.Replace("std::", "");

                bool valueString = System.Convert.ToBoolean(columnIdx % 2);
                if (valueString)
                {
                    parseStr += tab_ + tab_ + token;
                    parseStr += ";" + nextLine_;
                }
                else
                {
                    parseStr += tab_ + tab_ + "public " + token;
                    parseStr += " ";
                }
            }
            parseStr += nextLine_;
            return parseStr;
        }

        protected override string parse()
        {
            string parseStr = header_ + nextLine_;

            foreach (DataRow classRow in excelData_.Rows)
            {
                string nameToken = classRow[0].ToString();
                if (nameToken.StartsWith("#"))
                {
                    continue;
                }
                const int classNameCol = 0;
                const int valTokenCol = 1;

                parseStr += string.Format(classHeader_, classRow[classNameCol].ToString());
                string token = classRow[valTokenCol].ToString();
                // 파라메터 없는 패킷이면...
                if (token == "")
                {
                    parseStr += nextLine_;
                    parseStr += tab_ + encodeFuncHead_ + nextLine_ + tab_ + tab_ + "}" + nextLine_;
                    parseStr += tab_ + decodeFuncHead_ + nextLine_ + tab_ + tab_ + "}" + nextLine_;
                    parseStr += classTailer_ + nextLine_;
                    continue;
                }
                //파라메터 있는 패킷이면...
                parseStr += parseMemberVal(classRow);
                parseStr += parseEncodeFunction(classRow);
                parseStr += parseDecodeFunction(classRow);

                parseStr += classTailer_ + nextLine_;
            }
            parseStr += tailer_;
            return parseStr;
        }
    }
}
