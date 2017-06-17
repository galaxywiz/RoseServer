using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Data;
using System.Data.OleDb;
using System.IO;

namespace packetMaker
{
    class MakePacket
    {
        public string nextLine_ = "#newLine";
        public string tab_ = "    ";
        public string scopeIn_ = "`";       //임시 중괄호
        public string scopeOut_ = "~";      //임시 중괄호

        protected string excelFilePath_;
        protected DataTable excelData_;
        public MakePacket(string filePath, string formSheet)
        {
            ExcelParser excel = new ExcelParser(filePath, formSheet);
            this.initialize(excel.table());

            excelFilePath_ = filePath;
        }

        public void make(string dataSheet, string outFIleName)
        {
            ExcelParser excel_ = new ExcelParser(excelFilePath_, dataSheet);
            excelData_ = excel_.table();
            this.writeFile(this.parse(), outFIleName);
        }

        protected virtual void initialize(DataTable excelData)
        {
        }

        protected virtual string parse()
        {
            return null;
        }

        private void writeFile(string parseStr, string outFIleName)
        {
            //중괄호는 C언어로 보면 %d, %s 같은 개념이라 파싱할때 못썻으니
            //파일로 쓰기 전에에 임시 중괄호를 진짜 중괄호로 바꾼다.
            parseStr = parseStr.Replace(scopeIn_, "{");
            parseStr = parseStr.Replace(scopeOut_, "}");

            FileStream fs;
            fs = new FileStream(outFIleName, FileMode.Create);
            StreamWriter write = new StreamWriter(fs);
            string[] lines = System.Text.RegularExpressions.Regex.Split(parseStr, nextLine_);
            foreach (string line in lines)
            {
                write.WriteLine(line);
                Console.WriteLine(line);
            }
            write.Close();
        }
    }
}
