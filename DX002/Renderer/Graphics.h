#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "MyWin.h"
#include "Window/YoiException.h"
#include "InfoManager/DxgiInfoManager.h"
#include "Timer/YoiTimer.h"
#include <D3D11.h>
#include <wrl.h>
#include "ImGuiManager/ImGuiManager.h"
#include "Sprite\SpriteV1Cube.h"
#include "SceneObject\PerspectiveCamera.h"
#include "ImWindow\ObjectPropertyWin.h"
#include "Shader\ShaderManager.h"
#include "BufferManager\BufferManager.h"
#include "Texture\Texture.h"
#include "Texture\Sampler.h"
#include "Material\MaterialManager.h"
#include "Texture/TextureManager.h"
#include "Light\LightManager.h"
#include "Sprite\SpriteManager.h"

#include "CShader\CShader.h"


namespace yoi
{
	class Graphics
	{
		friend class Sprite;
	public:
		class Exception : public YoiException
		{
			using YoiException::YoiException;
		};

		class HrException : public Exception
		{
		public:
			HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
			const char* what() const noexcept override;
			const char* GetType() const noexcept override;
			HRESULT GetErrorCode() const noexcept;
			std::string GetErrorString() const noexcept;
			std::string GetErrorDescription() const noexcept;
			std::string GetErrorInfo() const noexcept;

			static std::string TranslateErrorCode(HRESULT hr) noexcept;
		private:
			HRESULT hr;
			std::string info;
		};

		class InfoException : public Exception
		{
		public:
			InfoException(int line, const char* file,std::vector<std::string> infoMsgs) noexcept;
			const char* what() const noexcept override;
			const char* GetType() const noexcept override;
			std::string GetErrorInfo() const noexcept;
		private:
			std::string info;
		};

		class DeviceRemovedException : public HrException
		{
			using HrException::HrException;
		public:
			const char* GetType() const noexcept override;
			const char* what() const noexcept override;
		private:
			std::string reason;
		};

	public:
		Graphics(HWND hWnd);
		Graphics(const Graphics&) = delete;
		Graphics& operator=(const Graphics&) = delete;
		virtual ~Graphics() = default;
		
		void ImGuiFrame();
		void EndFrame();

		void ClearBuffer(float red, float green, float blue);

	/****** test draw ******/
	protected:
		
		std::unique_ptr<SceneObj> m_RootObj;
		CameraObj* m_MainCamera;

		YoiTimer timer;

		std::unique_ptr<ObjectPropertyWin> m_pObjWin;

		std::unique_ptr<Texture> m_pTexture;
		std::unique_ptr<Sampler> m_pSampler;
		std::unique_ptr<Sampler> m_pSampler1;
		float m_DeltaTime;
	public:
		void InitTestDraw();
		void DrawTriangle();

		CameraObj* GetMainCamera();
	/****** test draw * end */


	protected:
		Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
		Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthStencil;
		/*
		* ʹ��&���������ȡ����ComPtr����Դ�ĵ�ַ�����ڲ�ָ��ĵ�ַ��
		* ʹ��&�������ComPtr����Release���ٷ���ָ��ĵ�ַ������ζ��ʹ��&��ԭ�ȵ�ʵ����ʧЧ��
		* ʹ��GetAddressOf()��ȡָ��ĵ�ַ�������ͷ��Ѿ�ʹ�õ��ڴ档
		* ʹ��Get()��ȡָ�롣
		* ʹ��ReleaseAndGetAddressOf()��&�������ͬ��
		*/
	public:
		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetContext();

#ifdef  YOI_DEBUG
	protected:
		DxgiInfoManager infoManager;
	public:
		DxgiInfoManager& GetInfoManager();
#endif //  YOI_DEBUG

		/**** ImGui ****/
		protected:
			std::unique_ptr<ImGuiManager> pImGuiManager;

		/****** Singleton ******/
		// only single window accepted
		private:
			static Graphics* s_pInstance;
		public:
			static Graphics& GetInstance();

		/******* Shader **********/
		protected:
			std::unique_ptr<ShaderManager> m_pShaderManager;
		public:
			Shader* GetShader(const std::string& shaderTitle);

		/********* Buffer Manager ***********/
		protected:
			std::unique_ptr<BufferManager> m_pBufferManager;
		public:
			static ID3D11Buffer* GetBuffer(BufferManager::Buffer buffer);
			static ID3D11Buffer* SetBufferData(BufferManager::Buffer buffer, unsigned int size, unsigned int offset, void* src);

		/*********** Material Manager *************/
		protected:
			std::unique_ptr<MaterialManager> m_pMaterialManager;

		/*********** Texture Mananger *************/
		protected:
			std::unique_ptr<TextureManager> m_pTextureManager;

		/*********** Light Manager ****************/
		protected:
			std::unique_ptr<LightManager> m_pLightManager;
		/*********** Sprite Manager ***************/
		protected:
			std::unique_ptr<SpriteManager> m_pSpriteManager;

		/*********** Load Model ***************/
		public:
			SceneObj* LoadModel(const std::string& path);
		private:
			unsigned int LoadMaterials(const aiScene * scene, const std::string & dictionary);
			unsigned int LoadMeshes(const aiScene* scene, unsigned int materialBaseInd, std::vector<Mesh> &meshes);
			Mesh LoadMesh(const aiMesh* mesh);
			SceneObj* ProcessNode(const aiNode* node, const std::vector<Mesh>& meshes);

			std::map<std::wstring, unsigned int> m_LoadedTex;

	};

}