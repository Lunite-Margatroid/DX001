#include "pch.h"
#include "DxgiInfoManager.h"
#include "Window/Window.h"
#include "Log\FileLogger.h"

#pragma comment(lib, "dxguid.lib")

#define GFX_THROW_NOINFO(hrcall) if(FAILED(hr = (hrcall))) throw Graphics::HrException(__LINE__, __FILE__, hr)


namespace yoi
{
	DxgiInfoManager::DxgiInfoManager()
		:
		pDxgiInfoQueue(nullptr)
	{
		typedef HRESULT (WINAPI* DXGIGetDebugInterface)(REFIID, void**);

		const auto hModDxgiDebug =
			LoadLibraryExA("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);

		if (hModDxgiDebug == nullptr)
		{
			throw YOIWND_LAST_EXCEPT();
		}

		const auto DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(
			reinterpret_cast<void*>(GetProcAddress(hModDxgiDebug, "DXGIGetDebugInterface"))
			);

		if (DxgiGetDebugInterface == nullptr)
		{
			throw YOIWND_LAST_EXCEPT();
		}


		HRESULT hr;
		GFX_THROW_NOINFO(DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), (void**)&pDxgiInfoQueue));
	}

	DxgiInfoManager::~DxgiInfoManager()
	{
		pDxgiInfoQueue->Release();
	}

	void DxgiInfoManager::Set() noexcept
	{
		next = pDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	}

	std::vector<std::string> DxgiInfoManager::GetMessages()const
	{
		std::vector<std::string> messages;
		const auto end = pDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
		for (auto i = next; i < end; i++)
		{
			HRESULT hr;
			SIZE_T messageLenght;

			GFX_THROW_NOINFO(pDxgiInfoQueue->GetMessageA(DXGI_DEBUG_ALL, i, nullptr, &messageLenght));

			auto bytes = std::make_unique<byte[]>(messageLenght);
			auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());

			GFX_THROW_NOINFO(pDxgiInfoQueue->GetMessageA(DXGI_DEBUG_ALL, i, pMessage, &messageLenght));
			messages.emplace_back(pMessage->pDescription);
		}
		return messages;
	}
}