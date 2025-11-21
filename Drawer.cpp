#include "Drawer.h"
#include "resource.h"

CDrawer::CDrawer()
{
	m_GreenBrush.CreateSolidBrush(RGB(102, 153, 0));
	m_WhiteBrush.CreateSolidBrush(RGB(250, 240, 220));
	m_DarkGrayBrush.CreateSolidBrush(RGB(153, 204, 102));
	m_LightGrayBrush.CreateSolidBrush(RGB(200, 230, 160));

	m_ButtonPen.CreatePen(PS_SOLID, 1, RGB(153, 204, 102));
	m_BorderPen.CreatePen(PS_SOLID, 1, RGB(102, 153, 0));

	m_iGameTextHeight = 50;
	m_GameTextFont.CreateFont(m_iGameTextHeight, 0, 0, 0, FW_THIN, 0, 0, 0, DEFAULT_CHARSET,
		OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, PROOF_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, "Mistral");

	m_iInfoTextHeight = 40;
	m_InfoTextFont.CreateFont(m_iInfoTextHeight, 0, 0, 0, FW_THIN, 0, 0, 0, DEFAULT_CHARSET,
		OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, PROOF_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, "Mistral");

	bm_cabbage_and_.LoadBitmap(IDB_BM_CABBAGE_AND);
	bm_cabbage_or_.LoadBitmap(IDB_BM_CABBAGE_OR);

	bm_running_turtle_and_.LoadBitmap(IDB_BM_TURTLE_RUN_AND);
	bm_running_turtle_blue_.LoadBitmap(IDB_BM_TURTLE_RUN_BLUE);
	bm_running_turtle_green_.LoadBitmap(IDB_BM_TURTLE_RUN_GREEN);
	bm_running_turtle_magenta_.LoadBitmap(IDB_BM_TURTLE_RUN_MAGENTA);
	bm_running_turtle_red_.LoadBitmap(IDB_BM_TURTLE_RUN_RED);
	bm_running_turtle_yellow_.LoadBitmap(IDB_BM_TURTLE_RUN_YELLOW);

	bm_choose_color_white_.LoadBitmap(IDB_BM_CHOOSECOLOR_WHITE);
	bm_choose_color_blue_.LoadBitmap(IDB_BM_CHOOSECOLOR_BLUE);
	bm_choose_color_green_.LoadBitmap(IDB_BM_CHOOSECOLOR_GREEN);
	bm_choose_color_magenta_.LoadBitmap(IDB_BM_CHOOSECOLOR_MAGENTA);
	bm_choose_color_red_.LoadBitmap(IDB_BM_CHOOSECOLOR_RED);
	bm_choose_color_yellow_.LoadBitmap(IDB_BM_CHOOSECOLOR_YELLOW);
	bm_choose_color_cancel_.LoadBitmap(IDB_BM_CHOOSECOLOR_CANCEL);

	bm_blue_plusplus_.LoadBitmap(IDB_BM_CARD_BLUEPLUSPLUS);
	bm_green_plusplus_.LoadBitmap(IDB_BM_CARD_GREENPLUSPLUS);
	bm_magenta_plusplus_.LoadBitmap(IDB_BM_CARD_MAGENTAPLUSPLUS);
	bm_red_plusplus_.LoadBitmap(IDB_BM_CARD_REDPLUSPLUS);
	bm_yellow_plusplus_.LoadBitmap(IDB_BM_CARD_YELLOWPLUSPLUS);
	bm_blue_plus_.LoadBitmap(IDB_BM_CARD_BLUEPLUS);
	bm_green_plus_.LoadBitmap(IDB_BM_CARD_GREENPLUS);
	bm_magenta_plus_.LoadBitmap(IDB_BM_CARD_MAGENTAPLUS);
	bm_red_plus_.LoadBitmap(IDB_BM_CARD_REDPLUS);
	bm_yellow_plus_.LoadBitmap(IDB_BM_CARD_YELLOWPLUS);
	bm_blue_minus_.LoadBitmap(IDB_BM_CARD_BLUEMINUS);
	bm_green_minus_.LoadBitmap(IDB_BM_CARD_GREENMINUS);
	bm_magenta_minus_.LoadBitmap(IDB_BM_CARD_MAGENTAMINUS);
	bm_red_minus_.LoadBitmap(IDB_BM_CARD_REDMINUS);
	bm_yellow_minus_.LoadBitmap(IDB_BM_CARD_YELLOWMINUS);
	bm_nocolor_up_.LoadBitmap(IDB_BM_CARD_WHITEUP);
	bm_nocolor_upup_.LoadBitmap(IDB_BM_CARD_WHITEUPUP);
	bm_nocolor_plus_.LoadBitmap(IDB_BM_CARD_WHITEPLUS);
	bm_nocolor_minus_.LoadBitmap(IDB_BM_CARD_WHITEMINUS);

	bm_stone_and_.LoadBitmap(IDB_BM_STONEFLAT_AND);
	bm_stone_or_.LoadBitmap(IDB_BM_STONEFLAT_OR);

	my_turtle_color_ = messages::colors_t::unknown;
	winner_color_	= messages::colors_t::unknown;
}

CDrawer::~CDrawer()
{
	m_GreenBrush.DeleteObject();
	m_WhiteBrush.DeleteObject();
	m_DarkGrayBrush.DeleteObject();
	m_LightGrayBrush.DeleteObject();

	m_GameTextFont.DeleteObject();
	m_InfoTextFont.DeleteObject();

	bm_cabbage_and_.DeleteObject();
	bm_cabbage_or_.DeleteObject();

	bm_running_turtle_and_.DeleteObject();
	bm_running_turtle_blue_.DeleteObject();
	bm_running_turtle_green_.DeleteObject();
	bm_running_turtle_magenta_.DeleteObject();
	bm_running_turtle_red_.DeleteObject();
	bm_running_turtle_yellow_.DeleteObject();

	bm_choose_color_white_.DeleteObject();
	bm_choose_color_blue_.DeleteObject();
	bm_choose_color_green_.DeleteObject();
	bm_choose_color_magenta_.DeleteObject();
	bm_choose_color_red_.DeleteObject();
	bm_choose_color_yellow_.DeleteObject();

	bm_blue_plusplus_.DeleteObject();
	bm_green_plusplus_.DeleteObject();
	bm_magenta_plusplus_.DeleteObject();
	bm_red_plusplus_.DeleteObject();
	bm_yellow_plusplus_.DeleteObject();
	bm_blue_plus_.DeleteObject();
	bm_green_plus_.DeleteObject();
	bm_magenta_plus_.DeleteObject();
	bm_red_plus_.DeleteObject();
	bm_yellow_plus_.DeleteObject();
	bm_blue_minus_.DeleteObject();
	bm_green_minus_.DeleteObject();
	bm_magenta_minus_.DeleteObject();
	bm_red_minus_.DeleteObject();
	bm_yellow_minus_.DeleteObject();
	bm_nocolor_up_.DeleteObject();
	bm_nocolor_upup_.DeleteObject();
	bm_nocolor_plus_.DeleteObject();
	bm_nocolor_minus_.DeleteObject();
}

