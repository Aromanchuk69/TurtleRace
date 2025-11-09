#pragma once
#include <afxsock.h>
#include <list>
#include <string>
#include <mutex>
#include <thread>
#include "Interfaces.h"
#include "Message.h"

class CClientSocket : public CSocket
{
	char				sending_buffer_[8192];
	char				receiving_buffer_[8192];
	int					current_receiving_position_;	// the current position in reseiving_buffer_

	socket_interface*	responser_;
	std::string			client_name_;
	std::mutex			read_socket_mutex_;

public:
	CClientSocket(socket_interface* responser);
	~CClientSocket();

	//bool Send(Message* message);

	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);

	bool send_message(messages::Message& message);

	//const	std::string& client_name() { return client_name_; }
	//void	client_name(const std::string& name) { client_name_ = name; }

	//void	send_logon_request(long required_version);
	//void	send_logon_response(logon_response_t response);
	//void	send_snapshot(Snapshot& snapshot);
	//void	send_snapshot_request();

	//void	send_file_init(const std::string& server_path, const std::string& client_path);
	//void	send_file();

	//void	receive_file_init(const std::string& client_path, const std::string& server_path);
	//void	rename_file_request(const std::string& transfer_from_, const std::string& transfer_to_);
	//void	erase_file_request(const std::string& transfer_from_, const std::string& transfer_to_);
	//void	send_transfer_result(const std::string& file_name, __int64 file_length, file_transfer_marker_t result);
};
