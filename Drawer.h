#pragma once
#include "afxwin.h"
#include <vector>
#include <set>
#include <string>
#include "Message.h"
#include "BSpliner.h"

class CDrawer
{
	struct title_running_turtle_t
	{
		title_running_turtle_t() : __x(0), __y(0), _x(0), _y(0), __y_up(0), _y_down(0)
		{};

		int __x;
		int __y;
		int _x;
		int _y;

		int __y_up;
		int _y_down;
	};

	struct title_running_turtles_set_t
	{
		title_running_turtles_set_t()
		{
		};

		void init(CRect& rect)
		{
			running_zone_ = rect;

			int h_turtlesize = 110;
			int v_turtlesize = 62;
			int h_distance = (rect.right + h_turtlesize) / 5;
			int v_distance = rect.bottom / 5;

			for (int i = 0; i < 5; i++)
			{
				title_turtles_[i].__x = rect.left - h_turtlesize + (h_distance / 2) - i * h_distance;
				title_turtles_[i].__y = v_distance - v_turtlesize / 2;

				title_turtles_[i]._x = rect.left - h_turtlesize - i * h_distance;
				title_turtles_[i]._y = rect.bottom - v_distance - v_turtlesize / 2;
			}
		};

		void step(bool away)
		{
			for (int i = 0; i < 5; i++)
			{
				title_turtles_[i].__x += 2;
				if (title_turtles_[i].__x >= running_zone_.right)
					title_turtles_[i].__x = -110;

				title_turtles_[i]._x += 3;
				if (title_turtles_[i]._x >= running_zone_.right)
					title_turtles_[i]._x = -110;

				if (away)
				{
					title_turtles_[i].__y_up -= 3;
					title_turtles_[i]._y_down -= 12;
				}
			}
		};

		void start_away()
		{
			for (int i = 0; i < 5; i++)
			{
				title_turtles_[i].__y_up = title_turtles_[i].__y;
				title_turtles_[i]._y_down = title_turtles_[i]._y;
			}
		}

		CRect						running_zone_;
		title_running_turtle_t		title_turtles_[5];
	};

	struct color_card_for_choose_t
	{
		color_card_for_choose_t() : x(0), y(0), width(1), choosen_(false)
		{
		};

		int x;
		int y;
		int width;
		bool choosen_;
	};

	struct color_card_chooser_t
	{
		void init(const CRect& rect)
		{
			for (int i = 0; i < 5; i++)
			{
				color_cards_for_choose_[i].y = rect.top + rect.Height() / 2 - 96;
				color_cards_for_choose_[i].x = rect.left + rect.Width() / 2 + (i - 2) * 150;
				color_cards_for_choose_[i].width = 1;
			}
		};

		bool increase()
		{
			for (int i = 0; i < 5; i++)
			{
				color_cards_for_choose_[i].x -= 1;
				color_cards_for_choose_[i].width += 2;
			}

			if (color_cards_for_choose_[0].width > 95)
				return false;
			else
				return true;
		};

		bool decrease()
		{
			for (int i = 0; i < 5; i++)
			{
				color_cards_for_choose_[i].x += 1;
				color_cards_for_choose_[i].width -= 2;
			}

			if (color_cards_for_choose_[0].width < 2)
				return false;
			else
				return true;
		};

		bool is_color_choosen(const CPoint& point)
		{
			for (int i = 0; i < 5; i++)
			{
				if (point.x > color_cards_for_choose_[i].x && point.x < color_cards_for_choose_[i].x + 96 &&
					point.y > color_cards_for_choose_[i].y && point.y < color_cards_for_choose_[i].y + 96)
				{
					color_cards_for_choose_[i].choosen_ = true;
					return true;
				}
			}

			return false;
		}

		color_card_for_choose_t			color_cards_for_choose_[5];
	};

	class CPlayingCard
	{
	public:
		CPlayingCard() : card_type_(messages::cards_t::c_0_unknown), h_center_(0), v_center_(0), orig_v_center_(0), choosen_(false), clarified_color_(messages::colors_t::unknown)
		{
		}
		~CPlayingCard() {};