void CDrawer::generate_turtle_move(messages::colors_t color, int steps)
{
	int start_position =  game_field_.race_track_.turtle_position(color);
	int end_position = start_position + steps;
	int	i;
	CPoint	from_xy, to_xy;

	if (end_position > 11)
		return;

	spliner_.clear();

	if (!start_position)
	{
		for (i = 0; i < 5; i++)
		{
			if (game_field_.race_track_.start_position_[i] == color)
			{
				int X = 5;
				int Y = game_field_.main_field_rect_.top + 34 + i * 80;
				spliner_.add_point(X, Y);
				from_xy.SetPoint(X, Y);
				break;
			}
		}
	}
	else
	{
		std::vector<messages::colors_t>::iterator it_turtle;
		int j = 0;
		for (it_turtle = game_field_.race_track_.rocks_[start_position - 1].begin(); it_turtle != game_field_.race_track_.rocks_[start_position].end(); it_turtle++)
		{
			if (*it_turtle == color)
			{
				int X = game_field_.main_field_rect_.left + 140 + 120 * (start_position - 1);
				int Y = game_field_.main_field_rect_.top + 55 + 5 * 62 - j * 49;
				spliner_.add_point(X, Y);
				from_xy.SetPoint(X, Y);
				break;
			}
			j++;
		}
	}

	{
		if (end_position)
		{

			int X = game_field_.main_field_rect_.left + 140 + 120 * (end_position - 1);
			int Y = game_field_.main_field_rect_.top + 55 + 5 * 62 - (int(game_field_.race_track_.rocks_[end_position - 1].size())) * 49;
			to_xy.SetPoint(X, Y);
		}
		else
		{
			int X = 5;
			int Y = game_field_.main_field_rect_.CenterPoint().y + 34;
			to_xy.SetPoint(X, Y);
		}
	}

	if (end_position - start_position > 1)
	{
		int X = game_field_.main_field_rect_.left + 140 + 120 * (start_position + 1 - 1);
		int Y = game_field_.main_field_rect_.top + 55 + 5 * 62 - (int(game_field_.race_track_.rocks_[start_position + 1 - 1].size())) * 49;
		Y = min(Y, min(to_xy.y, from_xy.y)) - 49 * 2;
		if (Y < 1)
			Y = 1;

		spliner_.add_point(X, Y);
	}
	else
	{
		int X = from_xy.x + (to_xy.x - from_xy.x) / 2;
		int Y;

		Y = min(to_xy.y, from_xy.y) - 49 * 2;

		if (Y < 1)
			Y = 1;

		spliner_.add_point(X, Y);
	}

	spliner_.add_point(to_xy.x, to_xy.y);

	spliner_.generate_track();

	jumping_turtles_.clear();
	if (!start_position)
	{
		for (i = 0; i < 5; i++)
		{
			if (game_field_.race_track_.start_position_[i] == color)
			{
				jumping_turtles_.push_back(game_field_.race_track_.start_position_[i]);
				game_field_.race_track_.start_position_[i] = messages::colors_t::unknown;
				break;
			}
		}
	}
	else
	{
		for ( i = 0; ; i++)
		{
			if (game_field_.race_track_.rocks_[start_position - 1][i] == color)
				break;
		}

		while (i < game_field_.race_track_.rocks_[start_position - 1].size())
		{
			jumping_turtles_.push_back(game_field_.race_track_.rocks_[start_position - 1][i]);
			game_field_.race_track_.rocks_[start_position - 1].erase(game_field_.race_track_.rocks_[start_position - 1].begin() + i);
		}
	}
}

void CDrawer::test_output(CDC* pDC, CRect& client)
{
	CDC		dcMem;
	dcMem.CreateCompatibleDC(pDC);

	int i;
	for (i = 0; i < 5; i++)
	{
		dcMem.SelectObject(&bm_running_turtle_and_);
		pDC->BitBlt(5 + i * 115, 170, 110, 62, &dcMem, 0, 0, SRCAND);

		switch (i)
		{
		case 0:	dcMem.SelectObject(&bm_running_turtle_blue_); break;
		case 1:	dcMem.SelectObject(&bm_running_turtle_green_); break;
		case 2:	dcMem.SelectObject(&bm_running_turtle_magenta_); break;
		case 3:	dcMem.SelectObject(&bm_running_turtle_red_); break;
		case 4:	dcMem.SelectObject(&bm_running_turtle_yellow_); break;
		}

		pDC->BitBlt(5 + i * 115, 170, 110, 62, &dcMem, 0, 0, SRCINVERT);
	}

	for (i = 0; i < 6; i++)
	{
		switch (i)
		{
		case 0:	dcMem.SelectObject(&bm_choose_color_white_); break;
		case 1:	dcMem.SelectObject(&bm_choose_color_blue_); break;
		case 2:	dcMem.SelectObject(&bm_choose_color_green_); break;
		case 3:	dcMem.SelectObject(&bm_choose_color_magenta_); break;
		case 4:	dcMem.SelectObject(&bm_choose_color_red_); break;
		case 5:	dcMem.SelectObject(&bm_choose_color_yellow_); break;
		}
		
		pDC->StretchBlt(4 + i * 100, 0, 96, 96, &dcMem, 0, 0, 96, 96, SRCCOPY);
	}

	for (i = 0; i < 12; i++)
	{
		switch (i)
		{
		case 0:	dcMem.SelectObject(&bm_blue_plusplus_); break;
		case 1:	dcMem.SelectObject(&bm_blue_plus_); break;
		case 2:	dcMem.SelectObject(&bm_blue_minus_); break;

		case 3:	dcMem.SelectObject(&bm_green_plusplus_); break;
		case 4:	dcMem.SelectObject(&bm_green_plus_); break;
		case 5:	dcMem.SelectObject(&bm_green_minus_); break;

		case 6:	dcMem.SelectObject(&bm_magenta_plusplus_); break;
		case 7:	dcMem.SelectObject(&bm_magenta_plus_); break;
		case 8:	dcMem.SelectObject(&bm_magenta_minus_); break;

		case 9:	dcMem.SelectObject(&bm_red_plusplus_); break;
		case 10:	dcMem.SelectObject(&bm_red_plus_); break;
		case 11:	dcMem.SelectObject(&bm_red_minus_); break;
		}

		pDC->StretchBlt(4 + i * 100, 250, 96, 144, &dcMem, 0, 0, 96, 144, SRCCOPY);
	}

	for (i = 0; i < 7; i++)
	{
		switch (i)
		{
		case 0:	dcMem.SelectObject(&bm_yellow_plusplus_); break;
		case 1:	dcMem.SelectObject(&bm_yellow_plus_); break;
		case 2:	dcMem.SelectObject(&bm_yellow_minus_); break;

		case 3:	dcMem.SelectObject(&bm_nocolor_up_); break;
		case 4:	dcMem.SelectObject(&bm_nocolor_upup_); break;
		case 5:	dcMem.SelectObject(&bm_nocolor_plus_); break;
		case 6:	dcMem.SelectObject(&bm_nocolor_minus_); break;
		}

		pDC->StretchBlt(4 + i * 100, 400, 96, 144, &dcMem, 0, 0, 96, 144, SRCCOPY);
	}
}

void CDrawer::init_client_rectangle(CRect& client)
{
	// Game zones - Main field, Gamer's turtle and cards sites
	game_field_.prepare(client);
	title_runnung_turtles_.init(game_field_.main_field_rect_);
}

void CDrawer::init_game_objects()
{
	for (int i = 0; i < 5; i++)
	{
		if (color_card_chooser_.color_cards_for_choose_[i].choosen_)
		{
			game_field_.prepare_static_oblects_to_place(CPoint(color_card_chooser_.color_cards_for_choose_[i].x, color_card_chooser_.color_cards_for_choose_[i].y));
			break;
		}
	}

}

bool CDrawer::step_game_objects()
{
	return game_field_.step_static_oblects();
}

void CDrawer::draw_cards_for_choose(CDC* pDC, CRect& client)
{
	CDC		dcMem;
	dcMem.CreateCompatibleDC(pDC);

	dcMem.SelectObject(&bm_choose_color_white_);
	for (int i = 0; i < 5; i++)
	{
		pDC->StretchBlt(color_card_chooser_.color_cards_for_choose_[i].x, color_card_chooser_.color_cards_for_choose_[i].y, 
			color_card_chooser_.color_cards_for_choose_[i].width, 96, &dcMem, 0, 0, 96, 96, SRCCOPY);
	}
}

void CDrawer::draw_choosen_card(CDC* pDC, CRect& client)
{
	CDC		dcMem;
	int		i;

	for (i = 0; i < 5; i++)
		if (color_card_chooser_.color_cards_for_choose_[i].choosen_)
			break;

	if (i > 4)
		return;

	dcMem.CreateCompatibleDC(pDC);

	switch (my_turtle_color_)
	{
	case messages::colors_t::blue:		dcMem.SelectObject(&bm_choose_color_blue_);		break;
	case messages::colors_t::green:		dcMem.SelectObject(&bm_choose_color_green_);	break;
	case messages::colors_t::magenta:	dcMem.SelectObject(&bm_choose_color_magenta_);	break;
	case messages::colors_t::red:		dcMem.SelectObject(&bm_choose_color_red_);		break;
	case messages::colors_t::yellow:	dcMem.SelectObject(&bm_choose_color_yellow_);	break;
	default:
		return;		
	}

	pDC->StretchBlt(color_card_chooser_.color_cards_for_choose_[i].x, color_card_chooser_.color_cards_for_choose_[i].y, 
		color_card_chooser_.color_cards_for_choose_[i].width, 96, &dcMem, 0, 0, 96, 96, SRCCOPY);
}

