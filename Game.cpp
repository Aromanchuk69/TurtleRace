#include "Game.h"

CGame::CGame()
	: local_game_(false), game_stage_(game_stage_t::not_started_yet), pDlg_(NULL)
{
	information_ = "Начните новую игру или подключитесь к существующей";
}

CGame::~CGame()
{
}

void CGame::init_configuration()
{
	if (configuration_.load("turtlerace.cfg"))
		AfxMessageBox("Ошибка чтения файла конфигурации!", MB_ICONINFORMATION);
}

void CGame::clear_game_info()
{
	if (!information_.empty())
	{
		information_ = "";
		pDlg_->Invalidate(FALSE);
	}
}

void CGame::log_event(std::string event)
{
	information_ = event;
	pDlg_->Invalidate(FALSE);
}

void CGame::server_started()
{
	clear_game_info();

	game_client_.start_client("127.0.0.1", configuration_.port(), configuration_.user(), this);
}

void CGame::server_start_failed(std::string reason)
{
	game_stage_ = game_stage_t::not_started_yet;

	information_ = reason;
	pDlg_->Invalidate(FALSE);
}

void CGame::client_started()
{
	clear_game_info();

	log_event("Connected to server");
}

void CGame::client_start_failed(std::string reason)
{
	game_stage_ = game_stage_t::not_started_yet;

	information_ = reason;
	pDlg_->Invalidate(FALSE);
}

void CGame::create_game(int& port, const std::string& login)
{
	configuration_.port(port);
	configuration_.user(login);

	game_stage_ = game_stage_t::waiting_for_connect;
	game_server_.start_server(configuration_.port(), this);

	configuration_.save();
}

void CGame::join_game(const std::string& ip_address, int& port, const std::string& login)
{
	configuration_.ip_address(ip_address);
	configuration_.port(port);
	configuration_.user(login);

	game_stage_ = game_stage_t::waiting_for_connect;
	game_client_.start_client(configuration_.ip_address(), configuration_.port(), configuration_.user(), this);

	configuration_.save();
}

void CGame::logon_accepted(std::string name)
{
	drawer_.set_gamer_name(name);

	if (!game_server_.is_started())
		game_client_.ready_to_start();
	
	game_stage_ = game_stage_t::waiting_for_press_start;

	clear_game_info();
}

void CGame::logon_rejected(std::string name, std::string reason)
{
	game_stage_ = game_stage_t::not_started_yet;

	if (game_stage_ != game_stage_t::game_finished)
	{
		information_ = reason;
		pDlg_->Invalidate(FALSE);
	}

	game_server_.stop_server();
}

void CGame::update_gamers_list(const std::vector<std::string>& gamers)
{
	//if (gamers.size() > 1 && game_server_.is_started() )
	//	information_ = "Для продолжения нажмите кнопку \"Старт\"";

	drawer_.update_gamers_list(gamers);
	pDlg_->Invalidate(FALSE);
}

bool CGame::game_in_progress() const
{
	return ((game_stage_ != game_stage_t::not_started_yet) && (game_stage_ != game_stage_t::finished));
}

