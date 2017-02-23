#pragma once

#pragma comment (lib, "libnodave/libnodave.lib")

#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <cstdlib>
#include <fstream>

#include "logger/Logger.h"
#include "sqlite/sqlite3.h"

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