void CDrawer::write_game_info(CDC* pDC, const char* text)
{
	pDC->SelectObject(&m_GameTextFont);
	pDC->SetTextAlign(TA_CENTER);
	pDC->SetTextColor(RGB(50, 153, 0));
	pDC->SetBkMode(TRANSPARENT);

//	1442 * 710
	pDC->TextOut(game_field_.card_field_rect_.CenterPoint().x, game_field_.card_field_rect_.bottom - m_iGameTextHeight * 1.5, text);
}

void CDrawer::write_gamers_list(CDC* pDC, CRect& client)
{
	std::vector<std::string>::const_iterator it;
	int i = 0;
	int top_of_list = game_field_.main_field_rect_.bottom + (game_field_.card_field_rect_.Height() / 12);

	pDC->SelectObject(&m_InfoTextFont);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(50, 153, 0));

	pDC->SetTextAlign(TA_LEFT);
	pDC->TextOut(game_field_.card_field_rect_.left + 20,
		game_field_.main_field_rect_.bottom + game_field_.card_field_rect_.Height() / 2 - m_iInfoTextHeight / 2, 
		"В забеге участвуют :");

	pDC->SetTextAlign(TA_CENTER);
	for (it = gamers_list_.begin(); it != gamers_list_.end(); it++)
	{
		pDC->TextOut(game_field_.card_field_rect_.CenterPoint().x, int(top_of_list + (i) * (m_iInfoTextHeight)), (*it).c_str());
		i++;
	}
}

void CDrawer::draw_empty_field(CDC* pDC)
{
	// Main field
	pDC->SelectObject(&m_GreenBrush);
	pDC->SelectObject(&m_BorderPen);
	pDC->Rectangle(game_field_.main_field_rect_);

	pDC->SelectObject(&m_LightGrayBrush);
	// Info field (with user's card)
	pDC->Rectangle(game_field_.info_field_rect_);
	// cfards field (for choosing cards and game comments)
	pDC->Rectangle(game_field_.card_field_rect_);
}

void CDrawer::draw_title_turtles(CDC* pDC, bool away)
{
	int i, Y1, Y2;
	CDC		dcMem;
	dcMem.CreateCompatibleDC(pDC);

	dcMem.SelectObject(&bm_running_turtle_and_);	// Выбрали AND-маску
	for (i = 0; i < 5; i++)
	{
		Y1 = away ? title_runnung_turtles_.title_turtles_[i].__y_up : title_runnung_turtles_.title_turtles_[i].__y;

		if (Y1 + 62 < 0)
			return;

		if ((title_runnung_turtles_.title_turtles_[i].__x + 110) > 0 && title_runnung_turtles_.title_turtles_[i].__x < game_field_.main_field_rect_.right)
			pDC->BitBlt(title_runnung_turtles_.title_turtles_[i].__x, Y1, 110, 62, &dcMem, 0, 0, SRCAND);			// Вывели

		Y2 = away ? title_runnung_turtles_.title_turtles_[i]._y_down : title_runnung_turtles_.title_turtles_[i]._y;
		if ((title_runnung_turtles_.title_turtles_[i]._x + 110) > 0 && title_runnung_turtles_.title_turtles_[i]._x < game_field_.main_field_rect_.right)
			pDC->BitBlt(title_runnung_turtles_.title_turtles_[i]._x, Y2, 110, 62, &dcMem, 0, 0, SRCAND);	// Вывели
	}

	for (i = 0; i < 5; i++)
	{
		switch (i)	// Выбрали OR-маску
		{
		case 0:	dcMem.SelectObject(&bm_running_turtle_blue_);	break;
		case 1:	dcMem.SelectObject(&bm_running_turtle_green_);	break;
		case 2:	dcMem.SelectObject(&bm_running_turtle_magenta_);break;
		case 3:	dcMem.SelectObject(&bm_running_turtle_red_);	break;
		case 4:	dcMem.SelectObject(&bm_running_turtle_yellow_);	break;
		}

		if ((title_runnung_turtles_.title_turtles_[i].__x + 110) > 0 && title_runnung_turtles_.title_turtles_[i].__x < game_field_.main_field_rect_.right)
			pDC->BitBlt(title_runnung_turtles_.title_turtles_[i].__x, Y1, 110, 62, &dcMem, 0, 0, SRCINVERT);		// Вывели
		if ((title_runnung_turtles_.title_turtles_[i]._x + 110) > 0 && title_runnung_turtles_.title_turtles_[i]._x < game_field_.main_field_rect_.right)
			pDC->BitBlt(title_runnung_turtles_.title_turtles_[i]._x, Y2, 110, 62, &dcMem, 0, 0, SRCINVERT);	// Вывели
	}
}

void CDrawer::draw_start_button(CDC* pDC, CRect& client)
{
	if (gamers_list_.size() < 2)
		return;

	pDC->SelectObject(&m_DarkGrayBrush);
	pDC->SelectObject(&m_ButtonPen);
	//pDC->Rectangle(game_field_.start_button_ext_rectangle_);
	//pDC->SelectObject(&m_LightGrayBrush);
	pDC->Rectangle(game_field_.start_button_int_rectangle_);

	pDC->SelectObject(&m_GameTextFont);
	pDC->SetTextColor(RGB(50, 153, 0));
	pDC->SetTextAlign(TA_CENTER);
	pDC->SetBkMode(TRANSPARENT);

	pDC->TextOut(int(game_field_.start_button_int_rectangle_.left + game_field_.start_button_int_rectangle_.Width() /2), 
				int(game_field_.start_button_int_rectangle_.top + game_field_.start_button_int_rectangle_.Height() / 2 - m_iGameTextHeight / 2), 
				"Старт");
}

void CDrawer::draw_end_button(CDC* pDC)
{
	pDC->SelectObject(&m_DarkGrayBrush);
	pDC->SelectObject(&m_ButtonPen);
	//pDC->Rectangle(game_field_.start_button_ext_rectangle_);
	//pDC->SelectObject(&m_LightGrayBrush);
	pDC->Rectangle(game_field_.start_button_int_rectangle_);

	pDC->SelectObject(&m_GameTextFont);
	pDC->SetTextColor(RGB(50, 153, 0));
	pDC->SetTextAlign(TA_CENTER);
	pDC->SetBkMode(TRANSPARENT);

	pDC->TextOut(int(game_field_.start_button_int_rectangle_.left + game_field_.start_button_int_rectangle_.Width() / 2),
		int(game_field_.start_button_int_rectangle_.top + game_field_.start_button_int_rectangle_.Height() / 2 - m_iGameTextHeight / 2),
		"Завершить");

	if (!winner_name_.empty())
	{
		std::string		victory = "Победитель : " + winner_name_;
		write_game_info(pDC, victory.c_str());
	}
}

void CDrawer::draw_skip_button(CDC* pDC)
{
	pDC->SelectObject(&m_DarkGrayBrush);
	//pDC->Rectangle(game_field_.skip_button_ext_rectangle_);
	//pDC->SelectObject(&m_LightGrayBrush);
	pDC->Rectangle(game_field_.skip_button_int_rectangle_);

	pDC->SelectObject(&m_GameTextFont);
	pDC->SetTextColor(RGB(50, 153, 0));
	pDC->SetTextAlign(TA_CENTER);
	pDC->SetBkMode(TRANSPARENT);

	pDC->TextOut(int(game_field_.skip_button_int_rectangle_.left + game_field_.skip_button_int_rectangle_.Width() / 2),
		int(game_field_.skip_button_int_rectangle_.top + game_field_.skip_button_int_rectangle_.Height() / 2 - m_iGameTextHeight / 2),
		"Дальше");
}

bool CDrawer::is_start_pressed(CPoint point)
{
	if (point.x > game_field_.start_button_ext_rectangle_.left && point.x < game_field_.start_button_ext_rectangle_.right &&
		point.y > game_field_.start_button_ext_rectangle_.top && point.y < game_field_.start_button_ext_rectangle_.bottom)
	{
		winner_name_ = "";
		return true;
	}
	else
		return false;
}