void CGame::Draw(CDC* pDC, CRect& client)
{
	switch (game_stage_)
	{
	default:
		drawer_.draw_empty_field(pDC);
		break;
	case game_stage_t::not_started_yet:
	case game_stage_t::waiting_for_connect:
		drawer_.draw_empty_field(pDC);
		drawer_.draw_title_turtles(pDC);
		break;
	case game_stage_t::waiting_for_press_start:
		drawer_.draw_empty_field(pDC);
		drawer_.draw_title_turtles(pDC);
		drawer_.write_gamers_list(pDC, client);
		if (game_server_.is_started())
			drawer_.draw_start_button(pDC, client);
		break;
	case game_stage_t::waiting_for_all:
		drawer_.draw_empty_field(pDC);
		drawer_.draw_title_turtles(pDC);
		drawer_.write_gamers_list(pDC, client);
	case game_stage_t::prepare_to_color_choosing:
	case game_stage_t::color_choosing:
	case game_stage_t::decreasing_color_cards:
		drawer_.draw_empty_field(pDC);
		drawer_.draw_title_turtles(pDC);
		drawer_.draw_cards_for_choose(pDC, client);
		break;
	case game_stage_t::increasing_choosen_card:
		drawer_.draw_empty_field(pDC);
		drawer_.draw_title_turtles(pDC);
		drawer_.draw_choosen_card(pDC, client);
		break;
	case game_stage_t::preparing_to_cards_deal:	 /// <<<-- WE ARE HERE NOW !!!
		drawer_.draw_empty_field(pDC);
		drawer_.draw_title_turtles(pDC, true);
		drawer_.draw_game_objects(pDC);
		break;
	case game_stage_t::waiting_for_cards:
		drawer_.draw_empty_field(pDC);
		drawer_.draw_game_objects(pDC);
		break;
	case game_stage_t::accepting_cards_deal:
		drawer_.draw_empty_field(pDC);
		drawer_.draw_game_objects(pDC);
		drawer_.draw_playing_cards(pDC, client);
		break;
	case game_stage_t::waiting_for_next_turn:
	case game_stage_t::my_turn_now:
	case game_stage_t::waiting_for_move_ack:
	case game_stage_t::removing_chosen_card:
	case game_stage_t::making_move:
	case game_stage_t::waiting_for_result:
	case game_stage_t::color_clarify_preparation:
	case game_stage_t::color_clarification:
	case game_stage_t::color_clarify_finishing:
	case game_stage_t::color_clarify_cancelation:
		drawer_.draw_empty_field(pDC);
		drawer_.draw_game_objects(pDC);
		drawer_.draw_playing_cards(pDC, client);
		break;
	case game_stage_t::waiting_for_skip_turn:
		drawer_.draw_empty_field(pDC);
		drawer_.draw_game_objects(pDC);
		drawer_.draw_playing_cards(pDC, client);
		drawer_.draw_skip_button(pDC);
		break;
	case game_stage_t::ending_game:
		drawer_.draw_empty_field(pDC);
		drawer_.draw_game_ending(pDC);
		break;
	case game_stage_t::game_finished:
		drawer_.draw_empty_field(pDC);
		drawer_.draw_game_ending(pDC);
		drawer_.draw_end_button(pDC);
		break;
	}

	if (!information_.empty())
		drawer_.write_game_info(pDC, information_.c_str());
}

void CGame::on_timer()
{
	switch (game_stage_)
	{
	default:
		break;
	case game_stage_t::not_started_yet:
	case game_stage_t::waiting_for_connect:
	case game_stage_t::waiting_for_press_start:
	case game_stage_t::waiting_for_all:
		drawer_.title_turtles_step();
		break;
	case game_stage_t::prepare_to_color_choosing:
		drawer_.title_turtles_step();
		if (!drawer_.cards_for_choose_increase())
		{
			game_stage_ = game_stage_t::color_choosing;
			information_ = "Выберите себе черепашку";
		}
		break;
	case game_stage_t::color_choosing:
		drawer_.title_turtles_step();
		break;
	case game_stage_t::decreasing_color_cards:
		drawer_.title_turtles_step();
		if (!drawer_.cards_for_choose_decrease())
		{
			game_stage_ = game_stage_t::increasing_choosen_card;
			information_ = "Цвет вашей черепашки выбран";
		}
		break;
	case game_stage_t::increasing_choosen_card:
		drawer_.title_turtles_step();
		if (!drawer_.cards_for_choose_increase())
		{
			game_stage_ = game_stage_t::preparing_to_cards_deal;
			drawer_.title_turtles_start_away();
			drawer_.init_game_objects();
			// Init Game Objects
			information_ = "";
		}
		break;
	case game_stage_t::preparing_to_cards_deal:
		drawer_.title_turtles_step(true);
		if (!drawer_.step_game_objects())
		{
			game_stage_ = game_stage_t::waiting_for_cards;
			game_client_.waiting_for_cards(true);
		}
		break;
	case game_stage_t::accepting_cards_deal:
		if (!drawer_.increase_playing_cards())
		{
			game_stage_ = game_stage_t::waiting_for_next_turn;
			game_client_.ready_for_next_move();
			information_ = "Ожидаем остальных участников";
		}
		break;
	case game_stage_t::removing_chosen_card:
		if (!drawer_.disappear_choosen_card())
		{
			messages::colors_t	color;
			messages::cards_t	card = drawer_.get_choosen_card(color);
			drawer_.end_card_disappearing();
			game_stage_ = game_stage_t::waiting_for_move_ack;
			information_ = "Ход сделан";
			game_client_.my_move(card, color);
		}
		break;
	case game_stage_t::making_move:
		if (!drawer_.increase_playing_cards())	// СЮДЫ еще перемещение черепашек!!!
		{
			if (drawer_.is_turn_possible())
			{
				game_client_.move_done();
				game_stage_ = game_stage_t::waiting_for_result;
			}
			else
				game_stage_ = game_stage_t::waiting_for_skip_turn;
		}
		break;
	case game_stage_t::color_clarify_preparation:
		if (!drawer_.color_clarification_begin())
		{
			game_stage_ = game_stage_t::color_clarification;
		}
		break;
	case game_stage_t::color_clarify_finishing:
		if (!drawer_.color_clarification_end())
		{
			messages::colors_t	color;
			messages::cards_t	card = drawer_.get_choosen_card(color);
			game_stage_ = game_stage_t::waiting_for_move_ack;
			information_ = "Ход сделан";
			game_client_.my_move(card, color);
		}
		break;
	case game_stage_t::color_clarify_cancelation:
		if (!drawer_.color_clarification_end())
			your_turn_now();
		break;
	case game_stage_t::ending_game:
		if (!drawer_.step_game_ending())
		{
			game_stage_ = game_stage_t::game_finished;
			game_client_.stop_client();
			game_server_.stop_server();
		}
		break;
	case game_stage_t::game_finished:
		{
			drawer_.step_cabbage();
		}
		break;
	}

	pDlg_->Invalidate(FALSE);
}

