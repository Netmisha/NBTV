#include "AppManager.h"
void ProcessMessage(const RecvStruct &recv_str, AppManager& am)
{
    if ((recv_str.ip_ != am.GetIP()) || BROADCAST_LOOPBACK)
    {
        //allocation in heap
        UnpackedMessage unp_msg = Parcer::UnpackMessage(recv_str.packet_);

        switch (unp_msg.type_)
        {
        case CHAT_MESSAGE:
            am.AddMsg(*(UserMsg*)unp_msg.msg_);
            break;

        case LOG_MESSAGE:
            am.ProcessLogMessage(*(LogMessage*)unp_msg.msg_, recv_str.ip_);
            break;

        case GET_FILE_MESSAGE:
            am.SendFile(am.GetFilePath(*((int*)unp_msg.msg_)),
                     recv_str.ip_,
                     am.GetFileName(*((int*)unp_msg.msg_)));
            break;

        case FILE_LIST_REQUEST:
            am.SendList(recv_str.ip_);
            break;

        }

        delete unp_msg.msg_;
    }

} //will remove it in independent header

void RecvLoop(AppManager& am)
{
    static bool is_working_ = true;
    while (is_working_)
    {
        RecvStruct packet;
        RecvSocket recv_socket;
        recv_socket.Recv(&packet);
        
        //TODO
        ProcessMessage(packet, am);
      //Send new info to AppManager
    // And AFTER THAT refresh UI (will use GetCurrentChat() func)
    }
}


int main()
{
	AppManager AM;
	AM.Work();
    RecvLoop(AM); //for getting CHAT_MESSAGE, 
                //GET_FILE_MESSAGE, FILE_LIST_REQUEST
    //All senging are hiden in AppManager

    return 0;
}