bool CDrawer::is_skip_pressed(CPoint point)
{
	if (point.x > game_field_.skip_button_ext_rectangle_.left && point.x < game_field_.skip_button_ext_rectangle_.right &&
		point.y > game_field_.skip_button_ext_rectangle_.top && point.y < game_field_.skip_button_ext_rectangle_.bottom)
		return true;
	else
		return false;
}

void CDrawer::init_playing_cards(const std::vector<messages::cards_t>& cards)
{
	int i = 0;
	int h_center_line = game_field_.card_field_rect_.left + ((game_field_.card_field_rect_.right - game_field_.card_field_rect_.left) / 2);
	int v_center_line = game_field_.card_field_rect_.top + ((game_field_.card_field_rect_.bottom - game_field_.card_field_rect_.top) / 2) - 30;

	std::vector<messages::cards_t>::const_iterator	it_card;
	for (it_card = cards.begin(); it_card != cards.end(); it_card++)
	{
		playing_cards_[i].init_on_deal(*it_card, h_center_line + (i - 2) * 150, v_center_line);
		i++;
	}
}

void CDrawer::remove_card(messages::cards_t card)
{
	std::vector<messages::cards_t>::const_iterator	it_card;
	for (int i = 0; i < 5; i++)
	{
		if (playing_cards_[i].card_type() == card)
		{
			playing_cards_[i].remove();
			break;
		}
	}
}

void CDrawer::new_card(messages::cards_t card)
{
	std::vector<messages::cards_t>::const_iterator	it_card;
	for (int i = 0; i < 5; i++)
	{
		if (playing_cards_[i].card_type() == messages::cards_t::c_0_unknown)
		{
			playing_cards_[i].set(card);
			break;
		}
	}
}

void CDrawer::draw_playing_cards(CDC* pDC, CRect& client)
{
	int i = 0;
	for (i = 0; i < 5; i++)
	{
		if (playing_cards_[i].card_type() != messages::cards_t::c_0_unknown)
		{
			if (playing_cards_[i].card_place().Height())
			{
				CDC		dcMem;
				dcMem.CreateCompatibleDC(pDC);

				switch (playing_cards_[i].card_type())
				{
				case messages::cards_t::c_1_blue_plusplus:		dcMem.SelectObject(&bm_blue_plusplus_);		break;
				case messages::cards_t::c_1_green_plusplus:		dcMem.SelectObject(&bm_green_plusplus_);	break;
				case messages::cards_t::c_1_magenta_plusplus:	dcMem.SelectObject(&bm_magenta_plusplus_);	break;
				case messages::cards_t::c_1_red_plusplus:		dcMem.SelectObject(&bm_red_plusplus_);		break;
				case messages::cards_t::c_1_yellow_plusplus:	dcMem.SelectObject(&bm_yellow_plusplus_);	break;
				case messages::cards_t::c_2_blue_plus:			dcMem.SelectObject(&bm_blue_plus_);			break;
				case messages::cards_t::c_2_green_plus:			dcMem.SelectObject(&bm_green_plus_);		break;
				case messages::cards_t::c_2_magenta_plus:		dcMem.SelectObject(&bm_magenta_plus_);		break;
				case messages::cards_t::c_2_red_plus:			dcMem.SelectObject(&bm_red_plus_);			break;
				case messages::cards_t::c_2_yellow_plus:		dcMem.SelectObject(&bm_yellow_plus_);		break;
				case messages::cards_t::c_3_blue_minus:			dcMem.SelectObject(&bm_blue_minus_);		break;
				case messages::cards_t::c_3_green_minus:		dcMem.SelectObject(&bm_green_minus_);		break;
				case messages::cards_t::c_3_magenta_minus:		dcMem.SelectObject(&bm_magenta_minus_);		break;
				case messages::cards_t::c_3_red_minus:			dcMem.SelectObject(&bm_red_minus_);			break;
				case messages::cards_t::c_3_yellow_minus:		dcMem.SelectObject(&bm_yellow_minus_);		break;
				case messages::cards_t::c_4_nocolor_up:			dcMem.SelectObject(&bm_nocolor_up_);		break;
				case messages::cards_t::c_5_nocolor_upup:		dcMem.SelectObject(&bm_nocolor_upup_);		break;
				case messages::cards_t::c_6_nocolor_plus:		dcMem.SelectObject(&bm_nocolor_plus_);		break;
				case messages::cards_t::c_7_nocolor_minus:		dcMem.SelectObject(&bm_nocolor_minus_);		break;
				}

				pDC->StretchBlt(playing_cards_[i].card_place().left, playing_cards_[i].card_place().top, playing_cards_[i].card_place().Width(), playing_cards_[i].card_place().Height(), &dcMem, 0, 0, 96, 144, SRCCOPY);
			}
		}
	}
}

bool CDrawer::increase_playing_cards()
{
	int i = 0;
	bool increased = false;
	for (i = 0; i < 5; i++)
	{
		if (playing_cards_[i].card_type() != messages::cards_t::c_0_unknown)
			if (playing_cards_[i].increase())
				increased = true;
	}

	if (spliner_.remove_point())
		increased = true;
	else
	{
		game_field_.race_track_ = temporary_saved_track_;
		jumping_turtles_.clear();
	}

	return increased;
}

