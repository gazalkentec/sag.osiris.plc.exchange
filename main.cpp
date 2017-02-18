//**********************************************************//
//															//
//	sag.osiris.plc.exchange									//
//															//
//**********************************************************//

#include <Windows.h>
#include <tchar.h>

#include "stdafx.h"

using namespace std;
using namespace framework::Diagnostics;
using namespace framework::Threading;

SERVICE_STATUS        g_ServiceStatus = { 0 };
SERVICE_STATUS_HANDLE g_StatusHandle = NULL;
HANDLE                g_ServiceStopEvent = INVALID_HANDLE_VALUE;

#define SERVICE_NAME  _T("sag.osiris.plc.exchange")

VOID WINAPI ServiceMain(DWORD argc, LPTSTR *argv);

VOID WINAPI ServiceCtrlHandler(DWORD);

DWORD WINAPI ServiceWorkerThread(LPVOID lpParam);
DWORD WINAPI PLCExchangeWorker(LPVOID lpParam);
DWORD WINAPI MAINDBExchangeWorker(LPVOID lpParam);

CLogger<CNoLock> logger(LogLevel::Info, SERVICE_NAME);

ServiceParameters SERVICE_PARAMETERS;

void LoadConfig(int argc, TCHAR *argv[])
{

	TiXmlDocument config("C:/c++/sag.osiris.plc.exchange/x64/Debug/sag.osiris.plc.exchange.xml");

	TCHAR szFileName[MAX_PATH], szPath[MAX_PATH];
	GetModuleFileName(0, szFileName, MAX_PATH);
	ExtractFilePath(szFileName, szPath);

	SERVICE_PARAMETERS.ServiceName = "sjdfajsf";

	//config.Parse(ParsePattern);

	//if (config.Error())
	//{
	//	WRITELOG(logger, framework::Diagnostics::LogLevel::Error, _T("Config file is corrupt... Server is shutdown...");
	//}
/*
	if (config.LoadFile())
	{
		/*
		<sag.osiris.plc.exchange>
			<log>
				<level>0</level>
				<file_name>sag.osiris.plc.exchange.log</file_name>
				<file_path>.\log</file_path>
			</log>
		</sag.osiris.plc.exchange>
		*/
/*		try {

			WRITELOG(logger, framework::Diagnostics::LogLevel::Info, _T("Config is cuccessfully loaded... try to parse..."));

			TiXmlElement *root = config.FirstChildElement("sag.osiris.plc.exchange");

			TiXmlElement* log = 0;
			TiXmlElement* logFileName = 0;
			TiXmlElement* logFilePath = 0;
			TiXmlElement* loggingLevel = 0;

			WRITELOG(logger, framework::Diagnostics::LogLevel::Info, LPWSTR(config.Value()));

			return true;
		}
		catch(...) {

			WRITELOG(logger, framework::Diagnostics::LogLevel::Error, _T("Config parse error... Server is shutdown..."));

			goto EXIT;
		}
	}*/

	SERVICE_PARAMETERS.ServiceParametersLoaded = true;

}

int _tmain(int argc, TCHAR *argv[])
{

	logger.AddOutputStream(new std::wofstream("c:/temp/sag.osiris.plc.exchange.log"), true);//, framework::Diagnostics::LogLevel::Info);

	WRITELOG(logger, framework::Diagnostics::LogLevel::Info, _T("Server is try to start..."));

	LoadConfig(argc, argv);

	if (!SERVICE_PARAMETERS.ServiceParametersLoaded)
	{

		WRITELOG(logger, framework::Diagnostics::LogLevel::Info, _T("Config is not loaded! Server is shutdown..."));

		return -1;
	}

	SERVICE_TABLE_ENTRY ServiceTable[] =
	{
		{ SERVICE_NAME, (LPSERVICE_MAIN_FUNCTION)ServiceMain },
		{ NULL, NULL }
	};

	if (StartServiceCtrlDispatcher(ServiceTable) == FALSE)
	{
		WRITELOG(logger, framework::Diagnostics::LogLevel::Error, _T("Server start error! Shutdown!"));
		return GetLastError();
	}

	WRITELOG(logger, framework::Diagnostics::LogLevel::Info, _T("Server is stopped..."));

	return ERROR_SUCCESS;
}


