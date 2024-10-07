#pragma once
#include "SpotLight.h"
#include <d3d11.h>
#include "BufferManager\ConstBuffer.h"
#include "Logger\FileLogger.h"

namespace yoi
{
	class LightManager
	{
	private:
		ConstBuffer* m_pDirLightBuffer;
		ConstBuffer* m_pPointLightBuffer;
		ConstBuffer* m_pSpotLightBuffer;

		std::vector<DirLight*> m_DirLights;
		std::vector<PointLight*> m_PointLights;
		std::vector<SpotLight*> m_SpotLights;
	public:
		LightManager(ConstBuffer* pDirLightBuffer, ConstBuffer* pPointLightBuffer, ConstBuffer* pSpotLight);
		LightManager(const LightManager&) = delete;
		LightManager& operator = (const LightManager&) = delete;
		~LightManager();

		void Flush();
		void UpdateConstantBuffer();
		void Bind(ID3D11DeviceContext* pContext);

		template<typename T>
		Light* AddLight(const T& light)
		{
			FileLogger::Error("Unknown light type.");
			FileLogger::Flush();
			return nullptr;
		}

		template<>
		Light* AddLight(const DirLight& light)
		{
			if (m_DirLights.size() >= LIGHT_MAX_COUNT)
			{
				FileLogger::Error("Num of Direction Light has been Max.");
				FileLogger::Flush();
				return nullptr;
			}
			DirLight* newLight = new DirLight(light);
			m_DirLights.emplace_back(newLight);
			return dynamic_cast<Light*>(newLight);
		}

		template<>
		Light* AddLight(const PointLight& light)
		{
			if (m_PointLights.size() >= LIGHT_MAX_COUNT)
			{
				FileLogger::Error("Num of Point Light has been Max.");
				FileLogger::Flush();
				return nullptr;
			}
			PointLight* newLight = new PointLight(light);
			m_PointLights.emplace_back(newLight);
			return dynamic_cast<Light*>(newLight);
		}

		template<>
		Light* AddLight(const SpotLight& light)
		{
			if (m_SpotLights.size() >= LIGHT_MAX_COUNT)
			{
				FileLogger::Error("Num of Spot Light has been max.");
				FileLogger::Flush();
				return nullptr;
			}
			SpotLight* newLight = new SpotLight(light);
			m_SpotLights.emplace_back(newLight);
			return dynamic_cast<Light*>(newLight);
		}

		template<typename T>
		bool RemoveLight(const T* light)
		{
			FileLogger::Error("Unknown light type.");
			FileLogger::Flush();
			return false;
		}

		template<>
		bool RemoveLight(const DirLight* light)
		{
			auto iter = m_DirLights.begin();
			for (; iter != m_DirLights.end(); iter++)
			{
				if (*iter == light)
				{
					m_DirLights.erase(iter);
					return true;
				}
			}
			return false;
		}

		template<>
		bool RemoveLight(const PointLight* light)
		{
			auto iter = m_PointLights.begin();
			for (; iter != m_PointLights.end(); iter++)
			{
				if (*iter == light)
				{
					m_PointLights.erase(iter);
					return true;
				}
			}
			return false;
		}

		template<>
		bool RemoveLight(const SpotLight* light)
		{
			auto iter = m_SpotLights.begin();
			for (; iter != m_SpotLights.end(); iter++)
			{
				if (*iter == light)
				{
					m_SpotLights.erase(iter);
					return true;
				}
			}
			return false;
		}
	};
}