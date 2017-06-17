namespace DummyClient
{
    partial class ChattingForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.textBox_input_ = new System.Windows.Forms.TextBox();
            this.richTextBox_view_ = new System.Windows.Forms.RichTextBox();
            this.SuspendLayout();
            // 
            // textBox_input_
            // 
            this.textBox_input_.Location = new System.Drawing.Point(20, 314);
            this.textBox_input_.Name = "textBox_input_";
            this.textBox_input_.Size = new System.Drawing.Size(456, 21);
            this.textBox_input_.TabIndex = 3;
            this.textBox_input_.KeyDown += new System.Windows.Forms.KeyEventHandler(this.textBox_input_KeyDown);
            // 
            // richTextBox_view_
            // 
            this.richTextBox_view_.BackColor = System.Drawing.Color.Teal;
            this.richTextBox_view_.ForeColor = System.Drawing.Color.White;
            this.richTextBox_view_.Location = new System.Drawing.Point(20, 12);
            this.richTextBox_view_.Name = "richTextBox_view_";
            this.richTextBox_view_.ReadOnly = true;
            this.richTextBox_view_.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.Vertical;
            this.richTextBox_view_.Size = new System.Drawing.Size(457, 296);
            this.richTextBox_view_.TabIndex = 2;
            this.richTextBox_view_.Text = "";
            // 
            // ChattingForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSize = true;
            this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.ClientSize = new System.Drawing.Size(498, 349);
            this.Controls.Add(this.textBox_input_);
            this.Controls.Add(this.richTextBox_view_);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "ChattingForm";
            this.Text = "ChattingForm";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textBox_input_;
        private System.Windows.Forms.RichTextBox richTextBox_view_;
    }
}