		messages::cards_t	card_type() { return card_type_; }
		const CRect& card_place() { return card_place_; }
		bool				choosen() { return choosen_; }
		void				choosen(bool val) { choosen_ = val; }

		void init_on_deal(messages::cards_t card, int h_center, int v_center)
		{
			card_type_ = card;
			h_center_ = h_center;
			v_center_ = v_center;
			orig_v_center_ = v_center_;
			card_place_ = CRect(h_center_ - 48, v_center_, h_center_ + 48, v_center_);
		}
		void remove()
		{
			card_type_ = messages::cards_t::c_0_unknown;
			card_place_ = CRect(h_center_, v_center_, h_center_, v_center_);
		}
		void set(messages::cards_t card)
		{
			v_center_ = orig_v_center_;
			card_type_ = card;
			card_place_ = CRect(h_center_ - 48, v_center_, h_center_ + 48, v_center_);
		}
		bool increase()
		{
			if (card_place_.Height() < 144)
			{
				card_place_.top -= 2;
				card_place_.bottom += 2;
				return true;
			}
			return false;
		}
		bool point_in(CPoint& point)
		{
			if (card_place_.left < point.x && card_place_.right > point.x &&
				card_place_.top < point.y && card_place_.bottom > point.y)
				return true;
			else
				return false;
		}
		void start_disappearing()
		{
			choosen_ = true;
			card_place_ = CRect(h_center_ - 48, v_center_ - 72, h_center_ + 48, v_center_ + 72);
		}
		void end_disappearing()
		{
			card_type_ = messages::cards_t::c_0_unknown;
			choosen_ = false;
		}

		bool step_disappearing(bool slow = false)
		{
			if (card_place_.Width() == 0)
			{
				card_place_ = CRect(h_center_ - 48, v_center_ - 72, h_center_ + 48, v_center_ + 72);
				return false;
			}

			card_place_.left += (slow ? 1 : 2);
			if (card_place_.left > h_center_)
				card_place_.left = h_center_;

			card_place_.right -= (slow ? 1 : 2);
			if (card_place_.right < h_center_)
				card_place_.right = h_center_;

			card_place_.top += (slow ? 3 : 6);
			if (card_place_.top > v_center_)
				card_place_.top = v_center_;

			card_place_.bottom -= (slow ? 3 : 6);
			if (card_place_.bottom < v_center_)
				card_place_.bottom = v_center_;

			return true;
		}

		bool hide_for_color_clarification()
		{
			if (card_place_.Height() > 27)
			{
				card_place_.bottom -= 3;
				return true;
			}

			return false;
		}

		bool restore_after_color_clarification()
		{
			if (card_place_.Height() < 144)
			{
				card_place_.bottom += 3;
				return true;
			}

			return false;
		}

		bool disappear_after_color_clarification()
		{
			if (choosen_)
			{
				if (card_place_.Height())
				{
					card_place_.bottom -= 3;
					return true;
				}
			}

			return false;
		}

		void set_clarified_color(messages::colors_t color)
		{
			clarified_color_ = color;
		}

		messages::colors_t clarified_color() { return clarified_color_; };

	private:
		messages::cards_t	card_type_;
		int					h_center_;
		int					v_center_;
		int					orig_v_center_;

		bool				choosen_;
		messages::colors_t	clarified_color_;
		CRect				card_place_;
	};

	struct running_turtle_for_return_t
	{
		running_turtle_for_return_t() : color_(messages::colors_t::unknown)
		{};

		std::string name()
		{
			if (current_position_ == destination_)
				return name_;
			else
				return "";
		}

		CPoint				current_position_;
		CPoint				destination_;
		CPoint				step_;
		messages::colors_t	color_;
		std::string			name_;
	};

	struct running_cabbage_t
	{
		running_cabbage_t() : angle_(0)
		{
			position_.Offset(830, -86);
		};
		
		CPoint	position_;
		int		angle_;
	};

	struct ending_game_t
	{
		ending_game_t() : inited_(false) {};

		bool						inited_;
		running_turtle_for_return_t	turtles_[5];
		CPlayingCard				playing_cards_[5];
		CRect						stones_[11];
		CRect						winner_turtle_;
		running_cabbage_t			cabbages_[8];
	};

