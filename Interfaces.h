#pragma once
#include <string>

#include "Message.h"

class CClientSocket;

struct socket_interface
{
	virtual void new_client_connection() = 0;
	virtual void client_disconnection(CClientSocket*) = 0;
	virtual void process_message(CClientSocket*, messages::Message*) = 0;
};

struct game_interface
{
	virtual void log_event(std::string event) = 0;
	virtual void server_started() = 0;
	virtual void server_start_failed(std::string reason) = 0;
	virtual void client_started() = 0;
	virtual void client_start_failed(std::string reason) = 0;
	virtual void logon_accepted(std::string name) = 0;
	virtual void logon_rejected(std::string name, std::string reason) = 0;
	virtual void update_gamers_list(const std::vector<std::string>& gamers) = 0;
	virtual void set_turtle_color(messages::colors_t color) = 0;
	virtual void cards_deal(const std::vector<messages::cards_t>& cards) = 0;
	virtual void your_turn_now() = 0;
	virtual void move_ack(std::string& name, messages::colors_t color, int steps, messages::race_track_t& snapshot) = 0;
	virtual void new_card(messages::cards_t card) = 0;
	virtual void game_over(messages::colors_t winner_color, const std::map<messages::colors_t, std::string>& gamers_colors) = 0;
};