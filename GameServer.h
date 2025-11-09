#pragma once
#include "Interfaces.h"
#include "ServerSocket.h"
#include "ClientSocket.h"
#include <set>
#include <map>
#include <mutex>

class CGameServer : public socket_interface
{
	enum class game_stage_t
	{
		accept_gamers,
		choosing_a_color,
	};

	struct gamer_info_t
	{
		gamer_info_t() : socket_(0), ready_to_start_(false), ready_for_race_(false), turtle_color_(messages::colors_t::unknown)
		{
		};

		messages::colors_t	turtle_color_;
		CClientSocket*		socket_;
		bool				ready_to_start_;
		bool				ready_for_race_;
	};

	void refresh_gamers_list();
	void send_news_to_all(std::string info, CClientSocket* except = NULL);
	void ShuffleColors(int* pColors);
	void InitCards();
	void ShuffleCards();
public:
	CGameServer() : server_socket_(NULL), responser_(NULL), game_stage_(game_stage_t::accept_gamers) {};
	~CGameServer() {};

	void start_server(int port, game_interface* responser);
	void stop_server();
	bool is_started() { return (server_socket_ != NULL); }

	virtual void new_client_connection();
	virtual void client_disconnection(CClientSocket* socket);
	virtual void process_message(CClientSocket* socket, messages::Message* message);

private:
	game_interface*						responser_;
	CServerSocket*						server_socket_;
	std::mutex							message_processing_mutex_;
	std::map<std::string, gamer_info_t>	gamers_;
	std::set<CClientSocket*>			sockets_wo_logon_;
	game_stage_t						game_stage_;
	std::vector<messages::cards_t>		cards_to_play_;
	std::vector<messages::cards_t>		played_cards_;
	std::string							next_gamer_;
	messages::race_track_t				game_track_;
};