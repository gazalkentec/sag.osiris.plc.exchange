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
	std::string _config_file_name = "service.config";

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

	void LoadConfig(int argc, TCHAR *argv[], TCHAR *env[])
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

			if (config.LoadFile())
			{
				try
				{

					TiXmlNode *root = config.FirstChild();

					TiXmlNode *service = root->GetUserData();

					int i = 0;

				}
				catch (...) {
				}
			}
		}

		/*



		_service_name = "sag.arachne.exchanger";
		_logger.LogName = _service_name;
		_logger.LogFileName = _logger.LogName + _logger.LogFileExtention;

		_logger.LogFilePath = std::string(buff.begin(), buff.end());



		////SERVICE_PARAMETERS.ServiceName = "sjdfajsf";

		//if (config.LoadFile())
		//{
		///*
		//<sag.osiris.plc.exchange>
		//<log>
		//<level>0</level>
		//<file_name>sag.osiris.plc.exchange.log</file_name>
		//<file_path>.\log</file_path>
		//</log>
		//</sag.osiris.plc.exchange>
		//*/
		///*		try {

		//WRITELOG(logger, framework::Diagnostics::LogLevel::Info, _T("Config is cuccessfully loaded... try to parse..."));


		//TiXmlElement* log = 0;
		//TiXmlElement* logFileName = 0;
		//TiXmlElement* logFilePath = 0;
		//TiXmlElement* loggingLevel = 0;

		//WRITELOG(logger, framework::Diagnostics::LogLevel::Info, LPWSTR(config.Value()));

		//return true;
		//}
		//catch(...) {

		//WRITELOG(logger, framework::Diagnostics::LogLevel::Error, _T("Config parse error... Server is shutdown..."));

		//goto EXIT;
		//}
		//}*/

		//SERVICE_PARAMETERS.ServiceParametersLoaded = true;

	}

	~Configurator()
	{
	}
};

