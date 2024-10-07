#pragma once

namespace yoi
{
	template<BufferManager::Buffers buffer>
	void InitBuffer(yoi::BufferManager* bufMng)
	{
		FileLogger::Warn("Unknown Buffer Type");
		FileLogger::Flush();
	}
}