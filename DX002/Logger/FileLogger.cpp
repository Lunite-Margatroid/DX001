#include "pch.h"
#include "FileLogger.h"

namespace yoi
{
	FileLogger FileLogger::s_Instance;

	FileLogger::FileLogger() noexcept
	{
		m_pLogger = spdlog::basic_logger_mt("yoi_logger", "logs/log.txt");
#ifdef YOI_DEBUG
		m_pLogger->set_level(spdlog::level::level_enum::debug);
#else
		m_pLogger->set_level(spdlog::level::level_enum::info);
#endif // YOI_DEBUG
		
	}
	FileLogger& FileLogger::GetInstance()
	{
		return s_Instance;
	}
	void FileLogger::Warn(const char* warn)
	{
		GetInstance().m_pLogger->warn(warn);

	}
	void FileLogger::Info(const char* info)
	{
		GetInstance().m_pLogger->info(info);
	}
	void FileLogger::Debug(const char* info)
	{
		GetInstance().m_pLogger->debug(info);
	}
	void FileLogger::Error(const char* error)
	{
		GetInstance().m_pLogger->error(error);
	}
	void FileLogger::Flush()
	{
		GetInstance().m_pLogger->flush();
	}
}
