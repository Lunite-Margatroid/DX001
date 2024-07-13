#pragma once
#include "MyWin.h"
#include <wrl.h>
#include <dxgidebug.h>
namespace yoi
{
	class DxgiInfoManager
	{
	public:
		DxgiInfoManager();
		~DxgiInfoManager();
		DxgiInfoManager(const DxgiInfoManager&) = delete;
		DxgiInfoManager& operator = (const DxgiInfoManager&) = delete;
		void Set() noexcept;
	public:
		std::vector<std::string> GetMessages() const;
	private:
		unsigned long long next = 0u;
		IDXGIInfoQueue* pDxgiInfoQueue;
	};
}