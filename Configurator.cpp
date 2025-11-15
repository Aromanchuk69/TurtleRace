#include <afxwin.h>
#include <afxcmn.h>
#include "Configurator.h"

Configurator::Configurator()
{
	clear();
}

Configurator::~Configurator()
{
}

void Configurator::clear()
{
	user_ = "";
	port_ = 54321;
	ip_address_ = "127.0.0.1";
}

int Configurator::load(const std::string& cfg_file)
{
	config_file_name_ = cfg_file;

	CString	readstring;
	CFile	fFile;
	if (!fFile.Open(config_file_name_.c_str(), CFile::modeRead))
		return -1;

	clear();

	CArchive ar(&fFile, CArchive::load);

	try
	{
		for (; ; )
		{
			if (!ar.ReadString(readstring))
				break;

			if (readstring[0] == '#')
				continue;

			int iPos = readstring.Find('=');
			if (iPos > 0)
			{
				CString	parameter = readstring.Left(iPos);
				CString	value = readstring.Mid(iPos + 1);
				std::string str_val = LPCTSTR(value);

				if (parameter == "User")
					user_ = str_val;
				else
				if (parameter == "ConnectionHost")
					ip_address_ = str_val;
				else
				if (parameter == "ConnectionPort")
					port_ = atoi(LPCTSTR(value));
			}
		}
	}
	catch (...)
	{
		ar.Close();
		fFile.Close();
		return -1;
	}

	ar.Close();
	fFile.Close();
	return 0;
}

int Configurator::save()
{
	CFile	fFile;

	if (!fFile.Open(config_file_name_.c_str(), CFile::modeWrite | CFile::modeCreate))
	{
		AfxMessageBox("Невозможно открыть файл конфигурации!", MB_ICONINFORMATION);
		return -1;
	}

	try
	{
		CArchive ar(&fFile, CArchive::store);
		char	buffer[2048];

		ar.WriteString("# Librarian Config File\r\n");

		sprintf(buffer, "User=%s\n"
			"ConnectionHost=%s\n"
			"ConnectionPort=%d\n",
			user_.c_str(),
			ip_address_.c_str(),
			port_);

		ar.WriteString(buffer);
	}
	catch (...)
	{
		AfxMessageBox("Ошибка при сохранении конфигурации!", MB_ICONSTOP);
	}

	return 0;
}

