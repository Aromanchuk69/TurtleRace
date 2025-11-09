#pragma once
#include "Interfaces.h"
#include "ClientSocket.h"

class CGameClient : public socket_interface
{
public:
	CGameClient() : client_socket_(NULL), responser_(NULL), log_buffer_size_(256)
	{
		memset(log_buffer_, 0, log_buffer_size_);
	};
	~CGameClient() {};

	void start_client(const std::string& ip, int port, const std::string& login, game_interface* responser);
	void stop_client();
	bool ready_to_start();
	bool waiting_for_cards(bool first_deal);
	bool ready_for_next_move();
	bool move_done();
	bool move_skipped();
	bool my_move(messages::cards_t card, messages::colors_t color = messages::colors_t::unknown);

	virtual void new_client_connection() {};
	virtual void client_disconnection(CClientSocket* socket);
	virtual void process_message(CClientSocket* socket, messages::Message* message);

private:
	char log_buffer_[256];
	int	log_buffer_size_;

	std::string		gamer_name_;
	CClientSocket*	client_socket_;
	game_interface* responser_;
};