void CGame::set_turtle_color(messages::colors_t color)
{
	drawer_.set_my_turtle_color(color);
	game_stage_ = game_stage_t::prepare_to_color_choosing;
	information_ = "";
	drawer_.init_cards_for_choose();
	pDlg_->Invalidate(FALSE);
}

void CGame::cards_deal(const std::vector<messages::cards_t>& cards)
{
	drawer_.init_playing_cards(cards);
	game_stage_ = game_stage_t::accepting_cards_deal;
	information_ = "";
	pDlg_->Invalidate(FALSE);
}

void CGame::your_turn_now()
{
	set_card_info(messages::cards_t::c_0_unknown);

	if (drawer_.is_turn_possible())
		game_stage_ = game_stage_t::my_turn_now;
	else
		game_stage_ = game_stage_t::waiting_for_skip_turn;
}

void CGame::move_ack(std::string& name, messages::colors_t color, int steps, messages::race_track_t& snapshot)
{
	drawer_.generate_turtle_move(color, steps);

	drawer_.set_track(snapshot);

	game_stage_ = game_stage_t::making_move;

	pDlg_->Invalidate(FALSE);
}

void CGame::new_card(messages::cards_t card)
{
	drawer_.new_card(card);
}

void CGame::game_over(messages::colors_t winner_color, const std::map<messages::colors_t, std::string>& gamers_colors)
{
	drawer_.init_game_ending(winner_color, gamers_colors);
	game_stage_ = game_stage_t::ending_game;
	information_ = "";
}

void CGame::on_mouse_click(CPoint point)
{
	switch (game_stage_)
	{
	default:
		break;
	case game_stage_t::waiting_for_press_start:
		if (game_server_.is_started() && drawer_.is_start_pressed(point))
		{
			if (game_client_.ready_to_start())
			{
				game_stage_ = game_stage_t::waiting_for_all;
				information_ = "Ожидаем всех участников";
				pDlg_->Invalidate(FALSE);
			}
		}
		break;
	case game_stage_t::color_choosing:
		if (drawer_.is_color_choosen(point)) 
		{
			information_ = "";
			game_stage_ = game_stage_t::decreasing_color_cards;
			pDlg_->Invalidate(FALSE);
		}
		break;
	case game_stage_t::my_turn_now:
		{
			messages::cards_t	card = drawer_.card_under_cursor(point);
			std::set<messages::colors_t>	turtles_can_move = drawer_.possible_turtles_to_move(card);

			switch (int(turtles_can_move.size()))
			{
			case 0:
			{
				information_ = "Этот ход невозможен";
				pDlg_->Invalidate(FALSE);
			}
			break;
			case 1:
			{	// Цвет определен однозначно
				drawer_.init_card_disappearing(card);
				game_stage_ = game_stage_t::removing_chosen_card;
				information_ = "";
			}
			break;
			default:
			{	// Цвет надо уточнить
				information_ = "Уточните цвет";
				drawer_.init_color_clarification(card, turtles_can_move);
				game_stage_ = game_stage_t::color_clarify_preparation;
			}
			break;
			}
		}
		break;
	case game_stage_t::color_clarification:
		{
			switch ( drawer_.is_color_clarified(point) )
			{
			case 0:
				break;
			case 1:		// Выбрано
				{
					game_stage_ = game_stage_t::color_clarify_finishing;
				}
				break;
			case 2:		// Отмена
				{
					drawer_.discard_choice();
					game_stage_ = game_stage_t::color_clarify_cancelation;
				}
				break;
			}
		}
		break;
	case game_stage_t::waiting_for_skip_turn:
		if (drawer_.is_skip_pressed(point))
		{
			game_client_.move_skipped();
			game_stage_ = game_stage_t::waiting_for_result;
		}
		break;
	case game_stage_t::game_finished:
		if (drawer_.is_start_pressed(point))
			game_stage_ = game_stage_t::not_started_yet;
		information_ = "Начните новую игру или подключитесь к существующей";
		break;
	}
}

