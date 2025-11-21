#pragma once
#include <string>
#include <vector>
#include <map>

namespace messages {
	enum class message_type_t
	{
		unknown,
		empty,
		invitation_to_logon,
		logon_request,
		logon_response,
		gamers_list,
		send_news,
		short_report,
		choose_color,
		initial_cards_deal,
		server_command,
		make_move,
		move_ack,
		new_card,
		end_game,
	};

	enum class short_report_t
	{
		unknown,
		ready_to_start,
		ready_for_cards_deal,
		ready_for_next_move,
		move_done,
		move_skipped,
	};

	enum class server_command_t
	{
		unknown,
		your_turn_now,
	};

	enum class colors_t
	{
		unknown,
		blue,
		green,
		magenta,
		red,
		yellow
	};

	enum class cards_t
	{
		c_0_unknown,
		c_1_blue_plusplus,		// 1
		c_1_green_plusplus,		// 1
		c_1_magenta_plusplus,	// 1
		c_1_red_plusplus,		// 1
		c_1_yellow_plusplus,	// 1
		c_2_blue_plus,			// 5
		c_2_green_plus,			// 5
		c_2_magenta_plus,		// 5
		c_2_red_plus,			// 5
		c_2_yellow_plus,		// 5
		c_3_blue_minus,			// 2
		c_3_green_minus,		// 2
		c_3_magenta_minus,		// 2
		c_3_red_minus,			// 2
		c_3_yellow_minus,		// 2
		c_4_nocolor_up,			// 3
		c_5_nocolor_upup,		// 2
		c_6_nocolor_plus,		// 5
		c_7_nocolor_minus,		// 2
	};

	colors_t card_color(cards_t card);
	int card_steps(cards_t card);

	struct race_track_t
	{
		race_track_t()
		{
			init();
		};

		void init()
		{
			winner_ = colors_t::unknown;

			start_position_[0] = colors_t::blue;
			start_position_[1] = colors_t::green;
			start_position_[2] = colors_t::magenta;
			start_position_[3] = colors_t::red;
			start_position_[4] = colors_t::yellow;

			for ( int i = 0 ; i < 11 ; i++)
				rocks_[i].clear();
		};

		int turtle_position(colors_t color)
		{
			int i;
			for (i = 0; i < 5; i++)
			{
				if (start_position_[i] == color)
					return 0;
			}

			i = 0;
			for (i = 0; i < 11; i++)
			{
				std::vector<colors_t>::iterator	it_color;
				for (it_color = rocks_[i].begin(); it_color != rocks_[i].end(); it_color++)
				{
					if (*it_color == color)
						return i + 1;
				}
			}

			return -1;
		};

		bool can_move(int start_pos, int steps)
		{
			return (start_pos + steps >= 0);
		};

		void check_for_winner()
		{
			if (rocks_[10].size())
			{
				winner_ = *rocks_[10].begin();
			}
		}

		bool make_move(cards_t card, colors_t played_color)
		{
			colors_t	color = messages::card_color(card);
			if (color == colors_t::unknown)
				color = played_color;

			int			steps = messages::card_steps(card);
			int			start = turtle_position(color);

			if (start == -1 || color == colors_t::unknown || !steps || !can_move(start, steps) )
				return false;

			if (!start)
			{
				for (int i = 0; i < 5; i++)
				{
					if (start_position_[i] == color)
					{
						start_position_[i] = colors_t::unknown;
						rocks_[steps - 1].push_back(color);
						break;
					}
				}

				return true;
			}
			else
			{
				int i_turtle;
				for (i_turtle = 0; i_turtle < int(rocks_[start - 1].size()); i_turtle++)
				{
					if (rocks_[start - 1][i_turtle] == color)
						break;
				}

				if (start + steps > 11)
				{
					steps = 11 - start;
					//Побьеда (с)
					//winner_ = rocks_[start - 1][i_turtle];
					//return true;
				}

//				else
				{
					for (; i_turtle < int(rocks_[start - 1].size()); )
					{
						if (start + steps > 0)
							rocks_[start -1 + steps].push_back(rocks_[start - 1][i_turtle]);
						else
						{
							switch (rocks_[start - 1][i_turtle])
							{
							case colors_t::blue:	start_position_[0] = colors_t::blue;	break;
							case colors_t::green:	start_position_[1] = colors_t::green;	break;
							case colors_t::magenta:	start_position_[2] = colors_t::magenta;	break;
							case colors_t::red:		start_position_[3] = colors_t::red;		break;
							case colors_t::yellow:	start_position_[4] = colors_t::yellow;	break;
							}
						}

						rocks_[start - 1].erase(rocks_[start - 1].begin() + i_turtle);
					}
				}
			}

			return true;
		};

