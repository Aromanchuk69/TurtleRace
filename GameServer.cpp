#include "GameServer.h"
#include <time.h>

//#define QUICKEND

void CGameServer::start_server(int port, game_interface* responser)
{
	srand((unsigned)time(NULL));

	if (server_socket_)
	{
		return;
	}

	responser_ = responser;

	server_socket_ = new CServerSocket(this);
	server_socket_->Create(port);

	if (server_socket_->Listen())
	{
		responser_->server_started();
	}
	else
	{
		delete(server_socket_);
		server_socket_ = NULL;
		responser_->server_start_failed("Ошибка старта сервера");
	}
}

void CGameServer::stop_server()
{
	if (!server_socket_)
		return;

	//if (sockets_.client_socket_)
	//{
	//	sockets_.client_socket_->Close();
	//	info_responser_->LogEvent("Клиент ~" + sockets_.client_socket_->client_name() + "~ отключен");
	//	sockets_.delete_client();
	//}

	// Стопорим сам сервер
	server_socket_->Close();
	delete(server_socket_);
	server_socket_ = NULL;

	gamers_.clear();
	game_track_.init();

	responser_ = NULL;
}

void CGameServer::new_client_connection()
{
	if (server_socket_)
	{
		CClientSocket* new_client_socket = new CClientSocket(this);

		if (new_client_socket)
		{
			sockets_wo_logon_.insert(new_client_socket);

			server_socket_->Accept(*new_client_socket);

			messages::Msg_Invitation_To_Logon	message;

			new_client_socket->send_message(message);
		}
	}
}

void CGameServer::refresh_gamers_list()
{
	std::map<std::string, gamer_info_t>::iterator	it_gamer;

	messages::Msg_Gamers_List	gamers_list_msg;
	for (it_gamer = gamers_.begin(); it_gamer != gamers_.end(); it_gamer++)
		if (it_gamer->second.socket_)
			gamers_list_msg.add_gamer(it_gamer->first);

	for (it_gamer = gamers_.begin(); it_gamer != gamers_.end(); it_gamer++)
		if (it_gamer->second.socket_)
			it_gamer->second.socket_->send_message(gamers_list_msg);
}

void CGameServer::send_news_to_all(std::string info, CClientSocket* except)
{
	std::map<std::string, gamer_info_t>::iterator	it_gamer;

	messages::Msg_Send_News	news_msg(info);

	for (it_gamer = gamers_.begin(); it_gamer != gamers_.end(); it_gamer++)
		if (it_gamer->second.socket_)
			it_gamer->second.socket_->send_message(news_msg);
}

void CGameServer::client_disconnection(CClientSocket* socket)
{
	std::map<std::string, gamer_info_t>::iterator	it_gamer;
	for (it_gamer = gamers_.begin(); it_gamer != gamers_.end(); it_gamer++)
	{
		if (it_gamer->second.socket_ == socket)
		{
			if (game_stage_ == game_stage_t::accept_gamers)
				gamers_.erase(it_gamer);
			else
				it_gamer->second.socket_ = NULL;

			refresh_gamers_list();
			break;
		}
	}
}

void CGameServer::ShuffleColors(int* pColors)
{
	int i, first, second, temp;
	for (i = 0; i < 1000; i++)
	{
		first = rand() % 5;
		second = rand() % 5;
		temp = *(pColors + first);
		*(pColors + first) = *(pColors + second);
		*(pColors + second) = temp;
	}
}

