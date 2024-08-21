#include "pch.h"
#include "LightManager.h"
#include "Renderer\Graphics.h"
#include "Renderer\GFXMacro.h"

namespace yoi
{
	LightManager::LightManager(ID3D11Buffer* pDirLightBuffer, ID3D11Buffer* pPointLightBuffer, ID3D11Buffer* pSpotLight)
		:m_pDirLightBuffer(pDirLightBuffer),
		m_pPointLightBuffer(pPointLightBuffer),
		m_pSpotLightBuffer(pSpotLight)
	{
		
	}
	LightManager::~LightManager()
	{
		for (DirLight* light : m_DirLights)
		{
			delete light;
		}
		for (PointLight* light : m_PointLights)
		{
			delete light;
		}
		for (SpotLight* light : m_SpotLights)
		{
			delete light;
		}
	}
	void LightManager::Flush()
	{
		UpdateConstantBuffer();
	}
	void LightManager::UpdateConstantBuffer()
	{
		GFX_EXCEPT_SUPPORT();
		ID3D11DeviceContext *pContext = Graphics::GetInstance().GetContext();
		D3D11_MAPPED_SUBRESOURCE dataMap;

		size_t offset = 16;
		int light_num;

		GFX_THROW_INFO(pContext->Map(m_pDirLightBuffer, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &dataMap));
		light_num = static_cast<int>(m_DirLights.size());
		memcpy(dataMap.pData, &light_num, sizeof(int));
		for (DirLight* light : m_DirLights)
		{
			offset = light->WriteToBuffer(dataMap.pData, offset);
		}
		GFX_THROW_INFO_ONLY(pContext->Unmap(m_pDirLightBuffer, 0u));
		

		offset = 16;
		light_num = static_cast<int>(m_PointLights.size());
		GFX_THROW_INFO(pContext->Map(m_pPointLightBuffer, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &dataMap));
		memcpy(dataMap.pData, &light_num, sizeof(int));
		for (PointLight* light : m_PointLights)
		{
			offset = light->WriteToBuffer(dataMap.pData, offset);
		}
		GFX_THROW_INFO_ONLY(pContext->Unmap(m_pPointLightBuffer, 0u));

		offset = 16;
		light_num = static_cast<int>(m_SpotLights.size());
		GFX_THROW_INFO(pContext->Map(m_pSpotLightBuffer, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &dataMap));
		memcpy(dataMap.pData, &light_num, sizeof(int));
		for (SpotLight* light : m_SpotLights)
		{
			offset = light->WriteToBuffer(dataMap.pData, offset);
		}
		GFX_THROW_INFO_ONLY(pContext->Unmap(m_pSpotLightBuffer, 0u));
	}
	void LightManager::Bind(ID3D11DeviceContext* pContext)
	{
		GFX_EXCEPT_SUPPORT();
		GFX_THROW_INFO_ONLY(pContext->PSSetConstantBuffers(2, 3, &m_pDirLightBuffer));
	}
}