void CDrawer::draw_game_ending(CDC* pDC)
{
	CDC		dcMem;
	int		i;

	dcMem.CreateCompatibleDC(pDC);

	// My turtle
	switch (my_turtle_color_)	// Выбрали OR-маску
	{
	case messages::colors_t::blue:		dcMem.SelectObject(&bm_choose_color_blue_);		break;
	case messages::colors_t::green:		dcMem.SelectObject(&bm_choose_color_green_);	break;
	case messages::colors_t::magenta:	dcMem.SelectObject(&bm_choose_color_magenta_);	break;
	case messages::colors_t::red:		dcMem.SelectObject(&bm_choose_color_red_);		break;
	case messages::colors_t::yellow:	dcMem.SelectObject(&bm_choose_color_yellow_);	break;
	}
	pDC->StretchBlt(game_field_.my_card_pos_.x, game_field_.my_card_pos_.y, 96, 96, &dcMem, 0, 0, 96, 96, SRCCOPY);

	pDC->SelectObject(&m_InfoTextFont);
	pDC->SetTextAlign(TA_CENTER);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(50, 153, 0));
	pDC->TextOut(game_field_.my_card_end_pos_.x + 48, game_field_.my_card_end_pos_.y + 140, gamer_name_.c_str());

	// Капуста:
	dcMem.SelectObject(&bm_cabbage_and_);	// Выбрали AND-маску
	for (i = 0; i < 5; i++)
		pDC->BitBlt(game_field_.cabbage_left_side_, game_field_.main_field_rect_.top + 2 + (i * 90), 100, 86, &dcMem, 0, 0, SRCAND);	// Вывели

	dcMem.SelectObject(&bm_cabbage_or_);	// Выбрали OR-маску
	for (i = 0; i < 5; i++)
		pDC->BitBlt(game_field_.cabbage_left_side_, game_field_.main_field_rect_.top + 2 + (i * 90), 100, 86, &dcMem, 0, 0, SRCINVERT);	// Вывели


	/// Turtles run to their start places
	for (i = 0; i < 5; i++)
	{
		dcMem.SelectObject(&bm_running_turtle_and_);							// Загрузили OR-маску
		pDC->BitBlt(ending_game_.turtles_[i].current_position_.x, ending_game_.turtles_[i].current_position_.y, 110, 62, &dcMem, 0, 0, SRCAND);	// Вывели

		switch (ending_game_.turtles_[i].color_)	// Выбрали OR-маску
		{
		case messages::colors_t::blue:		dcMem.SelectObject(&bm_running_turtle_blue_);	break;
		case messages::colors_t::green:		dcMem.SelectObject(&bm_running_turtle_green_);	break;
		case messages::colors_t::magenta:	dcMem.SelectObject(&bm_running_turtle_magenta_); break;
		case messages::colors_t::red:		dcMem.SelectObject(&bm_running_turtle_red_);	break;
		case messages::colors_t::yellow:	dcMem.SelectObject(&bm_running_turtle_yellow_);	break;
		}

		pDC->BitBlt(ending_game_.turtles_[i].current_position_.x, ending_game_.turtles_[i].current_position_.y, 110, 62, &dcMem, 0, 0, SRCINVERT);	// Вывели
		std::string turtle_name = ending_game_.turtles_[i].name();
		if (!turtle_name.empty())
		{
			pDC->SetTextAlign(TA_LEFT);
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(200, 230, 160));
			pDC->TextOut(ending_game_.turtles_[i].current_position_.x + 115, ending_game_.turtles_[i].current_position_.y + m_iInfoTextHeight, turtle_name.c_str());
		}
	}

	// Playing cards
	for (i = 0; i < 5; i++)
	{
		if (ending_game_.playing_cards_[i].card_type() != messages::cards_t::c_0_unknown)
		{
			if (ending_game_.playing_cards_[i].card_place().Height())
			{
				switch (ending_game_.playing_cards_[i].card_type())
				{
				case messages::cards_t::c_1_blue_plusplus:		dcMem.SelectObject(&bm_blue_plusplus_);		break;
				case messages::cards_t::c_1_green_plusplus:		dcMem.SelectObject(&bm_green_plusplus_);	break;
				case messages::cards_t::c_1_magenta_plusplus:	dcMem.SelectObject(&bm_magenta_plusplus_);	break;
				case messages::cards_t::c_1_red_plusplus:		dcMem.SelectObject(&bm_red_plusplus_);		break;
				case messages::cards_t::c_1_yellow_plusplus:	dcMem.SelectObject(&bm_yellow_plusplus_);	break;
				case messages::cards_t::c_2_blue_plus:			dcMem.SelectObject(&bm_blue_plus_);			break;
				case messages::cards_t::c_2_green_plus:			dcMem.SelectObject(&bm_green_plus_);		break;
				case messages::cards_t::c_2_magenta_plus:		dcMem.SelectObject(&bm_magenta_plus_);		break;
				case messages::cards_t::c_2_red_plus:			dcMem.SelectObject(&bm_red_plus_);			break;
				case messages::cards_t::c_2_yellow_plus:		dcMem.SelectObject(&bm_yellow_plus_);		break;
				case messages::cards_t::c_3_blue_minus:			dcMem.SelectObject(&bm_blue_minus_);		break;
				case messages::cards_t::c_3_green_minus:		dcMem.SelectObject(&bm_green_minus_);		break;
				case messages::cards_t::c_3_magenta_minus:		dcMem.SelectObject(&bm_magenta_minus_);		break;
				case messages::cards_t::c_3_red_minus:			dcMem.SelectObject(&bm_red_minus_);			break;
				case messages::cards_t::c_3_yellow_minus:		dcMem.SelectObject(&bm_yellow_minus_);		break;
				case messages::cards_t::c_4_nocolor_up:			dcMem.SelectObject(&bm_nocolor_up_);		break;
				case messages::cards_t::c_5_nocolor_upup:		dcMem.SelectObject(&bm_nocolor_upup_);		break;
				case messages::cards_t::c_6_nocolor_plus:		dcMem.SelectObject(&bm_nocolor_plus_);		break;
				case messages::cards_t::c_7_nocolor_minus:		dcMem.SelectObject(&bm_nocolor_minus_);		break;
				}

				pDC->StretchBlt(ending_game_.playing_cards_[i].card_place().left, ending_game_.playing_cards_[i].card_place().top, 
					ending_game_.playing_cards_[i].card_place().Width(), ending_game_.playing_cards_[i].card_place().Height(), &dcMem, 0, 0, 96, 144, SRCCOPY);
			}
		}
	}

	// Stones
	if (ending_game_.stones_[i].Width() || ending_game_.stones_[i].Height())
	{
		dcMem.SelectObject(&bm_stone_and_);
		for (i = 0; i < 10; i++)
			pDC->StretchBlt(ending_game_.stones_[i].left, ending_game_.stones_[i].top, ending_game_.stones_[i].Width(), ending_game_.stones_[i].Height(),
				&dcMem, 0, 0, 120, 60, SRCAND);

		dcMem.SelectObject(&bm_stone_or_);
		for (i = 0; i < 10; i++)
			pDC->StretchBlt(ending_game_.stones_[i].left, ending_game_.stones_[i].top, ending_game_.stones_[i].Width(), ending_game_.stones_[i].Height(),
				&dcMem, 0, 0, 120, 60, SRCINVERT);
	}

	// Winner Turtle
	if (ending_game_.winner_turtle_.Width())
	{
		switch (winner_color_)
		{
		case messages::colors_t::blue:		dcMem.SelectObject(&bm_choose_color_blue_);		break;
		case messages::colors_t::green:		dcMem.SelectObject(&bm_choose_color_green_);	break;
		case messages::colors_t::magenta:	dcMem.SelectObject(&bm_choose_color_magenta_);	break;
		case messages::colors_t::red:		dcMem.SelectObject(&bm_choose_color_red_);		break;
		case messages::colors_t::yellow:	dcMem.SelectObject(&bm_choose_color_yellow_);	break;
		}

		pDC->StretchBlt(ending_game_.winner_turtle_.left, ending_game_.winner_turtle_.top,
			ending_game_.winner_turtle_.Width(), ending_game_.winner_turtle_.Height(), &dcMem, 0, 0, 96, 96, SRCCOPY);
	}

	// Cabbage - final
	for (i = 0; i < 8; i++)
	{
		if (ending_game_.cabbages_[i].position_.x == 830 && ending_game_.cabbages_[i].position_.y < -85)
			break;

		dcMem.SelectObject(&bm_cabbage_and_);	// Выбрали AND-маску
		pDC->BitBlt(ending_game_.cabbages_[i].position_.x, ending_game_.cabbages_[i].position_.y, 100, 86, &dcMem, 0, 0, SRCAND);	// Вывели

		dcMem.SelectObject(&bm_cabbage_or_);	// Выбрали OR-маску
		pDC->BitBlt(ending_game_.cabbages_[i].position_.x, ending_game_.cabbages_[i].position_.y, 100, 86, &dcMem, 0, 0, SRCINVERT);	// Вывели
	}

	// КАПУСТА - ДЛЯ ПРИМЕРКИ
	//if (ending_game_.winner_turtle_.Width() == 96)
	//{
	//	dcMem.SelectObject(&bm_cabbage_and_);	// Выбрали AND-маску
	//	pDC->BitBlt(670, 459 - 432, 100, 86, &dcMem, 0, 0, SRCAND);	// Вывели
	//	pDC->BitBlt(783, 459 - 385, 100, 86, &dcMem, 0, 0, SRCAND);	// Вывели
	//	pDC->BitBlt(830, 459 - 272, 100, 86, &dcMem, 0, 0, SRCAND);	// Вывели
	//	pDC->BitBlt(783, 459 - 159, 100, 86, &dcMem, 0, 0, SRCAND);	// Вывели
	//	pDC->BitBlt(670, 459 - 112, 100, 86, &dcMem, 0, 0, SRCAND);	// Вывели
	//	pDC->BitBlt(557, 459 - 159, 100, 86, &dcMem, 0, 0, SRCAND);	// Вывели
	//	pDC->BitBlt(510, 459 - 272, 100, 86, &dcMem, 0, 0, SRCAND);	// Вывели
	//	pDC->BitBlt(557, 459 - 385, 100, 86, &dcMem, 0, 0, SRCAND);	// Вывели

	//	dcMem.SelectObject(&bm_cabbage_or_);	// Выбрали OR-маску
	//	pDC->BitBlt(670, 459 - 432, 100, 86, &dcMem, 0, 0, SRCINVERT);	// Вывели
	//	pDC->BitBlt(783, 459 - 385, 100, 86, &dcMem, 0, 0, SRCINVERT);	// Вывели
	//	pDC->BitBlt(830, 459 - 272, 100, 86, &dcMem, 0, 0, SRCINVERT);	// Вывели
	//	pDC->BitBlt(783, 459 - 159, 100, 86, &dcMem, 0, 0, SRCINVERT);	// Вывели
	//	pDC->BitBlt(670, 459 - 112, 100, 86, &dcMem, 0, 0, SRCINVERT);	// Вывели
	//	pDC->BitBlt(557, 459 - 159, 100, 86, &dcMem, 0, 0, SRCINVERT);	// Вывели
	//	pDC->BitBlt(510, 459 - 272, 100, 86, &dcMem, 0, 0, SRCINVERT);	// Вывели
	//	pDC->BitBlt(557, 459 - 385, 100, 86, &dcMem, 0, 0, SRCINVERT);	// Вывели
	//}
}

