#include "GameClient.h"
#include "Message.h"

void CGameClient::start_client(const std::string& ip, int port, const std::string& login, game_interface* responser)
{
	responser_ = responser;
	gamer_name_ = login;

	if (client_socket_)
	{
		responser_->log_event("Связь с сервером уже установлена");
		return;
	}

	client_socket_ = new CClientSocket(this);
	client_socket_->Create();

	if (!client_socket_->Connect(ip.c_str(), UINT(port)))
	{
		sprintf_s(log_buffer_, log_buffer_size_, "Сбой соединения. Ошибка : %d", client_socket_->GetLastError());
		responser_->client_start_failed(log_buffer_);

		client_socket_->Close();
		delete(client_socket_);
		client_socket_ = NULL;
		responser_ = NULL;

		return;
	}

	sprintf_s(log_buffer_, log_buffer_size_, "Установлено соединение с %s ; port %d", ip.c_str(), port);
	responser_->log_event(log_buffer_);
}

void CGameClient::stop_client()
{
	if (!client_socket_)
		return;

	client_socket_->Close();
	delete(client_socket_);
	client_socket_ = NULL;

	responser_ = NULL;
}

void CGameClient::client_disconnection(CClientSocket* socket)
{
	std::vector<std::string> empty_list;
	responser_->update_gamers_list(empty_list);
	responser_->logon_rejected(gamer_name_, "Потеряно соединение с сервером");
	stop_client();
}

bool CGameClient::ready_to_start()
{
	if (!client_socket_)
		return false;

	messages::Msg_Short_Report ready_to_start(gamer_name_, messages::short_report_t::ready_to_start);
	client_socket_->send_message(ready_to_start);

	return true;
}

bool CGameClient::waiting_for_cards(bool first_deal)
{
	if (!client_socket_)
		return false;

	if (first_deal)
	{
		messages::Msg_Short_Report wait_for_cards(gamer_name_, messages::short_report_t::ready_for_cards_deal);
		client_socket_->send_message(wait_for_cards);
	}
	else
	{
		//messages::Msg_Short_Report wait_for_cards(gamer_name_, messages::short_report_t::need_1_card);
		//client_socket_->send_message(wait_for_cards);
	}

	return true;
}

bool CGameClient::ready_for_next_move()
{
	if (!client_socket_)
		return false;

	messages::Msg_Short_Report wait_for_cards(gamer_name_, messages::short_report_t::ready_for_next_move);
	client_socket_->send_message(wait_for_cards);

	return true;
}

bool CGameClient::my_move(messages::cards_t card, messages::colors_t color)
{
	if (!client_socket_)
		return false;

	messages::Msg_Make_Move make_move(gamer_name_, card, color);
	client_socket_->send_message(make_move);

	return true;
}

bool CGameClient::move_done()
{
	if (!client_socket_)
		return false;

	messages::Msg_Short_Report move_done(gamer_name_, messages::short_report_t::move_done);
	client_socket_->send_message(move_done);

	return true;
}

bool CGameClient::move_skipped()
{
	if (!client_socket_)
		return false;

	messages::Msg_Short_Report move_skipped(gamer_name_, messages::short_report_t::move_skipped);
	client_socket_->send_message(move_skipped);

	return true;
}

void CGameClient::process_message(CClientSocket* socket, messages::Message* message)
{
	switch (message->message_type())
	{
	case messages::message_type_t::invitation_to_logon:
		{
		messages::Msg_Invitation_To_Logon* msg = (messages::Msg_Invitation_To_Logon*)message;

		messages::Msg_Logon_Request	logon_request(gamer_name_);
		client_socket_->send_message(logon_request);
		}
		break;
	case messages::message_type_t::logon_response:
		{
		messages::Msg_Logon_Response* msg = (messages::Msg_Logon_Response*)message;

		if (msg->result().empty())
		{
			responser_->logon_accepted(gamer_name_);
		}
		else
		{
			responser_->logon_rejected(gamer_name_, msg->result());
			stop_client();
		}
		}
		break;
	case messages::message_type_t::gamers_list:
		{
			messages::Msg_Gamers_List* msg = (messages::Msg_Gamers_List*)message;
			responser_->update_gamers_list(msg->gamers());
		}
		break;
	case messages::message_type_t::send_news:
		{
			messages::Msg_Send_News* msg = (messages::Msg_Send_News*)message;
			responser_->log_event(msg->info());
		}
		break;
	case messages::message_type_t::choose_color:
		{
			messages::Msg_Choose_Color* msg = (messages::Msg_Choose_Color*)message;
			responser_->set_turtle_color(msg->color());
		}
	break;
	case messages::message_type_t::initial_cards_deal:
		{
			messages::Msg_Initial_Cards_Deals* msg = (messages::Msg_Initial_Cards_Deals*)message;
			responser_->cards_deal(msg->cards());
		}
		break;
	case messages::message_type_t::server_command:
		{
			messages::Msg_Server_Command* msg = (messages::Msg_Server_Command*)message;
			switch (msg->command_type())
			{
			case messages::server_command_t::your_turn_now:
				responser_->your_turn_now();
				break;
			}
		}
		break;
	case messages::message_type_t::move_ack:
		{
			messages::Msg_Move_Ack* msg = (messages::Msg_Move_Ack*)message;
			responser_->move_ack(msg->user_id(), msg->color(), msg->steps(), msg->snapshot());
		}
		break;
	case messages::message_type_t::new_card:
		{
			messages::Msg_New_Card* msg = (messages::Msg_New_Card*)message;
			responser_->new_card(msg->card());
		}
		break;
	case messages::message_type_t::end_game:
		{
			messages::Msg_End_Game* msg = (messages::Msg_End_Game*)message;
			responser_->game_over(msg->winner_color(), msg->gamers_colors());
		}
		break;
	default:
		break;
	}
}
