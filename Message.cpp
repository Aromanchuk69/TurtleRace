#include "Message.h"

namespace messages {
	int Message::serialize_int(char* buffer, int val)
	{
		memcpy(buffer, &val, sizeof(int));
		return int(sizeof(int));
	}

	int Message::deserialize_int(char* buffer, int& val)
	{
		memcpy(&val, buffer, sizeof(int));
		return int(sizeof(int));
	}

	int Message::serialize_char_array(char* buffer, const char* val, int len)
	{
		memcpy(buffer, val, len);
		return len;
	}

	int Message::deserialize_char_array(char* buffer, char* val, int len)
	{
		memcpy(val, buffer, len);
		val[len] = 0;
		return len;
	}

	int Message::serialize_string(char* buffer, const std::string& _from)
	{
		int offset = 0;
		int str_len = int(_from.length());
		offset += serialize_int(buffer + offset, str_len);
		if (str_len)
			offset += serialize_char_array(buffer + offset, _from.c_str(), str_len);

		return offset;
	}

	int Message::deserialize_string(char* buffer, std::string& _to)
	{
		if (!buffer)
			_to;

		int offset = 0;
		int str_len = 0;
		char string_buffer[256];
		offset += deserialize_int(buffer + offset, str_len);
		if (str_len)
		{
			offset += deserialize_char_array(buffer + offset, string_buffer, str_len);
			string_buffer[str_len] = 0;
			_to = string_buffer;
		}
		else
			_to = "";

		return offset;
	}

	int Message::serialize_int64(char* buffer, __int64& val)
	{
		memcpy(buffer, &val, sizeof(__int64));
		return int(sizeof(__int64));
	}

	int Message::deserialize_int64(char* buffer, __int64& val)
	{
		memcpy(&val, buffer, sizeof(__int64));
		return int(sizeof(__int64));
	}

	//int Message::serialize_long(char* buffer, long val)
	//{
	//	memcpy(buffer, &val, sizeof(long));
	//	return int(sizeof(long));
	//}
	//
	//int Message::deserialize_long(char* buffer, long& val)
	//{
	//	memcpy(&val, buffer, sizeof(long));
	//	return int(sizeof(long));
	//}

	Message* create_message(message_type_t type)
	{
		switch (type)
		{
		case message_type_t::unknown:				return new Message();
		case message_type_t::logon_request:			return new Msg_Logon_Request();
		case message_type_t::invitation_to_logon:	return new Msg_Invitation_To_Logon();
		case message_type_t::logon_response:		return new Msg_Logon_Response();
		case message_type_t::gamers_list:			return new Msg_Gamers_List();
		case message_type_t::send_news:				return new Msg_Send_News();
		case message_type_t::short_report:			return new Msg_Short_Report();
		case message_type_t::choose_color:			return new Msg_Choose_Color();
		case message_type_t::initial_cards_deal:	return new Msg_Initial_Cards_Deals();
		case message_type_t::server_command:		return new Msg_Server_Command();
		case message_type_t::make_move:				return new Msg_Make_Move();
		case message_type_t::move_ack:				return new Msg_Move_Ack();
		case message_type_t::new_card:				return new Msg_New_Card();
		case message_type_t::end_game:				return new Msg_End_Game();
		default:
			break;
		}

		return NULL;
	}

	colors_t card_color(cards_t card)
	{
		switch (card)
		{
		default:
		case cards_t::c_4_nocolor_up:
		case cards_t::c_5_nocolor_upup:
		case cards_t::c_6_nocolor_plus:
		case cards_t::c_7_nocolor_minus:
		case cards_t::c_0_unknown:			return colors_t::unknown;
		case cards_t::c_1_blue_plusplus:	return colors_t::blue;
		case cards_t::c_1_green_plusplus:	return colors_t::green;
		case cards_t::c_1_magenta_plusplus:	return colors_t::magenta;
		case cards_t::c_1_red_plusplus:		return colors_t::red;
		case cards_t::c_1_yellow_plusplus:	return colors_t::yellow;
		case cards_t::c_2_blue_plus:		return colors_t::blue;
		case cards_t::c_2_green_plus:		return colors_t::green;
		case cards_t::c_2_magenta_plus:		return colors_t::magenta;
		case cards_t::c_2_red_plus:			return colors_t::red;
		case cards_t::c_2_yellow_plus:		return colors_t::yellow;
		case cards_t::c_3_blue_minus:		return colors_t::blue;
		case cards_t::c_3_green_minus:		return colors_t::green;
		case cards_t::c_3_magenta_minus:	return colors_t::magenta;
		case cards_t::c_3_red_minus:		return colors_t::red;
		case cards_t::c_3_yellow_minus:		return colors_t::yellow;
		}
	};

