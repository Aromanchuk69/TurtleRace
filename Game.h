#pragma once
#include <string>
#include "Interfaces.h"
#include "Drawer.h"
#include "GameServer.h"
#include "GameClient.h"
#include "Configurator.h"

enum class game_stage_t
{
	not_started_yet = 0,
	waiting_for_connect,
	waiting_for_press_start,
	waiting_for_all,
	prepare_to_color_choosing,
	color_choosing,
	decreasing_color_cards,
	increasing_choosen_card,
	preparing_to_cards_deal,
	waiting_for_cards,
	accepting_cards_deal,
	waiting_for_next_turn,
	my_turn_now,
	removing_chosen_card,
	waiting_for_move_ack,
	making_move,
	waiting_for_result,
	color_clarify_preparation,
	color_clarification,
	color_clarify_finishing,
	color_clarify_cancelation,
	waiting_for_skip_turn,
	ending_game,
	game_finished,

	finished,
};

class CGame : public game_interface
{
public:
	CGame();
	~CGame();

	void set_dialog(CDialog* pDlg) { pDlg_ = pDlg; };

	bool game_in_progress() const;
	void Draw(CDC* pDC, CRect& client);

	void init_client_rectangle(CRect client) { drawer_.init_client_rectangle(client); };
	void init_configuration();

	void get_credentials(std::string& ip_address, int& port, std::string& login)
	{
		ip_address = configuration_.ip_address();
		port = configuration_.port();
		login = configuration_.user();
	};

	void create_game(int& port, const std::string& login);
	void join_game(const std::string& ip_address, int& port, const std::string& login);
	void on_timer();
	void on_mouse_click(CPoint point);
	void on_mouse_move(CPoint point);

	void clear_game_info();
	void set_card_info(messages::cards_t card);

	virtual void log_event(std::string event);
	virtual void server_started();
	virtual void server_start_failed(std::string reason);

	virtual void client_started();
	virtual void client_start_failed(std::string reason);

	virtual void logon_accepted(std::string name);
	virtual void logon_rejected(std::string name, std::string reason);

	virtual void update_gamers_list(const std::vector<std::string>& gamers);
	virtual void set_turtle_color(messages::colors_t color);
	virtual void cards_deal(const std::vector<messages::cards_t>& cards);
	virtual void your_turn_now();
	virtual void move_ack(std::string& name, messages::colors_t color, int steps, messages::race_track_t& snapshot);
	virtual void new_card(messages::cards_t card);
	virtual void game_over(messages::colors_t winner_color, const std::map<messages::colors_t, std::string>& gamers_colors);

private:
	CDialog*		pDlg_;
	bool			local_game_;
	game_stage_t	game_stage_;
	CDrawer			drawer_;
	CGameServer		game_server_;
	CGameClient		game_client_;
	std::string		information_;
	Configurator	configuration_;
};