		colors_t				start_position_[5];
		std::vector<colors_t>	rocks_[11];
		colors_t				winner_;
	};


	class Message
	{
	protected:
		static int serialize_int(char* buffer, int val);
		static int deserialize_int(char* buffer, int& val);
		static int serialize_int64(char* buffer, __int64& val);
		static int deserialize_int64(char* buffer, __int64& val);
		static int serialize_char_array(char* buffer, const char* val, int len);
		static int deserialize_char_array(char* buffer, char* val, int len);
		static int serialize_string(char* buffer, const std::string& _from);
		static int deserialize_string(char* buffer, std::string& _to);

		struct message_header_t
		{
			message_header_t()
				: msg_length_(0)
				, app_id_("TurtleRace")
				, protocol_version_(251023)
				, msg_type_(message_type_t::unknown)
			{
			};

			int serialize(char* buffer);
			int deserialize(char* buffer);

			int length()
			{
				return int(
					sizeof(int) +		// app_id length size
					app_id_.length() +	// app_id length
					sizeof(__int64) +	// protocol version
					sizeof(int) +		// msg_type
					sizeof(int)			// msg length
					);
			};

			std::string		app_id_;
			__int64			protocol_version_;
			message_type_t	msg_type_;
			int				msg_length_;
		};

		message_header_t	header_;
	public:

		Message() {
			header_.msg_type_ = message_type_t::empty;
		};

		Message(message_type_t type) {
			header_.msg_type_ = type;
		};

		virtual ~Message()
		{
		};

		const std::string& app_id() const { return header_.app_id_; };
		virtual int serialize(char* buffer);
		virtual int deserialize(char* buffer);

		message_type_t	message_type() { return header_.msg_type_; };
		__int64			protocol_version() { return header_.protocol_version_; };

		virtual int message_length() { return 0; };
		int full_message_length() { return int(header_.length() + header_.msg_length_); };
	};

	Message* create_message(message_type_t type);

	class Msg_Invitation_To_Logon : public Message
	{
	public:
		Msg_Invitation_To_Logon()
			: Message(message_type_t::invitation_to_logon)
		{
		}

		virtual ~Msg_Invitation_To_Logon()
		{
		};

		virtual int serialize(char* buffer);
		virtual int deserialize(char* buffer);

		virtual int message_length()
		{
			return 0;
		}

	private:
	};

	class Msg_Logon_Request : public Message
	{
	public:
		Msg_Logon_Request(const std::string& user_id)
			: Message(message_type_t::logon_request)
		{
			user_id_ = user_id;
		}

		Msg_Logon_Request()
			: Message(message_type_t::logon_request)
		{
		}

		virtual ~Msg_Logon_Request()
		{
		};

		virtual int serialize(char* buffer);
		virtual int deserialize(char* buffer);

		virtual int message_length()
		{
			return int(
				sizeof(int) +		// user_id_ length size
				user_id_.length()	// user_id_ length
				);
		}

		const std::string& user_id() { return user_id_; }
		__int64	version() { return header_.protocol_version_; };
	private:
		std::string	user_id_;
	};

	class Msg_Logon_Response : public Message
	{
	public:
		Msg_Logon_Response(std::string result)
			: Message(message_type_t::logon_response)
			, result_(result)
		{
		}

		Msg_Logon_Response()
			: Message(message_type_t::logon_response)
		{
		}

		virtual ~Msg_Logon_Response()
		{
		};

		virtual int serialize(char* buffer);
		virtual int deserialize(char* buffer);
		virtual int message_length()
		{
			return int(
				sizeof(int) +		// result_ length size
				result_.length()	// result_ length
				);
		}

		const std::string& result() { return result_; };
	private:
		std::string	result_;
	};

	class Msg_Send_News : public Message
	{
	public:
		Msg_Send_News(std::string info)
			: Message(message_type_t::send_news)
			, info_(info)
		{
		}

