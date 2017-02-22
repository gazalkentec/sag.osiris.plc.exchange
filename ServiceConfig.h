#pragma once

struct LoggerParameters {
	std::string LogName;
	std::string LogFilePath;
	std::string LogFileName;
	const std::string LogFileExtention = ".config";
	int LogLevel = 0;
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

class ServiceConfig
{
private:

	bool _is_loaded = false;
	std::string _service_name;

	LoggerParameters _logger;
	PLCParameters _plc;
	LocalDBParameters _localdb;
	MainDBParameters _maindb;

public:

	bool IsLoaded() { return _is_loaded; };

	const char* GetServiceNameC() { return _service_name.c_str(); }

	LoggerParameters GetLogger() { return _logger; };
	PLCParameters GetPLC() { return _plc; };
	LocalDBParameters GetLocalDB() { return _localdb; };
	MainDBParameters GetMainDB() { return _maindb; };


	ServiceConfig()
	{
		_service_name = "sag.arachne.exchanger";
		_is_loaded = true;
	}

	~ServiceConfig()
	{
	}
};

