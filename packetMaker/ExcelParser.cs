using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Data;
using System.Data.OleDb;

namespace packetMaker
{
    class ExcelParser
    {
        private DataTable excelTable_ = null;

        public ExcelParser(string filePath, string sheetName)
        {
            this.read(filePath, sheetName);
        }
        private void read(string filePath, string sheetName)
        {
            string provider = "Provider=Microsoft.ACE.OLEDB.12.0; Data Source=" + filePath + ";Extended Properties=Excel 12.0";

            OleDbConnection excel = new OleDbConnection(provider);
            excel.Open();

            string strQuery = "SELECT * FROM [" + sheetName + "$]";

            OleDbCommand dbCommand = new OleDbCommand(strQuery, excel);
            OleDbDataAdapter dataAdapter = new OleDbDataAdapter(dbCommand);

            DataTable table = new DataTable();
            dataAdapter.Fill(table);

            excelTable_ = table;

            dataAdapter.Dispose();
            dbCommand.Dispose();

            excel.Close();
            excel.Dispose();
        }
        ~ExcelParser()
        {
            excelTable_.Dispose();
        }

        public DataTable table()
        {
            return excelTable_;
        }
    }
}