	int card_steps(cards_t card)
	{
		switch (card)
		{
		default:
		case cards_t::c_0_unknown:			return 0;
		case cards_t::c_5_nocolor_upup:
		case cards_t::c_1_blue_plusplus:
		case cards_t::c_1_green_plusplus:
		case cards_t::c_1_magenta_plusplus:
		case cards_t::c_1_red_plusplus:
		case cards_t::c_1_yellow_plusplus:	return 2;
		case cards_t::c_4_nocolor_up:
		case cards_t::c_2_blue_plus:
		case cards_t::c_2_green_plus:
		case cards_t::c_2_magenta_plus:
		case cards_t::c_2_red_plus:
		case cards_t::c_2_yellow_plus:
		case cards_t::c_6_nocolor_plus:		return 1;
		case cards_t::c_7_nocolor_minus:
		case cards_t::c_3_blue_minus:
		case cards_t::c_3_green_minus:
		case cards_t::c_3_magenta_minus:
		case cards_t::c_3_red_minus:
		case cards_t::c_3_yellow_minus:		return -1;
		}
	};

	int Message::message_header_t::serialize(char* buffer)
	{
		int offset = 0;

		offset += serialize_string(buffer + offset, app_id_);
		offset += serialize_int64(buffer + offset, protocol_version_);
		offset += serialize_int(buffer + offset, int(msg_type_));
		offset += serialize_int(buffer + offset, msg_length_);

		return offset;
	};

	int Message::message_header_t::deserialize(char* buffer)
	{
		int tmp_int;
		int offset = 0;

		offset += deserialize_string(buffer + offset, app_id_);
		offset += deserialize_int64(buffer + offset, protocol_version_);
		offset += deserialize_int(buffer + offset, tmp_int);
		msg_type_ = message_type_t(tmp_int);
		offset += deserialize_int(buffer + offset, msg_length_);

		return offset;
	}

	int Message::serialize(char* buffer)
	{
		header_.msg_length_ = message_length();
		return header_.serialize(buffer);
	}

	int Message::deserialize(char* buffer)
	{
		return header_.deserialize(buffer);
	}

	int Msg_Invitation_To_Logon::serialize(char* buffer)
	{
		header_.msg_length_ = message_length();
		return header_.serialize(buffer);
	}

	int Msg_Invitation_To_Logon::deserialize(char* buffer)
	{
		return header_.deserialize(buffer);
	}

	int Msg_Logon_Request::serialize(char* buffer)
	{
		int offset = 0;
		header_.msg_length_ = message_length();
		offset += header_.serialize(buffer + offset);
		offset += serialize_string(buffer + offset, user_id_);
	
		return offset;
	};
	
	int Msg_Logon_Request::deserialize(char* buffer)
	{
		int offset = 0;
	
		offset += header_.deserialize(buffer + offset);
		offset += deserialize_string(buffer + offset, user_id_);
	
		return offset;
	};

	int Msg_Logon_Response::serialize(char* buffer)
	{
		int offset = 0;
		header_.msg_length_ = message_length();
		offset += header_.serialize(buffer + offset);
		offset += serialize_string(buffer + offset, result_);

		return offset;
	};

	int Msg_Logon_Response::deserialize(char* buffer)
	{
		int offset = 0;

		offset += header_.deserialize(buffer + offset);
		offset += deserialize_string(buffer + offset, result_);

		return offset;
	};

	int Msg_Send_News::serialize(char* buffer)
	{
		int offset = 0;
		header_.msg_length_ = message_length();
		offset += header_.serialize(buffer + offset);
		offset += serialize_string(buffer + offset, info_);

		return offset;
	};