	struct game_field_t {
		game_field_t() : cabbage_left_side_(0), rocks_height_(0), rocks_bottom_(0), turtles_position_(0)//, field_prepared_(false)
		{
		};

		void prepare(CRect& rect)
		{
			int v_border = int( double(rect.Height()) * 0.65);
			int h_border = int ( double(rect.Width()) * 0.21);

			main_field_rect_.SetRect(rect.left+1, rect.top+1, rect.right-1, v_border -1);
			info_field_rect_.SetRect(rect.left + 1, v_border, h_border - 1, rect.bottom - 1);	// 454
			card_field_rect_.SetRect(h_border, v_border, rect.right - 1, rect.bottom - 1);

			start_button_ext_rectangle_.SetRect(card_field_rect_.right - 320,
				card_field_rect_.CenterPoint().y - 40,
				card_field_rect_.right - 20,
				card_field_rect_.CenterPoint().y + 40);

			start_button_int_rectangle_.SetRect(card_field_rect_.right - 310,
				card_field_rect_.CenterPoint().y - 30,
				card_field_rect_.right - 30,
				card_field_rect_.CenterPoint().y + 30);

			skip_button_ext_rectangle_.SetRect(card_field_rect_.right - 180, 
				card_field_rect_.CenterPoint().y + 50,
				card_field_rect_.right - 10, 
				card_field_rect_.CenterPoint().y + 115);
			skip_button_int_rectangle_.SetRect(card_field_rect_.right - 175,
				card_field_rect_.CenterPoint().y + 55,
				card_field_rect_.right - 15,
				card_field_rect_.CenterPoint().y + 110);


			cabbage_left_side_	= main_field_rect_.right;
			rocks_height_		= 0;
			rocks_bottom_		= main_field_rect_.bottom - 2;
		};

		void prepare_static_oblects_to_place(const CPoint& choosen_card_position)
		{
			cabbage_left_side_	= main_field_rect_.right;
			rocks_height_		= 0;
			rocks_bottom_		= main_field_rect_.bottom - 2;

			my_card_pos_ = choosen_card_position;
			my_card_end_pos_ = CPoint(info_field_rect_.CenterPoint().x - 48, my_card_pos_.y);

			my_card_step_.x = (my_card_pos_.x - my_card_end_pos_.x) / 80;

			race_track_.init();
			turtles_position_ = main_field_rect_.left - 112;
		}

		bool step_static_oblects()
		{
			bool step_made = false;

			if (cabbage_left_side_ > main_field_rect_.right - 102)
			{
				cabbage_left_side_--;
				step_made = true;
			}

			if (rocks_height_ < 60)
			{
				rocks_height_ += 2;
				step_made = true;
			}

			if (my_card_pos_.x > my_card_end_pos_.x)
			{
				my_card_pos_.x -= 4;
				if (my_card_pos_.x < my_card_end_pos_.x)
					my_card_pos_.x = my_card_end_pos_.x;
				step_made = true;
			}

			if (turtles_position_ < 5)
			{
				turtles_position_ += 1;
				step_made = true;
			}

			return step_made;
		}

		int			cabbage_left_side_;
		int			rocks_height_;
		int			rocks_bottom_;
		int			turtles_position_;
		CRect		main_field_rect_;
		CRect		info_field_rect_;
		CRect		card_field_rect_;
		CPoint		my_card_pos_;
		CPoint		my_card_end_pos_;
		CPoint		my_card_step_;
		CRect		start_button_ext_rectangle_;
		CRect		start_button_int_rectangle_;
		CRect		skip_button_ext_rectangle_;
		CRect		skip_button_int_rectangle_;

		messages::race_track_t	race_track_;
	};

	struct color_card_for_clarification_t
	{
		color_card_for_clarification_t(messages::colors_t color) : color_(color), choosen_(false)
		{
		};

		bool is_point_over(const CPoint& point)
		{
			if (point.x > place_.left && point.x < place_.right && point.y > place_.top && point.y < place_.bottom)
				return true;

			return false;
		}

