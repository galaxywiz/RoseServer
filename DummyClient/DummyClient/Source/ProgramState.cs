namespace DummyClient
{
    internal enum PROGRAM_STATE
    {
        LOGIN,
        CHATTING,
        MAX,
    }

    internal class ProgramState
    {
        private PROGRAM_STATE state_;
        private FormState formState_ = null;
        private string name_;

        public delegate void putMessageDele(string msg);
        public event putMessageDele putMessage_;

        private void changeState()
        {
            switch (state_) {
            case PROGRAM_STATE.LOGIN:
                formState_ = new LoginFormState();
                break;

            case PROGRAM_STATE.CHATTING:
                formState_ = new ChattingFormState();
                break;
            }
        }

        public void setState(PROGRAM_STATE state, string ip, uint port)
        {
            if (formState_ != null)
                formState_.close();

            state_ = state;
            this.changeState();
            formState_.open(ip, port);
        }

        public void setName(string name)
        {
            name_ = name;
        }

        public string name()
        {
            return name_;
        }

        public void sendPacket(PacketInterface packet)
        {
            this.formState_.sendPacket(ref packet);
        }

        public void putMessage(string msg)
        {
            putMessage_(msg);
        }

        public void close()
        {
            this.formState_.close();
        }
    }
}