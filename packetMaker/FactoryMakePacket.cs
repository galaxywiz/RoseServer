using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Data;

namespace packetMaker
{
    class FactoryMakePacket : MakePacket
    {
        private string header_;
        private string macro_;
        private string tailer_;

        public FactoryMakePacket(string filePath, string formSheet)
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
                else if (parseToken.CompareTo("macro") == 0)
                    macro_ = row[parseIdx].ToString();
                else if (parseToken.CompareTo("tailer") == 0)
                    tailer_ = row[parseIdx].ToString();
            }
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

                parseStr += string.Format(macro_, classRow[classNameCol].ToString());
                parseStr += nextLine_;
            }
            parseStr += tailer_;
            return parseStr;
        }
    }
}