		Msg_Send_News()
			: Message(message_type_t::send_news)
		{
		}

		virtual ~Msg_Send_News()
		{
		};

		virtual int serialize(char* buffer);
		virtual int deserialize(char* buffer);
		virtual int message_length()
		{
			return int(
				sizeof(int) +		// result_ length size
				info_.length()	// result_ length
				);
		}

		const std::string& info() { return info_; };
	private:
		std::string	info_;
	};

	class Msg_Gamers_List : public Message
	{
	public:
		Msg_Gamers_List()
			: Message(message_type_t::gamers_list)
		{
		}

		virtual ~Msg_Gamers_List()
		{
		};

		void add_gamer(const std::string& name)
		{
			gamers_.push_back(name);
		};

		virtual int serialize(char* buffer);
		virtual int deserialize(char* buffer);
		virtual int message_length()
		{
			int length = sizeof(int);		// number of gamers length size
			std::vector<std::string>::const_iterator it;
			for (it = gamers_.begin(); it != gamers_.end(); it++)
				length += (int(sizeof(int) + (*it).length()));	// name length size + game length

			return length;
		}

		const std::vector<std::string> gamers() { return gamers_; };
	private:
		std::vector<std::string>	gamers_;
	};

	class Msg_Short_Report : public Message
	{
	public:
		Msg_Short_Report(std::string user_id, short_report_t type)
			: Message(message_type_t::short_report)
			, user_id_(user_id)
			, report_type_(type)
		{
		}

		Msg_Short_Report()
			: Message(message_type_t::short_report)
			, report_type_(short_report_t::unknown)
		{
		}

		virtual ~Msg_Short_Report()
		{
		};

		virtual int serialize(char* buffer);
		virtual int deserialize(char* buffer);
		virtual int message_length()
		{
			return int(
				sizeof(int) +		// user_id_ length size
				user_id_.length() +	// user_id_ length
				sizeof(int) 		// report_type_ size
				);
		}

		const std::string& user_id() { return user_id_; };
		short_report_t report_type() { return report_type_; };
	private:
		std::string	user_id_;
		short_report_t report_type_;
	};

	class Msg_Choose_Color : public Message
	{
	public:
		Msg_Choose_Color(colors_t color)
			: Message(message_type_t::choose_color)
			, color_(color)
		{
		}

		Msg_Choose_Color()
			: Message(message_type_t::choose_color)
			, color_(colors_t::unknown)
		{
		}

		virtual ~Msg_Choose_Color()
		{
		};

		virtual int serialize(char* buffer);
		virtual int deserialize(char* buffer);
		virtual int message_length()
		{
			return int(
				sizeof(int)		// color_ size
				);
		}

		colors_t color() { return color_; };
	private:
		colors_t color_;
	};

	class Msg_New_Card : public Message
	{
	public:
		Msg_New_Card(cards_t card)
			: Message(message_type_t::new_card)
			, card_(card)
		{
		}

		Msg_New_Card()
			: Message(message_type_t::new_card)
			, card_(cards_t::c_0_unknown)
		{
		}

		virtual ~Msg_New_Card()
		{
		};

		virtual int serialize(char* buffer);
		virtual int deserialize(char* buffer);
		virtual int message_length()
		{
			return int(
				sizeof(int)		// card_ size
				);
		}

		cards_t card() { return card_; };
	private:
		cards_t card_;
	};

	class Msg_Initial_Cards_Deals : public Message
	{
	public:
		Msg_Initial_Cards_Deals()
			: Message(message_type_t::initial_cards_deal)
		{
		}

		virtual ~Msg_Initial_Cards_Deals()
		{
		};

		void add_card(cards_t card)
		{
			cards_.push_back(card);
		};

		virtual int serialize(char* buffer);
		virtual int deserialize(char* buffer);
		virtual int message_length()
		{
			int length = int(5 *sizeof(int));		// five cards (int)
			return length;
		}

		const std::vector<cards_t> cards() { return cards_; };
	private:
		std::vector<cards_t>	cards_;
	};

	class Msg_Server_Command : public Message
	{
	public:
		Msg_Server_Command(server_command_t type)
			: Message(message_type_t::server_command)
			, command_(type)
		{
		}

		Msg_Server_Command()
			: Message(message_type_t::server_command)
			, command_(server_command_t::unknown)
		{
		}

