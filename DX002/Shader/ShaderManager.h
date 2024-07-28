#pragma once
#include "Shader.h"
#include "Window\YoiException.h"
#include "Logger\FileLogger.h"


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
	public:
		ShaderManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
		~ShaderManager();
		ShaderManager(const ShaderManager&) = delete;
		ShaderManager& operator = (const ShaderManager&) = delete;

		const std::map<std::string, Shader*>& GetShaderMap();
		Shader* GetShader(const std::string& shaderTitle);

		template<typename TShader>
		void RegisterShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
		{
			TShader* t = new TShader(pDevice, pContext);
			Shader* shader = dynamic_cast<Shader*>(t);
			if (shader == nullptr)
			{
				delete t;
				const char errorInfo[] = "Template class is not shader.";
				FileLogger::Error(errorInfo);
				FileLogger::Flush();
				throw ShaderException(__LINE__, __FILE__, errorInfo);
				return;
			}
			std::string shaderTitle = shader->GetTitle();
			if (m_Shaders.find(std::string(shaderTitle)) == m_Shaders.end())
			{
				m_Shaders[std::string(shader->GetTitle())] = shader;
			}
			else
			{
				const char errorInfo[] = "Same shader being loaded.";
				FileLogger::Error(errorInfo);
				FileLogger::Flush();
				throw ShaderException(__LINE__, __FILE__, errorInfo);
			}
			
		}
	};
}