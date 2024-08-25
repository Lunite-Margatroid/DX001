#pragma once

#define GFX_EXCEPT_NOINFO(hr) yoi::Graphics::HrException(__LINE__, __FILE__, (hr) )
#define GFX_THROW_NOINFO(hrcall) if(FAILED(hr = (hrcall))) throw yoi::Graphics::HrException(__LINE__, __FILE__, hr)

#ifdef YOI_DEBUG
#define GFX_EXCEPT_SUPPORT() yoi::DxgiInfoManager& infoManager = yoi::Graphics::GetInstance().GetInfoManager();HRESULT hr
#else
#define GFX_EXCEPT_SUPPORT() HRESULT hr
#endif // YOI_DEBUG

#ifdef YOI_DEBUG
#define GFX_EXCEPT(hr) yoi::Graphics::HrException(__LINE__, __FILE__, (hr), infoManager.GetMessages())
#define GFX_THROW_INFO(hrcall) infoManager.Set(); if(FAILED(hr = (hrcall))) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) yoi::Graphics::DeviceRemovedException(__LINE__, __FILE__, (hr), infoManager.GetMessages())
#define GFX_THROW_INFO_ONLY(call) infoManager.Set(); (call); \
		{auto v = infoManager.GetMessages();if(!v.empty()) {throw yoi::Graphics::InfoException(__LINE__, __FILE__, v);} }
#else
#define GFX_EXCEPT(hr) yoi::Graphics::HrException(__LINE__, __FILE__, (hr))
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) yoi::Graphics::DeviceRemovedException(__LINE__, __FILE__, (hr))
#define GFX_THROW_INFO_ONLY(call) (call)
#endif // YOI_DEBUG

#define GFX_THROW_FAILED(hrcall) if(FAILED(hr = (hrcall))) throw yoi::Graphics::HrException(__LINE__, __FILE__, hr)