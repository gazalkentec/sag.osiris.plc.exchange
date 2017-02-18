#pragma once

#pragma comment (lib, "libnodave/libnodave.lib")
#pragma comment (lib, "tinyxml/tinyxml.lib")

#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <cstdlib>
#include <fstream>

#include "tinyxml/tinyxml.h"

#include "logger/Logger.h"
#include "sqlite/sqlite3.h"

struct LoggerParameters {
	std::string LogName;
	std::string LogFilePath;
	std::string LogFileName;
	int LogLevel;
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
	std::vector<LoggerParameters> Logger;
	std::vector<PLCParameters> PLC;
	std::vector<LocalDBParameters> LocalDB;
	std::vector<MainDBParameters> MainDB;
};

LPTSTR ExtractFilePath(LPCTSTR FileName, LPTSTR buf)
{
	int i, len = lstrlen(FileName);
	for (i = len - 1; i >= 0; i--)
	{
		if (FileName[i] == _T('\\'))
			break;
	}
	lstrcpyn(buf, FileName, i + 2);
	return buf;
}