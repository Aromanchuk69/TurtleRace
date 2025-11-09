#include "ClientSocket.h"
#include "Message.h"

CClientSocket::CClientSocket(socket_interface* responser)
{
	memset(sending_buffer_, 0, 8192);
	memset(receiving_buffer_, 0, 8192);

	responser_ = responser;
	current_receiving_position_ = 0;
}

CClientSocket::~CClientSocket()
{
	Close();
}
//
//void CClientSocket::send_logon_request(long required_version)
//{
//	Msg_Logon_Request* msg = new Msg_Logon_Request(client_name_, required_version);
//	Send(msg);
//}

//void CClientSocket::send_logon_response(logon_response_t response)
//{
//	Msg_Logon_Response* msg = new Msg_Logon_Response(response);
//	Send(msg);
//}

void CClientSocket::OnReceive(int nErrorCode)
{
	std::lock_guard<std::mutex> lock(read_socket_mutex_);

	current_receiving_position_ += Receive(receiving_buffer_ + current_receiving_position_, 4096);

	for (; ; )
	{
	// Сотворим "пустой месседж (только хидер) и попробуем его считать
		messages::Message	dummy_message;
		if (current_receiving_position_ < dummy_message.full_message_length())
			break;

		// Теперь, когда имеем полный хидер, посмотрим, получится ли считать полный месседж
		dummy_message.deserialize(receiving_buffer_);
		if (current_receiving_position_ < dummy_message.full_message_length())
			break;

		messages::Message* next_message = messages::create_message(dummy_message.message_type());

		if (next_message)
		{
			int msg_len = next_message->deserialize(receiving_buffer_);
			responser_->process_message(this, next_message);
			current_receiving_position_ -= msg_len;
			memmove(receiving_buffer_, receiving_buffer_ + msg_len, (8192 - msg_len));
		}
	}

	CSocket::OnReceive(nErrorCode);
}

void CClientSocket::OnClose(int nErrorCode)
{
	CSocket::OnClose(nErrorCode);

	responser_->client_disconnection(this);
}

bool CClientSocket::send_message(messages::Message& message)
{
	message.serialize(sending_buffer_);
	
	int sent = CSocket::Send(sending_buffer_, message.full_message_length());

	if (sent == message.full_message_length())
		return true;
	else
		return false;
}

//bool CClientSocket::Send(Message* message)
//{
//	int len = message->serialize(sending_buffer_);
//	int sent = CSocket::Send(sending_buffer_, len);
//
//	// Info out (if nessessary)
//	switch (message->message_type())
//	{
//	case message_type_t::logon_request:
//	{
//		Msg_Logon_Request* msg = (Msg_Logon_Request*)message;
//	}
//	break;
//	case message_type_t::send_snapshot_marker:
//	{
//		Msg_Send_Snapshot_Marker* msg = (Msg_Send_Snapshot_Marker*)message;
//		if (msg->elements_to_send())
//			responser_->LogEvent("Передаю каталог...");
//		else
//			responser_->LogEvent("Передача каталога закончена");
//
//		responser_->EndProcessReport(this);
//	}
//	break;
//	case message_type_t::send_snapshot:
//	{
//		Msg_Send_Snapshot* msg = (Msg_Send_Snapshot*)message;
//		responser_->ProcessReport(this, msg->curpos_);
//	}
//	break;
//	case message_type_t::file_sending_marker:
//	{
//		Msg_File_Sending_Marker* msg = (Msg_File_Sending_Marker*)message;
//		if (msg->marker() == file_transfer_marker_t::transfer_done)
//		{
//			responser_->EndProcessReport(this);
//		}
//	}
//	break;
//	case message_type_t::file_sending:
//	{
//		Msg_Send_File* msg = (Msg_Send_File*)message;
//		responser_->ProcessReport(this, msg->curpos_);
//	}
//	break;
//	default:
//		break;
//	}
//
//	delete(message);
//
//	return (len == sent);
//}
//
//Message* CClientSocket::make_next_message(message_type_t type)
//{
//	Message* msg;
//
//	switch (type)
//	{
//	case message_type_t::logon_request:			msg = new Msg_Logon_Request;		break;
//	case message_type_t::logon_response:		msg = new Msg_Logon_Response;		break;
//	case message_type_t::send_snapshot_marker:	msg = new Msg_Send_Snapshot_Marker;	break;
//	case message_type_t::send_snapshot:			msg = new Msg_Send_Snapshot;		break;
//	case message_type_t::file_sending_marker:	msg = new Msg_File_Sending_Marker;	break;
//	case message_type_t::file_sending:			msg = new Msg_Send_File;			break;
//	case message_type_t::send_snapshot_request:	msg = new Msg_Send_Snapshot_Request; break;
//	default:
//		return NULL;
//	}
//
//	return msg;
//}