void CGameServer::InitCards()
{
	played_cards_.clear();
	cards_to_play_.clear();

	int i;

	cards_to_play_.push_back(messages::cards_t::c_1_blue_plusplus);
	cards_to_play_.push_back(messages::cards_t::c_1_green_plusplus);
	cards_to_play_.push_back(messages::cards_t::c_1_magenta_plusplus);
	cards_to_play_.push_back(messages::cards_t::c_1_red_plusplus);
	cards_to_play_.push_back(messages::cards_t::c_1_yellow_plusplus);

	for (i = 0 ; i < 5 ; i++)	cards_to_play_.push_back(messages::cards_t::c_2_blue_plus);
	for (i = 0 ; i < 5 ; i++)	cards_to_play_.push_back(messages::cards_t::c_2_green_plus);
	for (i = 0 ; i < 5 ; i++)	cards_to_play_.push_back(messages::cards_t::c_2_magenta_plus);
	for (i = 0 ; i < 5 ; i++)	cards_to_play_.push_back(messages::cards_t::c_2_red_plus);
	for (i = 0 ; i < 5 ; i++)	cards_to_play_.push_back(messages::cards_t::c_2_yellow_plus);

	for (i = 0; i < 2; i++)	cards_to_play_.push_back(messages::cards_t::c_3_blue_minus);
	for (i = 0; i < 2; i++)	cards_to_play_.push_back(messages::cards_t::c_3_green_minus);
	for (i = 0; i < 2; i++)	cards_to_play_.push_back(messages::cards_t::c_3_magenta_minus);
	for (i = 0; i < 2; i++)	cards_to_play_.push_back(messages::cards_t::c_3_red_minus);
	for (i = 0; i < 2; i++)	cards_to_play_.push_back(messages::cards_t::c_3_yellow_minus);

	for (i = 0; i < 3; i++)	cards_to_play_.push_back(messages::cards_t::c_4_nocolor_up);
	for (i = 0; i < 2; i++)	cards_to_play_.push_back(messages::cards_t::c_5_nocolor_upup);
	for (i = 0; i < 5; i++)	cards_to_play_.push_back(messages::cards_t::c_6_nocolor_plus);
	for (i = 0; i < 2; i++)	cards_to_play_.push_back(messages::cards_t::c_7_nocolor_minus);
}


void CGameServer::ShuffleCards()
{
	int ipos;
	int imax = int(cards_to_play_.size());

	for (int i = 0; i < 1000; i++)
	{
		std::vector<messages::cards_t>::iterator	it_card = cards_to_play_.begin();
		ipos = rand() % imax;
		it_card += ipos;
		messages::cards_t current = *it_card;
		cards_to_play_.erase(it_card);
		cards_to_play_.push_back(current);
	}
}