VOID WINAPI ServiceMain(DWORD argc, LPTSTR *argv)
{
	DWORD Status = E_FAIL;

	g_StatusHandle = RegisterServiceCtrlHandler(SERVICE_NAME, ServiceCtrlHandler);

	if (g_StatusHandle == NULL)
	{
		WRITELOG(logger, framework::Diagnostics::LogLevel::Error, _T("Server start error! Is will be stopped!"));
		goto EXIT;
	}

	// Tell the service controller we are starting
	ZeroMemory(&g_ServiceStatus, sizeof(g_ServiceStatus));
	g_ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	g_ServiceStatus.dwControlsAccepted = 0;
	g_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	g_ServiceStatus.dwWin32ExitCode = 0;
	g_ServiceStatus.dwServiceSpecificExitCode = 0;
	g_ServiceStatus.dwCheckPoint = 0;

	if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE)
	{
		//OutputDebugString(_T("My Sample Service: ServiceMain: SetServiceStatus returned error"));
	}

	/*
	* Perform tasks neccesary to start the service here
	*/
	//OutputDebugString(_T("My Sample Service: ServiceMain: Performing Service Start Operations"));

	// Create stop event to wait on later.
	g_ServiceStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (g_ServiceStopEvent == NULL)
	{
		//OutputDebugString(_T("My Sample Service: ServiceMain: CreateEvent(g_ServiceStopEvent) returned error"));

		g_ServiceStatus.dwControlsAccepted = 0;
		g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		g_ServiceStatus.dwWin32ExitCode = GetLastError();
		g_ServiceStatus.dwCheckPoint = 1;

		if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE)
		{
			//OutputDebugString(_T("My Sample Service: ServiceMain: SetServiceStatus returned error"));
		}
		goto EXIT;
	}

	// Tell the service controller we are started
	g_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	g_ServiceStatus.dwWin32ExitCode = 0;
	g_ServiceStatus.dwCheckPoint = 0;

	if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE)
	{
		//OutputDebugString(_T("My Sample Service: ServiceMain: SetServiceStatus returned error"));
	}

	// Start the thread that will perform the main task of the service
	HANDLE hThread = CreateThread(NULL, 0, ServiceWorkerThread, NULL, 0, NULL);

	// Wait until our worker thread exits effectively signaling that the service needs to stop
	WaitForSingleObject(hThread, INFINITE);


	/*
	* Perform any cleanup tasks
	*/

	CloseHandle(g_ServiceStopEvent);

	g_ServiceStatus.dwControlsAccepted = 0;
	g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
	g_ServiceStatus.dwWin32ExitCode = 0;
	g_ServiceStatus.dwCheckPoint = 3;

	if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE)
	{
		//OutputDebugString(_T("My Sample Service: ServiceMain: SetServiceStatus returned error"));
	}

EXIT:

	return;
}


VOID WINAPI ServiceCtrlHandler(DWORD CtrlCode)
{
	//OutputDebugString(_T("My Sample Service: ServiceCtrlHandler: Entry"));

	switch (CtrlCode)
	{
	case SERVICE_CONTROL_STOP:

		WRITELOG(logger, framework::Diagnostics::LogLevel::Info, _T("Server stop request. Service will be stopped..."));

		if (g_ServiceStatus.dwCurrentState != SERVICE_RUNNING)
			break;

		/*
		* Perform tasks neccesary to stop the service here
		*/

		g_ServiceStatus.dwControlsAccepted = 0;
		g_ServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
		g_ServiceStatus.dwWin32ExitCode = 0;
		g_ServiceStatus.dwCheckPoint = 4;

		if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE)
		{
			//OutputDebugString(_T("My Sample Service: ServiceCtrlHandler: SetServiceStatus returned error"));
		}

		// This will signal the worker thread to start shutting down
		SetEvent(g_ServiceStopEvent);

		break;

	default:
		break;
	}

	//OutputDebugString(_T("My Sample Service: ServiceCtrlHandler: Exit"));
}


DWORD WINAPI ServiceWorkerThread(LPVOID lpParam)
{

	WRITELOG(logger, framework::Diagnostics::LogLevel::Info, _T("running..."));

	// Start the thread that will perform the main task of the service
	CreateThread(NULL, 0, MAINDBExchangeWorker, NULL, 0, NULL);
	Sleep(300);
	// Start the thread that will perform the main task of the service
	CreateThread(NULL, 0, PLCExchangeWorker, NULL, 0, NULL);
	Sleep(300);
	//  Periodically check if the service has been requested to stop
	while (WaitForSingleObject(g_ServiceStopEvent, 0) != WAIT_OBJECT_0)
	{


		//  Simulate some work by sleeping
		Sleep(550);
	}

	WRITELOG(logger, framework::Diagnostics::LogLevel::Info, _T("exit..."));

	return ERROR_SUCCESS;
}


DWORD WINAPI PLCExchangeWorker(LPVOID lpParam)
{

	WRITELOG(logger, framework::Diagnostics::LogLevel::Info, _T("running..."));

	while (WaitForSingleObject(g_ServiceStopEvent, 0) != WAIT_OBJECT_0)
	{

		//  Simulate some work by sleeping
		Sleep(200);
	}

	WRITELOG(logger, framework::Diagnostics::LogLevel::Info, _T("exit..."));

	return ERROR_SUCCESS;
}

DWORD WINAPI MAINDBExchangeWorker(LPVOID lpParam)
{

	WRITELOG(logger, framework::Diagnostics::LogLevel::Info, _T("running..."));

	while (WaitForSingleObject(g_ServiceStopEvent, 0) != WAIT_OBJECT_0)
	{

		//  Simulate some work by sleeping
		Sleep(1000);
	}

	WRITELOG(logger, framework::Diagnostics::LogLevel::Info, _T("exit..."));

	return ERROR_SUCCESS;
}
