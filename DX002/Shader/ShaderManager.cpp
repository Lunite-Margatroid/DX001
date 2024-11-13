#include "pch.h"
#include "ShaderManager.h"
#include "P3_C3.h"
#include "P3_T2.h"
#include "P3_T2_Materialed.h"
#include "Logger\FileLogger.h"
#include "P3_N3_T2_Lighted.h"
#include "P3_N3_T2.h"
#include "P2_T2_2D.h"
#include "P3_N3_T2_Wave.h"
#include "P3_N3_T2_ChessBoard.h"
#include "P3_N3_T2_ClothSim.hpp"
#include "P3_Skybox.hpp"

namespace yoi
{
	ShaderManager::ShaderManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	{
		FileLogger::Info("Shaders loading...");
		RegisterShader<P3_C3>(pDevice, pContext);
		RegisterShader<P3_T2>(pDevice, pContext);
		RegisterShader<P3_T2_Materialed>(pDevice, pContext);
		RegisterShader<P3_N3_T2_Lighted>(pDevice, pContext);
		RegisterShader<P3_N3_T2>(pDevice, pContext);
		RegisterShader<P2_T2_2D>(pDevice, pContext);
		RegisterShader<P3_N3_T2_Wave>(pDevice, pContext);
		RegisterShader<P3_N3_T2_ChessBoard>(pDevice, pContext);
		RegisterShader<P3_N3_T2_ClothSim>(pDevice, pContext);
		RegisterShader<P3_Skybox>(pDevice, pContext);

		RegisterComputeShader(pDevice, "./shader-bin/WaterWaveCS.cso", "Water Wave Update");
		RegisterComputeShader(pDevice, "./shader-bin/WaterWaveDisturbCS.cso", "Water Wave Disturb");
		RegisterComputeShader(pDevice, "./shader-bin/WaterWaveCalmCS.cso", "Water Wave Calm");

	}
	ShaderManager::~ShaderManager()
	{
		for (auto& iter : m_Shaders)
		{
			delete iter.second;
		}

		for (auto& shader : m_ComputeShaders)
		{
			delete shader.second;
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
	CShader* ShaderManager::GetComputeShader(const std::string& shaderTitle)
	{
		if (m_ComputeShaders.find(shaderTitle) == m_ComputeShaders.end())
		{
			std::ostringstream oss;
			oss << "Compute Shader " << '\"' << shaderTitle << "\" does not exist.";
			FileLogger::Warn(oss.str().c_str());
			return nullptr;
		}
		else
		{
			return m_ComputeShaders[shaderTitle];
		}
		return nullptr;
	}
	void ShaderManager::RegisterComputeShader(ID3D11Device* pDevice, const std::string& path, const std::string& title)
	{
		if (m_ComputeShaders.find(title) != m_ComputeShaders.end())
		{
			std::ostringstream oss;
			oss << "Compute Shader with title " << '\"' << title << "\" has been loaded.";
			FileLogger::Error(oss.str().c_str());
		}
		else
		{
			m_ComputeShaders[title] = new CShader(pDevice, path);
		}
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