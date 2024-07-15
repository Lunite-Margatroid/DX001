#include "pch.h"
#include "ShaderManager.h"
#include "P3_C3.h"
#include "P3_T2.h"
#include "Logger\FileLogger.h"
namespace yoi
{
	ShaderManager::ShaderManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	{
		FileLogger::Info("Shaders loading...");
		RegisterShader<P3_C3>(pDevice, pContext);
		RegisterShader<P3_T2>(pDevice, pContext);
	}
	ShaderManager::~ShaderManager()
	{
		for (auto& iter : m_Shaders)
		{
			delete iter.second;
		}
	}
	const std::map<std::string, Shader*>& ShaderManager::GetShaderMap()
	{
		return m_Shaders;
	}
	Shader* ShaderManager::GetShader(const std::string& shaderTitle)
	{
		auto iter = m_Shaders.find(shaderTitle);
		if (iter == m_Shaders.end())
		{
			return nullptr;
		}
		return (*iter).second;
	}
	ShaderManager::ShaderException::ShaderException(int line, const char* file, const std::string& info)noexcept
		:YoiException(line, file), m_Info(info)
	{}

	const char* ShaderManager::ShaderException::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< m_Info << std::endl
			<< GetOriginString();
		whatBuffer = std::move(oss.str());
		return whatBuffer.c_str();
	}
	const char* ShaderManager::ShaderException::GetType() const noexcept
	{
		return "Shader Exception";
	}
}