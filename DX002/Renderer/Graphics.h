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
#include "ImWindow\ObjectPropertyWin.h"

#include "SceneObject\PerspectiveCamera.h"
#include "SceneObject\Camera2DObj.h"

#include "BufferManager\BufferManager.h"

#include "Material\MaterialManager.h"

#include "Texture/TextureManager.h"
#include "Texture\Sampler.h"
#include "Texture\Texture.h"
#include "Texture\OSRTexture.h"
#include "Texture\DSTexture.h"

#include "Light\LightManager.h"

#include "Sprite\SpriteManager.h"
#include "Sprite\SpriteV1Cube.h"

#include "Shader\ShaderManager.h"
#include "CShader\CShader.h"

#include "Pipeline.h"
#include "Screen.h"

namespace yoi
{
	class LightManager;

	class Graphics
	{
		friend class Sprite;
	/************* Excpetion *******************/
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
	/************* Excpetion ***************** end */

	public:
		Graphics(HWND hWnd, unsigned int width = YOI_WINDOW_WIDTH, unsigned int height = YOI_WINDOW_HEIGHT);
		Graphics(const Graphics&) = delete;
		Graphics& operator=(const Graphics&) = delete;
		virtual ~Graphics();
		
		void ImGuiFrame();
		void EndFrame();

		void ClearBuffer(float red, float green, float blue);
		void ClearBuffer();

	/****** test draw ******/
	protected:
		
		std::unique_ptr<SceneObj> m_RootObj;
		CameraObj* m_MainCamera;
		float m_ClearColor[4];

		YoiTimer timer;

		std::unique_ptr<ObjectPropertyWin> m_pObjWin;

		std::unique_ptr<Texture> m_pTexture;
		std::unique_ptr<Sampler> m_pSampler;
		std::unique_ptr<Sampler> m_pSampler1;
		float m_DeltaTime;

		// pipeline test
	public:
		void InitTestDraw();
		void DrawTriangle();

		void Update();
		

		CameraObj* GetMainCamera();
	/****** test draw * end */

	/******** pipeline test ***********/
	private:
		// pipeline to render main scene
		std::unique_ptr<Pipeline> m_pPipeline;
		// off-screen render target and the render target for main scene.
		std::unique_ptr<OSRTexture> m_pScreenTex;
		

		// render the off-screen texture on screen
		std::unique_ptr<Screen> m_pPipelineScreen;
		// the scene to show off-screen texture
		std::unique_ptr<SceneObj> m_pScreenScene;
		// the camera to show off-screen texture
		Camera2DObj* m_pCamera2D;
		// depthstencil texture for screen render
		std::unique_ptr<DSTexture> m_pDepthStencilTex;
	public:
		void InitPipeline();
		void RunPipeline();
	/******** pipeline test **** end ***/

	/*********** Context ********************/
	protected:
		Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
		Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthStencil;
		Microsoft::WRL::ComPtr<ID3D11BlendState> pBlendState;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> pRasterizerState;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDSState;
		/*
		* 使用&运算符，获取的是ComPtr的资源的地址，即内部指针的地址。
		* 使用&运算符，ComPtr会先Release，再返回指针的地址。这意味着使用&后，原先的实例会失效。
		* 使用GetAddressOf()获取指针的地址，不会释放已经使用的内存。
		* 使用Get()获取指针。
		* 使用ReleaseAndGetAddressOf()和&运算符相同。
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
			static Buffer* GetBuffer(BufferManager::Buffers buffer);
			static IndexBuffer* GetIndexBuffer(BufferManager::Buffers buffer);
			static VertexBuffer* GetVertexBuffer(BufferManager::Buffers buffer);
			static ConstBuffer* GetConstBuffer(BufferManager::Buffers buffer);
			static Buffer* SetBufferData(BufferManager::Buffers buffer, unsigned int size, unsigned int offset, void* src);

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