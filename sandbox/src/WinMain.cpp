#include "pch.h"
#include "Logger/FileLogger.h"
#include "Sandbox.h"

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nShowCmd)
{
	try
	{
		yoi::FileLogger::Info("App begin to Run.");
		yoi::FileLogger::Flush();
		yoi::Sandbox app;
		int ret = app.Run();
		std::ostringstream oss;
		oss << "App end. return code = " << ret;
		yoi::FileLogger::Info(oss.str().c_str());
		return app.Run();
	}
	catch (const yoi::YoiException& error)
	{
		MessageBoxA(nullptr, error.what(), error.GetType(), MB_OK | MB_ICONERROR);
	}
	catch (const std::exception& error)
	{
		MessageBoxA(nullptr, error.what(), "Standard Exception", MB_OK | MB_ICONERROR);
	}
	catch (...)
	{
		MessageBoxA(nullptr, "unknown error" , "Unidentified Error", MB_OK | MB_ICONERROR);
	}
	return -1;
}