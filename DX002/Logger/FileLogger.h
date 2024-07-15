#pragma once
#include "spdlog/sinks/basic_file_sink.h"

namespace yoi
{
    class FileLogger
    {
    private:
        std::shared_ptr<spdlog::logger> m_pLogger;
        static FileLogger s_Instance;
    public:
        FileLogger() noexcept;
        ~FileLogger() = default;
        FileLogger(const FileLogger&) = delete;
        FileLogger& operator=(const FileLogger&) = delete;

        static FileLogger& GetInstance();
        static void SetLogFile(const char* file);

        static void Trace(const char* trace);
        static void Info(const char* info);
        static void Warn(const char* warn);
        static void Debug(const char* info);
        static void Error(const char* error);
        static void Flush();
    };
}