	int Msg_Send_News::deserialize(char* buffer)
	{
		int offset = 0;

		offset += header_.deserialize(buffer + offset);
		offset += deserialize_string(buffer + offset, info_);

		return offset;
	};

	int Msg_Gamers_List::serialize(char* buffer)
	{
		int offset = 0;
		header_.msg_length_ = message_length();
		offset += header_.serialize(buffer + offset);

		offset += serialize_int(buffer + offset, int(gamers_.size()));
		std::vector<std::string>::const_iterator it;
		for (it = gamers_.begin(); it != gamers_.end(); it++)
			offset += serialize_string(buffer + offset, *it);

		return offset;
	};

	int Msg_Gamers_List::deserialize(char* buffer)
	{
		int offset = 0;
		int	tmp_int;

		offset += header_.deserialize(buffer + offset);

		offset += deserialize_int(buffer + offset, tmp_int);
		for (int i = 0; i < tmp_int; i++)
		{
			std::string tmp_str;
			offset += deserialize_string(buffer + offset, tmp_str);
			gamers_.push_back(tmp_str);
		}

		return offset;
	};

	int Msg_Short_Report::serialize(char* buffer)
	{
		int offset = 0;
		header_.msg_length_ = message_length();
		offset += header_.serialize(buffer + offset);
		offset += serialize_string(buffer + offset, user_id_);
		offset += serialize_int(buffer + offset, int(report_type_));

		return offset;
	};

	int Msg_Short_Report::deserialize(char* buffer)
	{
		int offset = 0;
		int	tmp_int;

		offset += header_.deserialize(buffer + offset);
		offset += deserialize_string(buffer + offset, user_id_);
		offset += deserialize_int(buffer + offset, tmp_int);
		report_type_ = short_report_t(tmp_int);

		return offset;
	};

	int Msg_Choose_Color::serialize(char* buffer)
	{
		int offset = 0;
		header_.msg_length_ = message_length();
		offset += header_.serialize(buffer + offset);
		offset += serialize_int(buffer + offset, int(color_));

		return offset;
	};

	int Msg_Choose_Color::deserialize(char* buffer)
	{
		int offset = 0;
		int	tmp_int;

		offset += header_.deserialize(buffer + offset);
		offset += deserialize_int(buffer + offset, tmp_int);
		color_ = colors_t(tmp_int);

		return offset;
	};

	int Msg_Initial_Cards_Deals::serialize(char* buffer)
	{
		int offset = 0;
		header_.msg_length_ = message_length();
		offset += header_.serialize(buffer + offset);

		std::vector<cards_t>::iterator	it_card;
		for (it_card = cards_.begin(); it_card < cards_.end(); it_card++)
			offset += serialize_int(buffer + offset, int(*it_card));

		return offset;
	};

	int Msg_Initial_Cards_Deals::deserialize(char* buffer)
	{
		int offset = 0;
		int	tmp_int;

		offset += header_.deserialize(buffer + offset);
		for (int i = 0; i < 5; i++)
		{
			offset += deserialize_int(buffer + offset, tmp_int);
			cards_.push_back(cards_t(tmp_int));
		}

		return offset;
	};

	int Msg_Server_Command::serialize(char* buffer)
	{
		int offset = 0;
		header_.msg_length_ = message_length();
		offset += header_.serialize(buffer + offset);
		offset += serialize_int(buffer + offset, int(command_));

		return offset;
	};

	int Msg_Server_Command::deserialize(char* buffer)
	{
		int offset = 0;
		int	tmp_int;

		offset += header_.deserialize(buffer + offset);
		offset += deserialize_int(buffer + offset, tmp_int);
		command_ = server_command_t(tmp_int);

		return offset;
	};

	int Msg_Make_Move::serialize(char* buffer)
	{
		int offset = 0;
		header_.msg_length_ = message_length();
		offset += header_.serialize(buffer + offset);
		offset += serialize_string(buffer + offset, user_id_);
		offset += serialize_int(buffer + offset, int(card_));
		offset += serialize_int(buffer + offset, int(turtle_color_));

		return offset;
	};

