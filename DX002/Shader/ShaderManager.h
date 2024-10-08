#pragma once
#include "Shader.h"
#include "Window\YoiException.h"
#include "Logger\FileLogger.h"
#include "CShader\CShader.h"

namespace yoi
{
	class ShaderManager
	{
	private:
		class ShaderException : public YoiException
		{
		private:
			std::string m_Info;
		public:
			ShaderException(int line, const char* file, const std::string& info) noexcept;
			const char* what() const noexcept override;
			const char* GetType() const noexcept override;
		};

	private:
		std::map<std::string, Shader*> m_Shaders;

		std::map<std::string, CShader*> m_ComputeShaders;
	public:
		ShaderManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
		~ShaderManager();
		ShaderManager(const ShaderManager&) = delete;
		ShaderManager& operator = (const ShaderManager&) = delete;

		const std::map<std::string, Shader*>& GetShaderMap();
		Shader* GetShader(const std::string& shaderTitle);
		CShader* GetComputeShader(const std::string& shaderTitle);


		template<typename TShader>
		void RegisterShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
		{
			TShader* tShader = new TShader(pDevice, pContext);
			Shader* shader = dynamic_cast<Shader*>(tShader);
			if (shader == nullptr)
			{
				delete tShader;
				const char errorInfo[] = "Template class is not shader.";
				FileLogger::Error(errorInfo);
				FileLogger::Flush();
				throw ShaderException(__LINE__, __FILE__, errorInfo);
				return;
			}
			std::string shaderTitle = tShader->GetTitle();
			if (m_Shaders.find(shaderTitle) == m_Shaders.end())
			{
				m_Shaders[shaderTitle] = tShader;
			}
			else
			{
				const char errorInfo[] = "Same shader being loaded.";
				FileLogger::Error(errorInfo);
				FileLogger::Flush();
				throw ShaderException(__LINE__, __FILE__, errorInfo);
			}
			
		}

		void RegisterComputeShader(ID3D11Device* pDevice, const std::string& path, const std::string& title);
	};
}