		CRect				place_;
		messages::colors_t	color_;
		bool				choosen_;
	};

	struct color_clarification_t
	{
		color_clarification_t() : cards_size_(0)
		{
		};

		void reset_variants()
		{
			color_variants_.clear();
		};

		void add_variant(messages::colors_t color)
		{
			color_card_for_clarification_t	variant(color);

			color_variants_.push_back(variant);
		};

		void init_positions(CRect& zone)
		{
			int bottom_line = zone.top + ((zone.bottom - zone.top) / 2) - 30 + 72;
			int center_line = zone.left + ((zone.right - zone.left) / 2);
			cards_size_ = 0;
			int i_card = 0;
			int total_cards_on_side = int(color_variants_.size() / 2);

			std::vector<color_card_for_clarification_t>::iterator it_card;
			for (it_card = color_variants_.begin(); it_card != color_variants_.end(); it_card++)
			{
				it_card->place_.bottom = it_card->place_.top = bottom_line;

				if (color_variants_.size() % 2)
				{	// нечетное число
					it_card->place_.left = center_line + (i_card - total_cards_on_side) * (96 + 48);
					it_card->place_.right = center_line + (i_card - total_cards_on_side) * (96 + 48) + 96;
				}
				else
				{	// четное число
					it_card->place_.left = center_line + (i_card - total_cards_on_side) * (96 + 48);
					it_card->place_.right = center_line + (i_card - total_cards_on_side) * (96 + 48) + 96;
				}

				i_card++;
			}
		};

		bool increase()
		{
			cards_size_ += 2;

			std::vector<color_card_for_clarification_t>::iterator it_card;
			for (it_card = color_variants_.begin(); it_card != color_variants_.end(); it_card++)
				it_card->place_.top = it_card->place_.bottom - cards_size_;

			return (cards_size_ < 96);
		}

		bool decrease()
		{
			if (cards_size_ > 0)
			{
				cards_size_ -= 2;

				if (cards_size_ < 0)
					cards_size_ = 0;

				std::vector<color_card_for_clarification_t>::iterator it_card;
				for (it_card = color_variants_.begin(); it_card != color_variants_.end(); it_card++)
					it_card->place_.top = it_card->place_.bottom - cards_size_;

				return true;
			}

			return false;
		}

		std::vector<color_card_for_clarification_t>	color_variants_;
		int cards_size_;
	};
	
	game_field_t				game_field_;
	title_running_turtles_set_t	title_runnung_turtles_;
	color_card_chooser_t		color_card_chooser_;

	std::vector<std::string>	gamers_list_;

	std::string					gamer_name_;
	messages::colors_t			my_turtle_color_;
	messages::colors_t			winner_color_;

	CPlayingCard				playing_cards_[5];
	color_clarification_t		color_clarification_;
	ending_game_t				ending_game_;
	std::string					winner_name_;
	B_Spliner					spliner_;
	messages::race_track_t		temporary_saved_track_;
	std::vector<messages::colors_t>	jumping_turtles_;
public:
	CDrawer();
	~CDrawer();

	void test_output(CDC* pDC, CRect& client);
	void generate_turtle_move(messages::colors_t color, int steps);
	void set_track(messages::race_track_t& track) { /*game_field_.race_track_*/ temporary_saved_track_ = track; };

	void init_client_rectangle(CRect& client);
	void draw_title_turtles(CDC* pDC, bool away = false);
	void title_turtles_step(bool away = false) { title_runnung_turtles_.step(away); };

	void title_turtles_start_away() { title_runnung_turtles_.start_away(); };
	void init_cards_for_choose()
	{
		color_card_chooser_.init(game_field_.card_field_rect_);
		game_field_.race_track_.init();
		temporary_saved_track_ = game_field_.race_track_;
	};
	bool cards_for_choose_increase() { return color_card_chooser_.increase(); };
	bool cards_for_choose_decrease() { return color_card_chooser_.decrease(); };
	void draw_cards_for_choose(CDC* pDC, CRect& client);
	void draw_choosen_card(CDC* pDC, CRect& client);