	int Msg_Make_Move::deserialize(char* buffer)
	{
		int offset = 0;
		int	tmp_int;

		offset += header_.deserialize(buffer + offset);
		offset += deserialize_string(buffer + offset, user_id_);
		offset += deserialize_int(buffer + offset, tmp_int);
		card_ = (messages::cards_t)tmp_int;
		offset += deserialize_int(buffer + offset, tmp_int);
		turtle_color_ = (messages::colors_t)tmp_int;

		return offset;
	};

	int Msg_Move_Ack::serialize(char* buffer)
	{
		int offset = 0;
		int	i;

		header_.msg_length_ = message_length();
		offset += header_.serialize(buffer + offset);
		offset += serialize_string(buffer + offset, user_id_);
		offset += serialize_int(buffer + offset, int(color_));
		offset += serialize_int(buffer + offset, steps_);

		for ( i=0 ; i< 5 ; i++)
			offset += serialize_int(buffer + offset, int(snapshot_.start_position_[i]));

		for (i = 0; i < 11; i++)
		{
			offset += serialize_int(buffer + offset, int(snapshot_.rocks_[i].size()));

			std::vector<colors_t>::iterator it;
			for ( it = snapshot_.rocks_[i].begin() ; it != snapshot_.rocks_[i].end() ; it++)
				offset += serialize_int(buffer + offset, int(*it));
		}

		return offset;
	};

	int Msg_Move_Ack::deserialize(char* buffer)
	{
		int offset = 0;
		int	tmp_int, i, n_turtles_here, j;

		offset += header_.deserialize(buffer + offset);
		offset += deserialize_string(buffer + offset, user_id_);

		offset += deserialize_int(buffer + offset, tmp_int);
		color_ = (messages::colors_t)tmp_int;
		offset += deserialize_int(buffer + offset, steps_);

		for (i = 0; i < 5; i++)
		{
			offset += deserialize_int(buffer + offset, tmp_int);
			snapshot_.start_position_[i] = colors_t(tmp_int);
		}

		for (i = 0; i < 11; i++)
		{
			offset += deserialize_int(buffer + offset, n_turtles_here);
			for (j = 0; j < n_turtles_here; j++)
			{
				offset += deserialize_int(buffer + offset, tmp_int);
				snapshot_.rocks_[i].push_back(colors_t(tmp_int));
			}
		}

		return offset;
	};

	int Msg_New_Card::serialize(char* buffer)
	{
		int offset = 0;
		header_.msg_length_ = message_length();
		offset += header_.serialize(buffer + offset);
		offset += serialize_int(buffer + offset, int(card_));

		return offset;
	};

	int Msg_New_Card::deserialize(char* buffer)
	{
		int offset = 0;
		int	tmp_int;

		offset += header_.deserialize(buffer + offset);
		offset += deserialize_int(buffer + offset, tmp_int);
		card_ = cards_t(tmp_int);

		return offset;
	};

	int Msg_End_Game::serialize(char* buffer)
	{
		int offset = 0;
		header_.msg_length_ = message_length();
		offset += header_.serialize(buffer + offset);
		offset += serialize_int(buffer + offset, int(winner_color_));
		offset += serialize_int(buffer + offset, int(gamers_colors_.size()));

		std::map<messages::colors_t, std::string>::iterator	it;
		for (it = gamers_colors_.begin(); it != gamers_colors_.end(); it++)
		{
			offset += serialize_int(buffer + offset, int(it->first));
			offset += serialize_string(buffer + offset, it->second);
		}

		return offset;
	};

	int Msg_End_Game::deserialize(char* buffer)
	{
		int offset = 0;
		int	tmp_int, tmp_int2;

		offset += header_.deserialize(buffer + offset);
		offset += deserialize_int(buffer + offset, tmp_int2);
		winner_color_ = messages::colors_t(tmp_int2);

		offset += deserialize_int(buffer + offset, tmp_int);

		for (int i = 0; i < tmp_int; i++)
		{
			std::string tmp_str;

			offset += deserialize_int(buffer + offset, tmp_int2);
			offset += deserialize_string(buffer + offset, tmp_str);
			gamers_colors_[messages::colors_t(tmp_int2)] = tmp_str;
		}

		return offset;
	};

};
