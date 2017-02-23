#pragma once

#include "stdafx.h"

#include "tinyxml/tinyxml.h"
#pragma comment (lib, "tinyxml/tinyxml.lib")

struct LoggerParameters {
	framework::Diagnostics::LogLevel LogLevel = framework::Diagnostics::LogLevel::Info;
	const std::string LogFileExtention = ".log";
	std::string LogName;
	std::string LogFilePath;
	std::string LogFileName;
};

struct PLCParameters {
	std::string PLCType;
	int PLCPortNumber;
	std::string PLCIPAddress;
	int PLCPollPeriodMSec;
};

struct LocalDBParameters {
	std::string LocalDBPath;
	std::string TrendsDBFileName;
	std::string MessagesDBFileName;
	std::string DictionariesDBFileName;
	std::string SecretsDBFileName;
};

struct MainDBParameters {
	std::string MainDBConnectionString;
	int MainDBPollPeriodMSec;
};

struct ServiceParameters {
	bool ServiceParametersLoaded;
	std::string ServiceName;

	LoggerParameters Logger;
	PLCParameters PLC;
	LocalDBParameters LocalDB;
	MainDBParameters MainDB;
};

class Configurator
{
private:

	bool _is_loaded = false;

	std::string _app_path;
	std::string _service_name;
	const std::string _config_file_name = "service.config";

	LoggerParameters _logger;
	PLCParameters _plc;
	LocalDBParameters _localdb;
	MainDBParameters _maindb;

public:

	const bool IsLoaded() { return _is_loaded; };
	const std::string AppPath() { return _app_path; };

	const std::string GetServiceName() { return _service_name; }
	const char* GetServiceNameC() { return _service_name.c_str(); }

	LoggerParameters GetLogger() { return _logger; };
	PLCParameters GetPLC() { return _plc; };
	LocalDBParameters GetLocalDB() { return _localdb; };
	MainDBParameters GetMainDB() { return _maindb; };


	Configurator()
	{
		_is_loaded = false;
	}

	bool LoadConfig(int argc, TCHAR *argv[], TCHAR *env[])
	{

		TCHAR szFileName[MAX_PATH];
		TCHAR szPath[MAX_PATH];

		GetModuleFileName(0, szFileName, MAX_PATH);
		ExtractFilePath(szFileName, szPath);

		std::wstring buff = szPath;

		_app_path = std::string(buff.begin(), buff.end());

		std::string configFile = _app_path + _config_file_name;

		TiXmlDocument config(configFile.c_str());

		if (!config.Error())
		{
			if (config.LoadFile(TIXML_ENCODING_UTF8))
			{
				try
				{
					TiXmlElement *service = config.FirstChildElement("service");

					if (service)
					{
						_service_name = service->Attribute("name");

						if (!_service_name.empty())
						{
							_logger.LogName = _service_name;
							_logger.LogFileName = _logger.LogName + _logger.LogFileExtention;
							_logger.LogFilePath = _app_path;

							TiXmlElement *logger = service->FirstChildElement("log");
							if (logger)
							{
								_logger.LogLevel = framework::Diagnostics::LogLevel(int(logger->Attribute("level")));
								_logger.LogFilePath += logger->Attribute("append_logfile_path");
								std::string buff = logger->Attribute("alter_logfile_name");

								if (!buff.empty())
								{
									_logger.LogFileName = buff;
								}
							}
							else
							{
								_is_loaded = false;
								return _is_loaded;
							}

							TiXmlElement *plc = service->FirstChildElement("plc");
							if (plc)
							{
								_plc.PLCType = plc->Attribute("type");
								_plc.PLCPollPeriodMSec = int(plc->Attribute("poll_period_msecv"));
								_plc.PLCPortNumber = int(plc->Attribute("port"));

							}
							else
							{
								_is_loaded = false;
								return _is_loaded;
							}

							//<plc poll_period_msecv = "500" ip_address = "192.168.1.100" port = "55555" / >
							//<localdb dictionaries = ".\db\dictionaries.db" messages = ".\db\messages.db" secrets = ".\db\secrets.db" trends = ".\db\trends.db" / >
							//<maindb user = "osiris_plc_exchange" userpass = "osiris_plc_exchange" poll_period_msec = "500" / >

						}
						else
						{
							_is_loaded = false;
							return _is_loaded;
						}
					}
				}
				catch (...)
				{
					_is_loaded = false;
					return _is_loaded;
				}
			}
		}


		//WRITELOG(logger, framework::Diagnostics::LogLevel::Info, _T("Config is cuccessfully loaded... try to parse..."));
		//WRITELOG(logger, framework::Diagnostics::LogLevel::Info, LPWSTR(config.Value()));
	}

	~Configurator()
	{
	}
};

