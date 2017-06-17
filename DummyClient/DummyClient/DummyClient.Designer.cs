namespace DummyClient
{
    partial class DummyClient
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
            this.mainPanel_ = new System.Windows.Forms.Panel();
            this.SuspendLayout();
            // 
            // mainPanel_
            // 
            this.mainPanel_.Location = new System.Drawing.Point(5, 8);
            this.mainPanel_.Name = "mainPanel_";
            this.mainPanel_.Size = new System.Drawing.Size(608, 420);
            this.mainPanel_.TabIndex = 0;
            // 
            // DummyClient
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(624, 441);
            this.Controls.Add(this.mainPanel_);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "DummyClient";
            this.Text = "DummyClient";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.DummyClient_FormClosing);
            this.Shown += new System.EventHandler(this.DummyClient_Shown);
            this.ResumeLayout(false);

        }

        #endregion

        public System.Windows.Forms.Panel mainPanel_;

    }
}