void CDrawer::draw_game_objects(CDC* pDC)
{
	CDC		dcMem;
	int		i;

	dcMem.CreateCompatibleDC(pDC);

	// Капуста:
	dcMem.SelectObject(&bm_cabbage_and_);	// Выбрали AND-маску
	for (i = 0; i < 5; i++)
		pDC->BitBlt(game_field_.cabbage_left_side_, game_field_.main_field_rect_.top + 2 + (i * 90), 100, 86, &dcMem, 0, 0, SRCAND);	// Вывели

	dcMem.SelectObject(&bm_cabbage_or_);	// Выбрали OR-маску
	for (i = 0; i < 5; i++)
		pDC->BitBlt(game_field_.cabbage_left_side_, game_field_.main_field_rect_.top + 2 + (i * 90), 100, 86, &dcMem, 0, 0, SRCINVERT);	// Вывели

	// Камни
	if (game_field_.rocks_height_)
	{
		pDC->SelectObject(&m_WhiteBrush);

		dcMem.SelectObject(&bm_stone_and_);
		for (i = 0; i < 10; i++)
			pDC->StretchBlt(game_field_.main_field_rect_.left + 130 + 120 * i, game_field_.rocks_bottom_ - game_field_.rocks_height_, 120, game_field_.rocks_height_, &dcMem, 0, 0, 120, 60, SRCAND);

		dcMem.SelectObject(&bm_stone_or_);
		for (i = 0; i < 10; i++)
			pDC->StretchBlt(game_field_.main_field_rect_.left + 130 + 120 * i, game_field_.rocks_bottom_ - game_field_.rocks_height_, 120, game_field_.rocks_height_, &dcMem, 0, 0, 120, 60, SRCINVERT);
	}

	switch (my_turtle_color_)	// Выбрали OR-маску
	{
	case messages::colors_t::blue:		dcMem.SelectObject(&bm_choose_color_blue_);		break;
	case messages::colors_t::green:		dcMem.SelectObject(&bm_choose_color_green_);	break;
	case messages::colors_t::magenta:	dcMem.SelectObject(&bm_choose_color_magenta_);	break;
	case messages::colors_t::red:		dcMem.SelectObject(&bm_choose_color_red_);		break;
	case messages::colors_t::yellow:	dcMem.SelectObject(&bm_choose_color_yellow_);	break;
	}

	pDC->StretchBlt(game_field_.my_card_pos_.x, game_field_.my_card_pos_.y, 96, 96, &dcMem, 0, 0, 96, 96, SRCCOPY);

	if (game_field_.my_card_end_pos_.x == game_field_.my_card_pos_.x)
	{
		pDC->SelectObject(&m_InfoTextFont);
		pDC->SetTextAlign(TA_CENTER);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(50, 153, 0));
		pDC->TextOut(game_field_.my_card_end_pos_.x + 48, game_field_.my_card_end_pos_.y + 140, gamer_name_.c_str());
	}

	dcMem.SelectObject(&bm_running_turtle_and_);							// Загрузили OR-маску
	for (i = 0; i < 5; i++)
	{
		if (game_field_.race_track_.start_position_[i] != messages::colors_t::unknown)
			pDC->BitBlt(game_field_.turtles_position_, game_field_.main_field_rect_.top + 34 + i * 80, 110, 62, &dcMem, 0, 0, SRCAND);	// Вывели
	}

	for (i = 0; i < 5; i++)
	{
		switch (game_field_.race_track_.start_position_[i])	// Выбрали OR-маску
		{
		case messages::colors_t::blue:		dcMem.SelectObject(&bm_running_turtle_blue_);	break;
		case messages::colors_t::green:		dcMem.SelectObject(&bm_running_turtle_green_);	break;
		case messages::colors_t::magenta:	dcMem.SelectObject(&bm_running_turtle_magenta_); break;
		case messages::colors_t::red:		dcMem.SelectObject(&bm_running_turtle_red_);	break;
		case messages::colors_t::yellow:	dcMem.SelectObject(&bm_running_turtle_yellow_);	break;
		}

		if (game_field_.race_track_.start_position_[i] != messages::colors_t::unknown)
			pDC->BitBlt(game_field_.turtles_position_, game_field_.main_field_rect_.top + 34 + i * 80, 110, 62, &dcMem, 0, 0, SRCINVERT);	// Вывели
	}

	for (i = 0; i < 11; i++)
	{
		std::vector<messages::colors_t>::iterator it_turtle;
		int j = 0;
		for (it_turtle = game_field_.race_track_.rocks_[i].begin(); it_turtle != game_field_.race_track_.rocks_[i].end(); it_turtle++)
		{
			dcMem.SelectObject(&bm_running_turtle_and_);	// Выбрали OR-маску
			pDC->BitBlt(game_field_.main_field_rect_.left + 140 + 120 * i, game_field_.main_field_rect_.top + 55 + 5 * 62 - j * 49, 110, 62, &dcMem, 0, 0, SRCAND);	// Вывели

			switch (*it_turtle)	// Выбрали OR-маску
			{
			case messages::colors_t::blue:		dcMem.SelectObject(&bm_running_turtle_blue_);	break;
			case messages::colors_t::green:		dcMem.SelectObject(&bm_running_turtle_green_);	break;
			case messages::colors_t::magenta:	dcMem.SelectObject(&bm_running_turtle_magenta_); break;
			case messages::colors_t::red:		dcMem.SelectObject(&bm_running_turtle_red_);	break;
			case messages::colors_t::yellow:	dcMem.SelectObject(&bm_running_turtle_yellow_);	break;
			}
			pDC->BitBlt(game_field_.main_field_rect_.left + 140 + 120 * i, game_field_.main_field_rect_.top + 55 + 5 * 62 - j * 49, 110, 62, &dcMem, 0, 0, SRCINVERT);	// Вывели

			j++;
		}
	}

	{
//		spliner_.draw(pDC);

		CPoint jmp_point;
		if (spliner_.get_step(jmp_point))
		{
			std::vector<messages::colors_t>::iterator	it_jt;
			int i_jt = 0;
			for (it_jt = jumping_turtles_.begin(); it_jt != jumping_turtles_.end(); it_jt++)
			{
				dcMem.SelectObject(&bm_running_turtle_and_);	// Выбрали OR-маску
				pDC->BitBlt(jmp_point.x, jmp_point.y - i_jt * 49, 110, 62, &dcMem, 0, 0, SRCAND);	// Вывели

				switch (*it_jt)	// Выбрали OR-маску
				{
				case messages::colors_t::blue:		dcMem.SelectObject(&bm_running_turtle_blue_);	break;
				case messages::colors_t::green:		dcMem.SelectObject(&bm_running_turtle_green_);	break;
				case messages::colors_t::magenta:	dcMem.SelectObject(&bm_running_turtle_magenta_); break;
				case messages::colors_t::red:		dcMem.SelectObject(&bm_running_turtle_red_);	break;
				case messages::colors_t::yellow:	dcMem.SelectObject(&bm_running_turtle_yellow_);	break;
				}
				pDC->BitBlt(jmp_point.x, jmp_point.y - i_jt * 49, 110, 62, &dcMem, 0, 0, SRCINVERT);	// Вывели

				i_jt++;
			}
		}
	}

	std::vector<color_card_for_clarification_t>::iterator it_color_variant;
	for (it_color_variant = color_clarification_.color_variants_.begin(); it_color_variant != color_clarification_.color_variants_.end(); it_color_variant++)
	{
		switch (it_color_variant->color_)
		{
		case messages::colors_t::blue:		dcMem.SelectObject(&bm_choose_color_blue_);		break;
		case messages::colors_t::green:		dcMem.SelectObject(&bm_choose_color_green_);	break;
		case messages::colors_t::magenta:	dcMem.SelectObject(&bm_choose_color_magenta_);	break;
		case messages::colors_t::red:		dcMem.SelectObject(&bm_choose_color_red_);		break;
		case messages::colors_t::yellow:	dcMem.SelectObject(&bm_choose_color_yellow_);	break;
		case messages::colors_t::unknown:	dcMem.SelectObject(&bm_choose_color_cancel_);	break;
		}

		pDC->StretchBlt(it_color_variant->place_.left, it_color_variant->place_.bottom - color_clarification_.cards_size_, 96, color_clarification_.cards_size_, &dcMem, 0, 0, 96, 96, SRCCOPY);
	}
}

