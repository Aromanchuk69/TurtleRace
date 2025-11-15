#pragma once
#include <string>
#include <set>

class Configurator
{
public:
	Configurator();
	~Configurator();
	void clear();

	int load(const std::string& cfg_file);
	int save();

	const std::string& ip_address() const { return ip_address_; }
	void				ip_address(const std::string& ip_address) { ip_address_ = ip_address; }

	const std::string& user() const { return user_; }
	void				user(const std::string& user) { user_ = user; }

	int					port() const { return port_; }
	void				port(int port) { port_ = port; }

private:
	std::string	config_file_name_;
	std::string	user_;
	int			port_;
	std::string	ip_address_;
};
