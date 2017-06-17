using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Data;

namespace packetMaker
{
    class HeaderMakePacket : MakePacket
    {
        private string header_;
        private string enum_;
        private string tailer_;
        private int packetIndextStart_ = 128;
        private string packetMakeDate = "2015/03/14 13:14:41";

        public HeaderMakePacket(string filePath, string formSheet)
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
                else if (parseToken.CompareTo("enum") == 0)
                    enum_ = row[parseIdx].ToString();
                else if (parseToken.CompareTo("tailer") == 0)
                    tailer_ = row[parseIdx].ToString();
            }
        }

        protected override string parse()
        {
            string parseStr = string.Format(header_, packetMakeDate) + nextLine_;
            
            int index = packetIndextStart_;
            foreach (DataRow classRow in excelData_.Rows)
            {
                string nameToken = classRow[0].ToString();
                if (nameToken.StartsWith("#"))
                {
                    continue;
                }
                const int classNameCol = 0;

                parseStr += string.Format(enum_, index++, classRow[classNameCol].ToString());
                parseStr += nextLine_;
            }
            parseStr += tailer_;

            return parseStr;
        }
    }
}