void CGame::on_mouse_move(CPoint point)
{
	switch (game_stage_)
	{
	default:
		break;
	case game_stage_t::my_turn_now:
		{
			messages::cards_t	card = drawer_.card_under_cursor(point);
			if (card != messages::cards_t::c_0_unknown)
			{
				if (drawer_.possible_turtles_to_move(card).size())
					set_card_info(card);
				else
				{
					information_ = "Этот ход невозможен";
					pDlg_->Invalidate(FALSE);
				}
			}
		}
	break;
	}
}

void CGame::set_card_info(messages::cards_t card)
{
	std::string old_info = information_;

	switch (card)
	{
	default:
	case messages::cards_t::c_0_unknown:
		information_ = "Ваш ход. Выберите карту.";
		break;
	case messages::cards_t::c_1_blue_plusplus:
		information_ = "Синяя черепаха делает два шага вперед";
		break;
	case messages::cards_t::c_1_green_plusplus:
		information_ = "Зеленая черепаха делает два шага вперед";
		break;
	case messages::cards_t::c_1_magenta_plusplus:
		information_ = "Розовая черепаха делает два шага вперед";
		break;
	case messages::cards_t::c_1_red_plusplus:
		information_ = "Красная черепаха делает два шага вперед";
		break;
	case messages::cards_t::c_1_yellow_plusplus:
		information_ = "Жёлтая черепаха делает два шага вперед";
		break;
	case messages::cards_t::c_2_blue_plus:
		information_ = "Синяя черепаха делает шаг вперед";
		break;
	case messages::cards_t::c_2_green_plus:
		information_ = "Зеленая черепаха делает шаг вперед";
		break;
	case messages::cards_t::c_2_magenta_plus:
		information_ = "Розовая черепаха делает шаг вперед";
		break;
	case messages::cards_t::c_2_red_plus:
		information_ = "Красная черепаха делает шаг вперед";
		break;
	case messages::cards_t::c_2_yellow_plus:
		information_ = "Жёлтая черепаха делает шаг вперед";
		break;
	case messages::cards_t::c_3_blue_minus:
		information_ = "Синяя черепаха делает шаг назад";
		break;
	case messages::cards_t::c_3_green_minus:
		information_ = "Зеленая черепаха делает шаг назад";
		break;
	case messages::cards_t::c_3_magenta_minus:
		information_ = "Розовая черепаха делает шаг назад";
		break;
	case messages::cards_t::c_3_red_minus:
		information_ = "Красная черепаха делает шаг назад";
		break;
	case messages::cards_t::c_3_yellow_minus:
		information_ = "Жёлтая черепаха делает шаг назад";
		break;
	case messages::cards_t::c_4_nocolor_up:
		information_ = "Последняя черепаха делает шаг вперед";
		break;
	case messages::cards_t::c_5_nocolor_upup:
		information_ = "Последняя черепаха делает два шага вперед";
		break;
	case messages::cards_t::c_6_nocolor_plus:
		information_ = "Любая черепаха делает шаг вперед";
		break;
	case messages::cards_t::c_7_nocolor_minus:
		information_ = "Любая черепаха делает шаг назад";
		break;
	}

	if (old_info != information_)
		pDlg_->Invalidate(FALSE);
}