void CDrawer::init_game_ending(messages::colors_t winner_color, const std::map<messages::colors_t, std::string>& gamers_colors)
{
	int i = 0 , j = 0 , n_turtle = 0;

	winner_color_ = winner_color;

	{
		std::map<messages::colors_t, std::string>::const_iterator it = gamers_colors.find(winner_color);
		if (it != gamers_colors.end())
			winner_name_ = it->second;
	}

	// Running Turtles 
	//for (i = 0; i < 5; i++)
	//{
	//	if (game_field_.race_track_.start_position_[i] != messages::colors_t::unknown)
	//	{
	//		game_field_.race_track_.rocks_[rand() % 10].push_back(game_field_.race_track_.start_position_[i]);
	//		game_field_.race_track_.start_position_[i] = messages::colors_t::unknown;
	//	}
	//}

	// Step 1 - collecting turtles and finding the current positions
	for (i = 0; i < 5; i++)
	{
		if (game_field_.race_track_.start_position_[i] != messages::colors_t::unknown)
		{
			ending_game_.turtles_[n_turtle].current_position_.SetPoint(5, game_field_.main_field_rect_.top + 34 + i * 80);
			ending_game_.turtles_[n_turtle].color_ = game_field_.race_track_.start_position_[i];

			n_turtle++;
		}
	}

	for (i = 0; i < 11; i++)
	{
		j = 0;
		std::vector<messages::colors_t>::iterator it_turtle;
		for (it_turtle = game_field_.race_track_.rocks_[i].begin(); it_turtle != game_field_.race_track_.rocks_[i].end(); it_turtle++)
		{
			ending_game_.turtles_[n_turtle].current_position_.SetPoint(game_field_.main_field_rect_.left + 140 + 120 * i,
															game_field_.main_field_rect_.top + 55 + 5 * 62 - j * 49);
			ending_game_.turtles_[n_turtle].color_ = *it_turtle;

			n_turtle++;
			j++;
		}
	}

	// Step 2 - determination of the end positions and steps
	for (i = 0; i < 5; i++)
	{
		ending_game_.turtles_[i].name_ = "";

		std::map<messages::colors_t, std::string>::const_iterator it_gamecolor = gamers_colors.find(ending_game_.turtles_[i].color_);
		if (it_gamecolor != gamers_colors.end())
			ending_game_.turtles_[i].name_ = it_gamecolor->second;

		ending_game_.turtles_[i].destination_.SetPoint(5, game_field_.main_field_rect_.top + 34 + (int(ending_game_.turtles_[i].color_) - 1) * 80);
		ending_game_.turtles_[i].step_.x = 6;

		if (ending_game_.turtles_[i].current_position_.y > ending_game_.turtles_[i].destination_.y)
			ending_game_.turtles_[i].step_.y = -3;
		else
			ending_game_.turtles_[i].step_.y = +3;
	}

	// Playing cards
	for (i = 0; i < 5; i++)
	{
		if (playing_cards_[i].card_type() != messages::cards_t::c_0_unknown)
			ending_game_.playing_cards_[i] = playing_cards_[i];
	}

	// Stones
	for (i = 0; i < 10; i++)
	{
		ending_game_.stones_[i] = CRect(game_field_.main_field_rect_.left + 130 + 120 * i, game_field_.rocks_bottom_ - game_field_.rocks_height_,
			game_field_.main_field_rect_.left + 250 + 120 * i, game_field_.rocks_bottom_);
	}

	// Winner Turtle
	ending_game_.winner_turtle_.SetRect(game_field_.main_field_rect_.CenterPoint().x, game_field_.main_field_rect_.CenterPoint().y,
		game_field_.main_field_rect_.CenterPoint().x, game_field_.main_field_rect_.CenterPoint().y);

	// Running Cabbages
	for (i = 0; i < 8; i++)
	{
		ending_game_.cabbages_[i].position_.SetPoint(game_field_.main_field_rect_.CenterPoint().x + 110, -86);
		ending_game_.cabbages_[i].angle_ = 0;
	}

	game_field_.race_track_.init();

	ending_game_.inited_ = true;
}

bool CDrawer::step_game_ending()
{
	int i;
	bool made_step = false;

	// Turtles on start position
	for (i = 0; i < 5; i++)
	{
		if (!ending_game_.turtles_[i].step_.x && !ending_game_.turtles_[i].step_.y)
			continue;

		made_step = true;

		ending_game_.turtles_[i].current_position_.x -= ending_game_.turtles_[i].step_.x;

		if (ending_game_.turtles_[i].current_position_.x <= ending_game_.turtles_[i].destination_.x)
		{
			ending_game_.turtles_[i].current_position_.x = ending_game_.turtles_[i].destination_.x;
			ending_game_.turtles_[i].step_.x = 0;
		}

		ending_game_.turtles_[i].current_position_.y += ending_game_.turtles_[i].step_.y;

		if (ending_game_.turtles_[i].step_.y < 0)
		{
			if (ending_game_.turtles_[i].current_position_.y < ending_game_.turtles_[i].destination_.y)
			{
				ending_game_.turtles_[i].current_position_.y = ending_game_.turtles_[i].destination_.y;
				ending_game_.turtles_[i].step_.y = 0;
			}
		}
		else
		{
			if (ending_game_.turtles_[i].current_position_.y > ending_game_.turtles_[i].destination_.y)
			{
				ending_game_.turtles_[i].current_position_.y = ending_game_.turtles_[i].destination_.y;
				ending_game_.turtles_[i].step_.y = 0;
			}
		}

		if (ending_game_.playing_cards_[i].step_disappearing())
			made_step = true;
		else
			ending_game_.playing_cards_[i].set(messages::cards_t::c_0_unknown);
	}

	// Turtles on stones
	for (i = 0; i < 11; i++)
	{
		if (ending_game_.stones_[i].Width() > 0)
		{
			ending_game_.stones_[i].left++;
			ending_game_.stones_[i].right--;
			made_step = true;
		}
		if (ending_game_.stones_[i].Height() > 0)
		{
			ending_game_.stones_[i].top++;
			ending_game_.stones_[i].bottom--;
			made_step = true;
		}
	}

	// Winner Turtle
	if (ending_game_.winner_turtle_.Width() < 96)
	{
		ending_game_.winner_turtle_.left--;
		ending_game_.winner_turtle_.right++;
		ending_game_.winner_turtle_.top--;
		ending_game_.winner_turtle_.bottom++;
		made_step = true;
	}

	int vertical_x = game_field_.main_field_rect_.CenterPoint().x + 110;

	// Running cabbages
	for (i = 0; i < 8; i++)
	{
		if (ending_game_.cabbages_[i].position_.x == vertical_x && ending_game_.cabbages_[i].position_.y <= 186)
		{	// At the vertical line
			made_step = true;
			ending_game_.cabbages_[i].position_.y += 3;

			if (ending_game_.cabbages_[i].position_.y < 55)
				break;
		}
		else
		{	// At the curve line. Radius 160 ; Center 670,186 ;
			if (ending_game_.cabbages_[i].angle_ == 315 - i * 45)
				continue;

			made_step = true;
			ending_game_.cabbages_[i].angle_++;
			double angle = double(ending_game_.cabbages_[i].angle_) / 57.3;
			ending_game_.cabbages_[i].position_.SetPoint(int(game_field_.main_field_rect_.CenterPoint().x - 50 + 160 * cos(angle)), int(186 + 160 * sin(angle)));
		}
	}

	return made_step;
}