void CGameServer::process_message(CClientSocket* socket, messages::Message* message)
{
	std::lock_guard<std::mutex> lock(message_processing_mutex_);

	switch (message->message_type())
	{
	case messages::message_type_t::logon_request:
		{
			messages::Msg_Logon_Request* msg = (messages::Msg_Logon_Request*)message;
			messages::Message	dummy_message;

			if (dummy_message.protocol_version() != msg->protocol_version())
			{
				messages::Msg_Logon_Response	response("Неверная версия программы");
				socket->send_message(response);
				return;
			}

			if (game_stage_ == game_stage_t::accept_gamers && gamers_.size() >= 5)
			{
				messages::Msg_Logon_Response	response("Команда уже набрана");
				socket->send_message(response);
				return;
			}

			std::map<std::string, gamer_info_t>::iterator	it_gamer;
			it_gamer = gamers_.find(msg->user_id());
			if ( it_gamer != gamers_.end())
			{
				if (it_gamer->second.socket_)
				{
					messages::Msg_Logon_Response	response("Игрок с таким именем уже в игре");
					socket->send_message(response);
					sockets_wo_logon_.erase(socket);
					return;
				}
				else
				{
					send_news_to_all("Игрок " + it_gamer->first + " вернулся в игру");
					it_gamer->second.socket_ = socket;
					messages::Msg_Logon_Response	response("");
					socket->send_message(response);
					sockets_wo_logon_.erase(socket);
				}
			}

			sockets_wo_logon_.erase(socket);

			gamer_info_t	g_info;
			g_info.socket_ = socket;

			gamers_[msg->user_id()] = g_info;

			messages::Msg_Logon_Response	response("");
			socket->send_message(response);

			refresh_gamers_list();
		}
		break;
	case messages::message_type_t::short_report:
		{
			messages::Msg_Short_Report* msg = (messages::Msg_Short_Report*)message;

			switch (msg->report_type())
			{
			case messages::short_report_t::ready_to_start:
				{
					std::map<std::string, gamer_info_t>::iterator	it_gamer;
					it_gamer = gamers_.find(msg->user_id());
					if (it_gamer != gamers_.end())
						it_gamer->second.ready_to_start_ = true;

					for (it_gamer = gamers_.begin(); it_gamer != gamers_.end(); it_gamer++)
					{
						if (!it_gamer->second.ready_to_start_)
							return;
					}

					int	iColors[5] = { 1,2,3,4,5 };
					ShuffleColors(iColors);

					game_track_.init();
					
					game_stage_ = game_stage_t::choosing_a_color;
					int i = 0;
					for (it_gamer = gamers_.begin(); it_gamer != gamers_.end(); it_gamer++)
					{
						if (it_gamer->second.socket_ && i < 5)
						{
							messages::colors_t	choosen = messages::colors_t(iColors[i]);
							it_gamer->second.turtle_color_ = choosen;
							messages::Msg_Choose_Color color_message(choosen);
							it_gamer->second.socket_->send_message(color_message);
						}

						i++;
					}

					InitCards();
					next_gamer_ = "";
				}
				break;
			case messages::short_report_t::ready_for_cards_deal:
				{
					messages::Msg_Initial_Cards_Deals	cards_deal;

					for (int i = 0; i < 5; i++)
					{
						int imax = int(cards_to_play_.size());
						std::vector<messages::cards_t>::iterator	it_card = cards_to_play_.begin() + (rand() % imax);

						cards_deal.add_card(*it_card);
						cards_to_play_.erase(it_card);
					}

					std::map<std::string, gamer_info_t>::iterator	it_gamer;
					for (it_gamer = gamers_.begin(); it_gamer != gamers_.end(); it_gamer++)
					{
						if (it_gamer->second.socket_ == socket)
						{
							it_gamer->second.socket_->send_message(cards_deal);
							break;
						}
					}

					game_track_.init();
				}
				break;
			case messages::short_report_t::ready_for_next_move:
			case messages::short_report_t::move_done:
				{
					std::map<std::string, gamer_info_t>::iterator	it_gamer;

					it_gamer = gamers_.find(msg->user_id());
					if (it_gamer != gamers_.end())
						it_gamer->second.ready_for_race_ = true;

					for (it_gamer = gamers_.begin(); it_gamer != gamers_.end(); it_gamer++)
					{
						if (!it_gamer->second.ready_for_race_ || !it_gamer->second.socket_)
							return;
					}

					game_track_.check_for_winner();
					if (game_track_.winner_ != messages::colors_t::unknown)
					{
						messages::Msg_End_Game	end_game(game_track_.winner_);

						for (it_gamer = gamers_.begin(); it_gamer != gamers_.end(); it_gamer++)
							end_game.add_gamer_color(it_gamer->second.turtle_color_, it_gamer->first);

						for (it_gamer = gamers_.begin(); it_gamer != gamers_.end(); it_gamer++)
						{
							if (it_gamer->second.socket_)
								it_gamer->second.socket_->send_message(end_game);

							it_gamer->second.ready_to_start_ = false;
							it_gamer->second.ready_for_race_ = false;
						}

						game_track_.init();
						next_gamer_ = "";
						return;
					}

					if (next_gamer_.empty())
					{
						std::vector<std::string> names;

						for (it_gamer = gamers_.begin(); it_gamer != gamers_.end(); it_gamer++)
							names.push_back(it_gamer->first);

						next_gamer_ = *(names.begin() + int(rand() % gamers_.size()));
					}

					std::string s_info = "Ход игрока " + next_gamer_;
					messages::Msg_Send_News news_msg(s_info);

					// Все готовы - даем команду делать ход, остальных извещаем
					for (it_gamer = gamers_.begin(); it_gamer != gamers_.end(); it_gamer++)
					{
						if (it_gamer->first == next_gamer_)
						{
							messages::Msg_Server_Command	cmd_msg(messages::server_command_t::your_turn_now);
							if (it_gamer->second.socket_)
							{
								it_gamer->second.socket_->send_message(cmd_msg);
							}
						}
						else
						{
							if (it_gamer->second.socket_)
								it_gamer->second.socket_->send_message(news_msg);
						}
					}
				}
				break;
			case  messages::short_report_t::move_skipped:
				{
					std::map<std::string, gamer_info_t>::iterator	it_gamer;
					it_gamer = gamers_.find(msg->user_id());

					it_gamer++;
					if (it_gamer == gamers_.end())
						it_gamer = gamers_.begin();

					next_gamer_ = it_gamer->first;

					std::string s_info = "Ход игрока " + next_gamer_;
					messages::Msg_Send_News news_msg(s_info);

					// Все готовы - даем команду делать ход, остальных извещаем
					for (it_gamer = gamers_.begin(); it_gamer != gamers_.end(); it_gamer++)
					{
						if (it_gamer->first == next_gamer_)
						{
							messages::Msg_Server_Command	cmd_msg(messages::server_command_t::your_turn_now);
							if (it_gamer->second.socket_)
							{
								it_gamer->second.socket_->send_message(cmd_msg);
							}
						}
						else
						{
							if (it_gamer->second.socket_)
								it_gamer->second.socket_->send_message(news_msg);
						}
					}
				}
				break;
			}
		}
		break;
	case messages::message_type_t::make_move:
		{
			messages::Msg_Make_Move* msg = (messages::Msg_Make_Move*)message;

			std::map<std::string, gamer_info_t>::iterator	it_gamer;
			std::map<std::string, gamer_info_t>::iterator	prev_gamer;
			prev_gamer = it_gamer = gamers_.find(msg->user_id());
			if (it_gamer != gamers_.end())
			{
				if (game_track_.make_move(msg->card(), msg->turtle_color()))
				{
					int imax = int(cards_to_play_.size());
					if (!imax)
					{
						cards_to_play_ = played_cards_;
						played_cards_.clear();
						imax = int(cards_to_play_.size());
					}

					///////////////////////////////////////////////////////////////////
#ifdef QUICKEND
					if (game_track_.start_position_[0] != messages::colors_t::unknown)
						#
					{
						#
						game_track_.rocks_[9].push_back(game_track_.start_position_[0]);
						#
						game_track_.winner_ = game_track_.start_position_[0];
						#
						game_track_.start_position_[0] = messages::colors_t::unknown;
						#
					}
					#
					else
						#
					{
						#
						game_track_.rocks_[9].push_back(game_track_.start_position_[1]);
						#
						game_track_.winner_ = game_track_.start_position_[1];
						#
						game_track_.start_position_[1] = messages::colors_t::unknown;
					}
#endif // QUICKEND
					///////////////////////////////////////////////////////////////////

					std::vector<messages::cards_t>::iterator	it_card = cards_to_play_.begin() + (rand() % imax);

					messages::Msg_New_Card new_card_msg(*it_card);
					it_gamer->second.socket_->send_message(new_card_msg);

					cards_to_play_.erase(it_card);

					played_cards_.push_back(msg->card());

					it_gamer++;
					if (it_gamer == gamers_.end())
						it_gamer = gamers_.begin();

					next_gamer_ = it_gamer->first;
				}

				messages::colors_t	color = messages::card_color(msg->card());
				if (color == messages::colors_t::unknown)
					color = msg->turtle_color();

				messages::Msg_Move_Ack	move_ack(msg->user_id(), color, messages::card_steps(msg->card()), game_track_);
				for (it_gamer = gamers_.begin(); it_gamer != gamers_.end(); it_gamer++)
				{
					if (it_gamer->second.socket_)
						it_gamer->second.socket_->send_message(move_ack);

					it_gamer->second.ready_for_race_ = false;
				}
			}
		}
		break;
	default:
		break;
	}
}