	void set_gamer_name(const std::string& gamer_name) { gamer_name_ = gamer_name; };
	void update_gamers_list(const std::vector<std::string>& gamers_list) { gamers_list_ = gamers_list; };
	void set_my_turtle_color(messages::colors_t color) { my_turtle_color_ = color; };

	void draw_empty_field(CDC* pDC);
	void init_game_ending(messages::colors_t winner_color, const std::map<messages::colors_t, std::string>& gamers_colors);
	bool step_game_ending();
	void step_cabbage();
	void draw_game_ending(CDC* pDC);
	void init_game_objects();
	bool step_game_objects();
	void draw_game_objects(CDC* pDC);
	void draw_start_button(CDC* pDC, CRect& client);
	void draw_end_button(CDC* pDC);
	void draw_skip_button(CDC* pDC);
	void write_game_info(CDC* pDC, const char* text);

	void write_gamers_list(CDC* pDC, CRect& client);

	bool is_start_pressed(CPoint point);
	bool is_skip_pressed(CPoint point);
	bool is_color_choosen(const CPoint& point) { return color_card_chooser_.is_color_choosen(point); };
	int	is_color_clarified(CPoint& point);

	void init_playing_cards(const std::vector<messages::cards_t>& cards);
	void remove_card(messages::cards_t card);
	void new_card(messages::cards_t card);
	bool increase_playing_cards();
	void draw_playing_cards(CDC* pDC, CRect& client);
	messages::cards_t	card_under_cursor(CPoint& point);

	std::set<messages::colors_t>	possible_turtles_to_move(messages::cards_t card);
	messages::cards_t get_choosen_card();
	messages::cards_t get_choosen_card(messages::colors_t& color);
	void	init_card_disappearing(messages::cards_t card);
	bool	disappear_choosen_card();
	void	end_card_disappearing();
	bool	init_color_clarification(messages::cards_t card, const std::set<messages::colors_t>& cards);
	bool	color_clarification_begin();
	bool	color_clarification_end();
	void	discard_choice();
	bool	is_turn_possible();
private:
	CPen		m_ButtonPen;
	CPen		m_BorderPen;
	CBrush		m_GreenBrush;
	CBrush		m_WhiteBrush;
	CBrush		m_DarkGrayBrush;
	CBrush		m_LightGrayBrush;

	CFont		m_GameTextFont;
	int			m_iGameTextHeight;
	CFont		m_InfoTextFont;
	int			m_iInfoTextHeight;

	CBitmap		bm_cabbage_and_;
	CBitmap		bm_cabbage_or_;

	CBitmap		bm_running_turtle_and_;
	CBitmap		bm_running_turtle_blue_;
	CBitmap		bm_running_turtle_green_;
	CBitmap		bm_running_turtle_magenta_;
	CBitmap		bm_running_turtle_red_;
	CBitmap		bm_running_turtle_yellow_;

	CBitmap		bm_choose_color_white_;
	CBitmap		bm_choose_color_blue_;
	CBitmap		bm_choose_color_green_;
	CBitmap		bm_choose_color_magenta_;
	CBitmap		bm_choose_color_red_;
	CBitmap		bm_choose_color_yellow_;
	CBitmap		bm_choose_color_cancel_;

	CBitmap		bm_blue_plusplus_;
	CBitmap		bm_green_plusplus_;
	CBitmap		bm_magenta_plusplus_;
	CBitmap		bm_red_plusplus_;
	CBitmap		bm_yellow_plusplus_;
	CBitmap		bm_blue_plus_;
	CBitmap		bm_green_plus_;
	CBitmap		bm_magenta_plus_;
	CBitmap		bm_red_plus_;
	CBitmap		bm_yellow_plus_;
	CBitmap		bm_blue_minus_;
	CBitmap		bm_green_minus_;
	CBitmap		bm_magenta_minus_;
	CBitmap		bm_red_minus_;
	CBitmap		bm_yellow_minus_;
	CBitmap		bm_nocolor_up_;
	CBitmap		bm_nocolor_upup_;
	CBitmap		bm_nocolor_plus_;
	CBitmap		bm_nocolor_minus_;

	CBitmap		bm_stone_and_;
	CBitmap		bm_stone_or_;
};
