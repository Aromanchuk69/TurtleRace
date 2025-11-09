#pragma once
#include <afxsock.h>
#include "Interfaces.h"


class CServerSocket : public CSocket
{
public:
	CServerSocket(socket_interface* responcer) : responcer_(responcer)
	{};

	virtual ~CServerSocket()
	{};

	virtual void OnAccept(int nErrorCode)
	{
		CSocket::OnAccept(nErrorCode);
		responcer_->new_client_connection();
	};

	virtual void OnConnect(int nErrorCode)
	{
		CSocket::OnConnect(nErrorCode);
	};

	virtual void OnClose(int nErrorCode)
	{
		//responcer_->new_client_connection();
		CSocket::OnClose(nErrorCode);
	};
private:
	socket_interface*	responcer_;
};
