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

#include "ServiceConfig.h"

inline LPTSTR ExtractFilePath(LPCTSTR FileName, LPTSTR buf)
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