void CDrawer::step_cabbage()
{
	double angle;

	ending_game_.cabbages_[0].angle_++;
	if (ending_game_.cabbages_[0].angle_ > 360)
		ending_game_.cabbages_[0].angle_ = 0;

	angle = double(ending_game_.cabbages_[0].angle_) / 57.3;
	ending_game_.cabbages_[0].position_.SetPoint(int(game_field_.main_field_rect_.CenterPoint().x - 50 + 160 * cos(angle)), 
		int(186 + 160 * sin(angle)));

	for (int i = 1; i < 8; i++)
	{
		ending_game_.cabbages_[i].angle_ = ending_game_.cabbages_[0].angle_ - 45 * i;
		if (ending_game_.cabbages_[i].angle_ < 0)
			ending_game_.cabbages_[i].angle_ += 360;

		angle = double(ending_game_.cabbages_[i].angle_) / 57.3;
		ending_game_.cabbages_[i].position_.SetPoint(int(game_field_.main_field_rect_.CenterPoint().x - 50 + 160 * cos(angle)), 
			int(186 + 160 * sin(angle)));
	}

	
}

messages::cards_t	CDrawer::card_under_cursor(CPoint& point)
{
	for (int i = 0; i < 5; i++)
	{
		if (playing_cards_[i].point_in(point))
			return playing_cards_[i].card_type();
	}

	return messages::cards_t::c_0_unknown;
}

int CDrawer::is_color_clarified(CPoint& point)
{
	std::vector<color_card_for_clarification_t>::iterator	it_var;
	for (it_var = color_clarification_.color_variants_.begin(); it_var != color_clarification_.color_variants_.end(); it_var++)
	{
		if (!it_var->is_point_over(point))
			continue;

		messages::colors_t choosen_color = it_var->color_;

		if (choosen_color == messages::colors_t::unknown)
			return 2;

		it_var->choosen_ = true;

		for (int i = 0; i < 5; i++)
		{
			if (playing_cards_[i].choosen())
			{
				playing_cards_[i].set_clarified_color(choosen_color);

				return 1;
			}
		}
	}

	return 0;
}

std::set<messages::colors_t> CDrawer::possible_turtles_to_move(messages::cards_t card)
{
	std::set<messages::colors_t>	possible_colors;

	if (card == messages::cards_t::c_0_unknown)
		return possible_colors;

	messages::colors_t	card_color		= messages::card_color(card);
	int					turtle_position = game_field_.race_track_.turtle_position(card_color);

	switch (card)
	{
	default:
	case messages::cards_t::c_0_unknown:
		break;
	case messages::cards_t::c_1_blue_plusplus:
	case messages::cards_t::c_1_green_plusplus:
	case messages::cards_t::c_1_magenta_plusplus:
	case messages::cards_t::c_1_red_plusplus:
	case messages::cards_t::c_1_yellow_plusplus:
	case messages::cards_t::c_2_blue_plus:
	case messages::cards_t::c_2_green_plus:
	case messages::cards_t::c_2_magenta_plus:
	case messages::cards_t::c_2_red_plus:
	case messages::cards_t::c_2_yellow_plus:
		possible_colors.insert(card_color);
		break;
	case messages::cards_t::c_3_blue_minus:
	case messages::cards_t::c_3_green_minus:
	case messages::cards_t::c_3_magenta_minus:
	case messages::cards_t::c_3_red_minus:
	case messages::cards_t::c_3_yellow_minus:
		if (turtle_position > 0)
		{
			possible_colors.insert(card_color);
		}
		break;
	case messages::cards_t::c_6_nocolor_plus:
		possible_colors.insert(messages::colors_t::blue);
		possible_colors.insert(messages::colors_t::green);
		possible_colors.insert(messages::colors_t::magenta);
		possible_colors.insert(messages::colors_t::red);
		possible_colors.insert(messages::colors_t::yellow);
		break;
	case messages::cards_t::c_7_nocolor_minus:
		{
		for (int i = 0; i < 10; i++)
		{
			std::vector<messages::colors_t>::iterator it;
			for (it = game_field_.race_track_.rocks_[i].begin(); it != game_field_.race_track_.rocks_[i].end(); it++)
				possible_colors.insert(*it);
		}
		}
		break;
	case messages::cards_t::c_4_nocolor_up:
	case messages::cards_t::c_5_nocolor_upup:
		{
			int i;
			for (i = 0; i < 5; i++)
			{
				if (game_field_.race_track_.start_position_[i] != messages::colors_t::unknown)
					possible_colors.insert(game_field_.race_track_.start_position_[i]);
			}

			if (possible_colors.size())
				break;

			for (i = 0; i < 10; i++)
			{
				std::vector<messages::colors_t>::iterator it;
				for (it = game_field_.race_track_.rocks_[i].begin(); it != game_field_.race_track_.rocks_[i].end(); it++)
					possible_colors.insert(*it);

				if (possible_colors.size())
					break;
			}
		}
		break;
	}

	if (card_color == messages::colors_t::unknown && possible_colors.size() == 1)
	{	// Card color unknown but we jave only a card that can move
		for (int i = 0; i < 5; i++)
		{
			if (playing_cards_[i].card_type() == card)
			{
				playing_cards_[i].set_clarified_color(* possible_colors.begin());
				break;
			}
		}
	}

	return possible_colors;
}

void CDrawer::init_card_disappearing(messages::cards_t card)
{
	for (int i = 0; i < 5; i++)
	{
		if (playing_cards_[i].card_type() == card)
		{
			playing_cards_[i].start_disappearing();
			return;
		}
	}
}

bool CDrawer::disappear_choosen_card()
{
	for (int i = 0; i < 5; i++)
	{
		if (playing_cards_[i].choosen())
			return playing_cards_[i].step_disappearing();
	}

	return false;
}

void CDrawer::end_card_disappearing()
{
	for (int i = 0; i < 5; i++)
	{
		if (playing_cards_[i].choosen())
			return playing_cards_[i].end_disappearing();
	}
}

messages::cards_t CDrawer::get_choosen_card()
{
	for (int i = 0; i < 5; i++)
	{
		if (playing_cards_[i].choosen())
			return playing_cards_[i].card_type();
	}

	return messages::cards_t::c_0_unknown;
}

messages::cards_t CDrawer::get_choosen_card(messages::colors_t& color)
{
	for (int i = 0; i < 5; i++)
	{
		if (playing_cards_[i].choosen())
		{

			playing_cards_[i].choosen(false);
			color = playing_cards_[i].clarified_color();
			messages::cards_t	card = playing_cards_[i].card_type();
			playing_cards_[i].set(messages::cards_t::c_0_unknown);

			return card;
		}
	}

	return messages::cards_t::c_0_unknown;
}

bool CDrawer::init_color_clarification(messages::cards_t card, const std::set<messages::colors_t>& cards)
{
	color_clarification_.reset_variants();

	if (!cards.size())
		return false;

	for (int i = 0; i < 5; i++)
	{
		if (playing_cards_[i].card_type() == card)
		{
			playing_cards_[i].choosen(true);
			break;
		}
	}

	std::set<messages::colors_t>::const_iterator	it_card;
	for (it_card = cards.begin(); it_card != cards.end(); it_card++)
		color_clarification_.add_variant(*it_card);

	color_clarification_.add_variant(messages::colors_t::unknown);	// Для отмены
	color_clarification_.init_positions(game_field_.card_field_rect_);

	return true;
}

bool CDrawer::color_clarification_begin()
{
	bool increased = false;
	bool decreased = false;

	increased = color_clarification_.increase() ;
	for (int i = 0; i < 5; i++)
	{
		decreased = playing_cards_[i].hide_for_color_clarification();
	}

	return (increased || decreased);
}

bool CDrawer::color_clarification_end()
{
	bool increased = false;
	bool decreased = false;

	decreased = color_clarification_.decrease();

	for (int i = 0; i < 5; i++)
	{
		if (playing_cards_[i].choosen())
		{
			playing_cards_[i].disappear_after_color_clarification();
		}
		else
			increased = playing_cards_[i].restore_after_color_clarification();
	}

	return (increased || decreased);
}

void CDrawer::discard_choice()
{
	for (int i = 0; i < 5; i++)
	{
		if (playing_cards_[i].choosen())
		{
			playing_cards_[i].choosen(false);
			playing_cards_[i].set_clarified_color(messages::colors_t::unknown);
			return;
		}
	}
}

bool CDrawer::is_turn_possible()
{
	for (int i = 0; i < 5; i++)
	{
		if (possible_turtles_to_move(playing_cards_[i].card_type()).size())
			return true;
	}

	return false;
}

