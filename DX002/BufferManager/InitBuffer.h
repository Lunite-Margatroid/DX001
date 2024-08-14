#pragma once

namespace yoi
{
	template<BufferManager::Buffer buffer>
	void InitBuffer(yoi::BufferManager* bufMng)
	{
		FileLogger::Warn("Unknown Buffer Type");
		FileLogger::Flush();
	}
}