		virtual ~Msg_Server_Command()
		{
		};

		virtual int serialize(char* buffer);
		virtual int deserialize(char* buffer);
		virtual int message_length()
		{
			return int(
				sizeof(int) 		// command_ size
				);
		}

		server_command_t command_type() { return command_; };
	private:
		server_command_t command_;
	};

	class Msg_Make_Move : public Message
	{
	public:
		Msg_Make_Move(std::string user_id, cards_t card, colors_t turtle_color)
			: Message(message_type_t::make_move)
			, user_id_(user_id)
			, card_(card)
			, turtle_color_(turtle_color)
		{
		}

		Msg_Make_Move()
			: Message(message_type_t::make_move)
			, card_(messages::cards_t::c_0_unknown)
			, turtle_color_(messages::colors_t::unknown)
		{
		}

		virtual ~Msg_Make_Move()
		{
		};

		virtual int serialize(char* buffer);
		virtual int deserialize(char* buffer);
		virtual int message_length()
		{
			return int(
				sizeof(int) + 		// user_id_ length size
				user_id_.length() +	// user_id_ length
				sizeof(int) + 		// card_ size
				sizeof(int) 		// turtle_color_ size
				);
		}

		std::string user_id()				{ return user_id_; };
		messages::cards_t card()			{ return card_; };
		messages::colors_t turtle_color()	{ return turtle_color_; };
	private:
		std::string			user_id_;
		messages::cards_t	card_;
		messages::colors_t	turtle_color_;
	};

	class Msg_Move_Ack : public Message
	{
	public:
		Msg_Move_Ack(std::string& user_id, colors_t color, int steps, race_track_t& snapshot)
			: Message(message_type_t::move_ack)
			, user_id_(user_id)
			, color_(color)
			, steps_(steps)
			, snapshot_(snapshot)
		{
		}

		Msg_Move_Ack()
			: Message(message_type_t::move_ack)
			, color_(colors_t::unknown)
			, steps_(0)
		{
		}

		virtual ~Msg_Move_Ack()
		{
		};

		virtual int serialize(char* buffer);
		virtual int deserialize(char* buffer);
		virtual int message_length()
		{
			int n_turtles_on_field = 0;
			for (int i = 0; i < 11; i++)
				n_turtles_on_field += (int)snapshot_.rocks_[i].size();

			return int(
				sizeof(int) + 		// user_id_ length size
				user_id_.length() +	// user_id_ length
				sizeof(int) + 		// color_ size
				sizeof(int) + 		// steps_ size
				5 * sizeof(int) +	// start_position
				11 * sizeof(int) +	// vector size for each rock
				n_turtles_on_field * sizeof(int)		// total cards
				);
		}

		std::string			user_id()	{ return user_id_; };
		messages::colors_t	color()		{ return color_; };
		int					steps()		{ return steps_; };
		race_track_t&		snapshot()	{ return snapshot_; }
	private:
		std::string			user_id_;
		colors_t			color_;
		int					steps_;
		race_track_t		snapshot_;
	};

	class Msg_End_Game : public Message
	{
	public:
		Msg_End_Game(messages::colors_t color)
			: Message(message_type_t::end_game)
			, winner_color_(color)
		{
		}

		Msg_End_Game()
			: Message(message_type_t::end_game)
		{
		}

		virtual ~Msg_End_Game()
		{
		};

		virtual int serialize(char* buffer);
		virtual int deserialize(char* buffer);
		virtual int message_length()
		{
			int length = int(sizeof(int));	// Winner Color
			
			length += int(sizeof(int));								// total qty of gamers
			std::map<messages::colors_t, std::string>::iterator	it;
			for (it = gamers_colors_.begin(); it != gamers_colors_.end(); it++)
				length += int(sizeof(int) + sizeof(int) + it->second.length());	// Gamer ID length + GamerID + color

			return length;
		}

		void				add_gamer_color(messages::colors_t color, const std::string& gamer) { gamers_colors_[color] = gamer; };
		messages::colors_t	winner_color() { return winner_color_; };
		std::map<messages::colors_t, std::string>& gamers_colors() { return gamers_colors_; };

	private:
		messages::colors_t							winner_color_;
		std::map<messages::colors_t, std::string>	gamers_colors_;
	};

};