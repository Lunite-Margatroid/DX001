#include "pch.h"
#include <d3dcompiler.h>
#include "Graphics.h"
#include "GFXMacro.h"
#include "Logger\FileLogger.h"
#include "Shader\P3_C3.h"
#include "Sprite\ColoredCube.h"
#include "Sprite\TexturedCube.h"
#include "Application.h"
#include "PipelineV1.h"



#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

#ifdef max
#undef max
#endif

namespace yoi
{
	/****** singleton ******/
	Graphics* Graphics::s_pInstance = nullptr;
	/*------------------ HrException -----------------------*/
	Graphics::HrException::HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs) noexcept
		:Exception(line, file), hr(hr)
	{
		for (const auto& m : infoMsgs)
		{
			info += m;
			info.push_back('\n');
		}
		if (!info.empty())
		{
			info.pop_back();
		}
	}

	const char* Graphics::HrException::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< "line: " << GetLine() << "\nfile: " << GetFile() << std::endl
			<< "[Error Code] " << GetErrorCode() << std::endl
			<< "[Error String] " << GetErrorString() << std::endl
			<< "[Description] " << GetErrorDescription() << std::endl
			<< "[info] " << GetErrorInfo()
			<< GetOriginString();
		whatBuffer = oss.str();
		FileLogger::Error(whatBuffer.c_str());
		FileLogger::Flush();
		return whatBuffer.c_str();
	}

	const char* Graphics::HrException::GetType() const noexcept
	{
		return "Graphics HRESULT Error";
	}

	HRESULT Graphics::HrException::GetErrorCode() const noexcept
	{
		return hr;
	}

	std::string Graphics::HrException::GetErrorString() const noexcept
	{
		return TranslateErrorCode(hr);
	}

	std::string Graphics::HrException::TranslateErrorCode(HRESULT hr) noexcept
	{
		char* pMsgBuf = nullptr;
		DWORD nMsgLen = FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr,
			hr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			reinterpret_cast<LPSTR>(&pMsgBuf),
			0,
			nullptr);
		if (nMsgLen == 0)
		{
			return "Unidentified error code";
		}
		std::string errorString = pMsgBuf;
		LocalFree(pMsgBuf);
		return errorString;
	}

	std::string Graphics::HrException::GetErrorDescription() const noexcept
	{
		return std::string("");
	}

	std::string Graphics::HrException::GetErrorInfo() const noexcept
	{
		return info;
	}

	/*------------------ HrException ---------------- * end */

	/* ---------------- DeviceRemovedException -------------------*/
	const char* Graphics::DeviceRemovedException::GetType() const noexcept
	{
		return "Device Removed Exception";
	}

	const char* Graphics::DeviceRemovedException::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< "[Error Code] " << GetErrorCode() << std::endl
			<< "[Error String] " << GetErrorString() << std::endl
			<< "[Description] " << GetErrorDescription() << std::endl
			<< "[info] " << GetErrorInfo()
			<< "[reason] " << reason
			<< GetOriginString();
		whatBuffer = oss.str();
		FileLogger::Error(whatBuffer.c_str());
		FileLogger::Flush();
		return whatBuffer.c_str();
	}
	/* ---------------- DeviceRemovedException ---------------- * end */

	/******************* InfoException ********************/
	Graphics::InfoException::InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept
		:Exception(line, file)
	{
		for (const auto& m : infoMsgs)
		{
			info += m;
			info.push_back('\n');
		}
		if (!info.empty())
		{
			info.pop_back();
		}
	}

	const char* Graphics::InfoException::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< "\n[Error Info]\n" 
			<<"line: " << GetLine() << "\nfile: " << GetFile()<< std::endl
			<<GetErrorInfo() << std::endl << std::endl;
		oss << GetOriginString();
		whatBuffer = oss.str();
		FileLogger::Error(whatBuffer.c_str());
		FileLogger::Flush();
		return whatBuffer.c_str();
	}
	const char* Graphics::InfoException::GetType() const noexcept
	{
		return "Graphics Info";
	}

	std::string Graphics::InfoException::GetErrorInfo() const noexcept
	{
		return info;
	}
	/******************* InfoException ************** end */

	Graphics::Graphics(HWND hWnd, unsigned int width, unsigned int height)
	{
		DXGI_SWAP_CHAIN_DESC scd = {};

		scd.BufferDesc.Width = width;
		scd.BufferDesc.Height = height;
		scd.BufferDesc.RefreshRate.Numerator = 0u;
		scd.BufferDesc.RefreshRate.Denominator = 0u;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		scd.SampleDesc.Count = 1;	// multisample count
		scd.SampleDesc.Quality = 0;	// quality level

		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.BufferCount = 1;		// num of buffer in swap chain
		scd.OutputWindow = hWnd;	// window of output
		scd.Windowed = true;
		scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		scd.Flags = 0;

		UINT swapCreateFlags = 0u;
#ifdef YOI_DEBUG
		swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		HRESULT hr; // 在macro中使用
		GFX_THROW_FAILED(
			D3D11CreateDeviceAndSwapChain(
				// 1* video adapter
				nullptr,
				// 2* Driver Type
				D3D_DRIVER_TYPE_HARDWARE,	// primary driver
				// 3* a handle to DLL
				// for software
				nullptr,
				// 4* flags
				swapCreateFlags,
				// 5* d3d feature levels
				nullptr,
				// 6* nums of d3d feature levels
				0,
				// 7* SDK Version
				D3D11_SDK_VERSION,
				// 8* swap chain description
				&scd,
				// 9* swap chain ptr
				&pSwap,
				// 10* device ptr
				&pDevice,
				// 11* out
				// return a ptr to D3D_FEATURE_LEVEL
				nullptr,
				// 12* context ptr
				&pContext
			));

		Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;
		GFX_THROW_FAILED(pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
		GFX_THROW_FAILED(pDevice->CreateRenderTargetView(
			pBackBuffer.Get(),
			nullptr,
			&pTarget));

		// create depthstencil view
		// create a 2d texture for depthstencil
		D3D11_TEXTURE2D_DESC  td = {};
		td.Width = width;
		td.Height = height;
		td.MipLevels = 1u;
		td.ArraySize = 1u;
		td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		td.SampleDesc.Count = 1u;
		td.SampleDesc.Quality = 0u;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		td.CPUAccessFlags = 0u;
		td.MiscFlags = 0u;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencil;

		GFX_THROW_INFO(pDevice->CreateTexture2D(&td, nullptr, &depthStencil));

		// create
		GFX_THROW_INFO(pDevice->CreateDepthStencilView(depthStencil.Get(), nullptr, &pDepthStencil));


		// RasterizerState
		D3D11_RASTERIZER_DESC stt = {};
		stt.FillMode = D3D11_FILL_SOLID;
		// stt.FillMode = D3D11_FILL_WIREFRAME;
		stt.CullMode = D3D11_CULL_NONE;
		stt.FrontCounterClockwise = false;
		stt.DepthBias = 0;
		stt.DepthBiasClamp = 0.0f;
		stt.ScissorEnable = false;
		stt.DepthClipEnable = true;
		stt.ScissorEnable = false;
		stt.MultisampleEnable = false;
		stt.AntialiasedLineEnable = false;

		// Microsoft::WRL::ComPtr<ID3D11RasterizerState> pRasterizerState;
		GFX_THROW_INFO(pDevice->CreateRasterizerState(&stt, &pRasterizerState));
		GFX_THROW_INFO_ONLY(pContext->RSSetState(pRasterizerState.Get()));

		// depth and stencil test
		D3D11_DEPTH_STENCIL_DESC dsd = {};
		dsd.DepthEnable = true;
		dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsd.DepthFunc = D3D11_COMPARISON_LESS;
		dsd.StencilEnable = false;
		dsd.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK; 
		dsd.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		dsd.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		dsd.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		dsd.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsd.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsd.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsd.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsd.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsd.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;

		// Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDSState;

		GFX_THROW_INFO(pDevice->CreateDepthStencilState(&dsd, &pDSState));
		GFX_THROW_INFO_ONLY(pContext->OMSetDepthStencilState(pDSState.Get(), 0));

		// set viewprot
		D3D11_VIEWPORT vp = {};
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		vp.Width = width;
		vp.Height = height;
		vp.MinDepth = 0;
		vp.MaxDepth = 1;
		GFX_THROW_INFO_ONLY(pContext->RSSetViewports(1, &vp));
		// set render target
		GFX_THROW_INFO_ONLY(pContext->OMSetRenderTargets(1, pTarget.GetAddressOf(), nullptr));


		// create blend state
		D3D11_BLEND_DESC bd = {};
		bd.AlphaToCoverageEnable = false;
		bd.IndependentBlendEnable = false;
		bd.RenderTarget[0].BlendEnable = true;
		bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; 
		bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].RenderTargetWriteMask = static_cast<unsigned char>(0xfu);

		GFX_THROW_INFO(pDevice->CreateBlendState(&bd, &pBlendState));
		GFX_THROW_INFO_ONLY(pContext->OMSetBlendState(pBlendState.Get(), nullptr, 0xffffffffu));

		// singleton
		s_pInstance = this;

		// Init shader manager
		m_pShaderManager = std::make_unique<ShaderManager>(pDevice.Get(), pContext.Get());

		// buffer manager
		m_pBufferManager = std::make_unique<BufferManager>(pDevice.Get());

		// init texture manager
		m_pTextureManager = std::make_unique<TextureManager>();

		// init material manager
		m_pMaterialManager = std::make_unique<MaterialManager>();

		// init light manager
		m_pLightManager = std::make_unique<LightManager>(
			m_pBufferManager->GetConstBuffer(BufferManager::Buffers::Constant_DirLight),
			m_pBufferManager->GetConstBuffer(BufferManager::Buffers::Constant_PointLight),
			m_pBufferManager->GetConstBuffer(BufferManager::Buffers::Constant_SpotLight)
		);

		// init sprite manager
		m_pSpriteManager = std::make_unique<SpriteManager>();

		
		// Init Scene obj
		m_RootObj = std::make_unique<SceneObj>(nullptr, nullptr, "Root");

		// init test draw 
		// InitTestDraw();
		
		// init ImGui
		pImGuiManager = std::make_unique<ImGuiManager>(hWnd, pDevice.Get(), pContext.Get());

		m_pObjWin = std::make_unique<ObjectPropertyWin>(m_RootObj.get());

		m_DeltaTime = 0.f;

		float clearColor[4] = { 0.224f, 0.773f, 0.733f , 1.0f};
		memcpy(m_ClearColor, clearColor, sizeof(float) * 4);
	}

	Graphics::~Graphics()
	{
		if (m_MainCamera)
			delete m_MainCamera;
	}

	void Graphics::ImGuiFrame()
	{
		pImGuiManager->BeginImGuiFrame();

		ImGui::DockSpaceOverViewport();

		m_pObjWin->ShowWindow();
		m_pMainSceneWin->ShowWindow();
	}

	void Graphics::EndFrame()
	{
		HRESULT hr;
#ifdef YOI_DEBUG
		infoManager.Set();
#endif

		pImGuiManager->EndImGuiFrame();

		if (FAILED(hr = pSwap->Present(1u, 0u)))
		{
			if (hr == DXGI_ERROR_DEVICE_REMOVED)
			{
				throw GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
			}
			else
			{
				GFX_THROW_FAILED(hr);
			}
		}


	}

	void Graphics::ClearBuffer(float red, float green, float blue)
	{
		const float color[] = { red, green, blue, 1.0f };
		pContext->ClearRenderTargetView(pTarget.Get(), color);
	}

	void Graphics::ClearBuffer()
	{
		pContext->ClearRenderTargetView(pTarget.Get(), m_ClearColor);
	}

	/*
	void Graphics::InitTestDraw()
	{
		// HRESULT hr;
		// texture
		ImgRes img("./img/rumia.jpg");
		m_pTexture = std::make_unique<Texture>(img);
		m_pTexture->Bind();

		Texture* texRumia = m_pTextureManager->LoadTexture("./img/rumia.jpg");
		Material* mtlRumia = new Material(texRumia, m_pTextureManager.get());
		m_pMaterialManager->Add(mtlRumia, "Rumia");
		Mesh mshRumia(
			GetBuffer(BufferManager::Buffer::Vertex_Textured_Cube),
			GetBuffer(BufferManager::Buffer::Index_Textured_Cube),
			sizeof(float) * 5,
			0u,
			0u,
			36,
			0u,
			0u,
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			GetShader("Materialed Shader"),
			mtlRumia);

		Mesh lightedCube(
			GetBuffer(BufferManager::Buffer::P3_N3_T2_Cube),
			GetBuffer(BufferManager::Buffer::Index_Textured_Cube),
			sizeof(float) * 8,
			0u,
			0u,
			36,
			0u,
			0u,
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			GetShader("Lighted Shader"),
			mtlRumia);

		// Add light
		m_pLightManager->AddLight(
			PointLight(glm::vec3(1.0f), 0.1f, 0.7f, 0.7f, glm::vec3(3.0f, 4.0f, 0.0f))
		);
		m_pLightManager->AddLight(
			DirLight());
		m_pLightManager->AddLight(
			SpotLight(glm::vec3(1.0f), 0.1f, 0.7f, 0.7f, glm::vec3(-3.0f, 4.0f, 0.0f), 1.0f, 0.14f, 0.07f,
				glm::vec3(1.0f, 0.0f, 0.0f))
		);
		m_pLightManager->AddLight(
			SpotLight(glm::vec3(1.0f), 0.1f, 0.7f, 0.7f, glm::vec3(0.0f, 4.0f, -3.0f), 1.0f, 0.14f, 0.07f,
				glm::vec3(0.0f, 0.0f, 1.0f))
		);

		m_pLightManager->Flush();

		// white material
		Material* whtMaterial = m_pMaterialManager->CreateMaterial(dynamic_cast<Texture*>(m_pTextureManager->GetAt()), dynamic_cast<Texture*>(m_pTextureManager->GetAt()), m_pTextureManager.get(), 64.0f);

		// colored cube
		SpriteV3* cubeSprite = m_pSpriteManager->Sprite(ColoredCube());
		SceneObj* colorCube = new SceneObj(m_RootObj.get(), cubeSprite, "Colored Cube");
		// textured cube
		cubeSprite = m_pSpriteManager->Sprite(lightedCube);
		SceneObj* texturedCube = new SceneObj(m_RootObj.get(), cubeSprite, "Texture Cube");
		texturedCube->SetPosition(glm::vec3(0.0f, 4.0f, 0.0f));

		// gird test
		SpriteV3* gridSprite = m_pSpriteManager->CreateGrid(
			100, 100, m_pShaderManager->GetShader("Lighted Shader"), whtMaterial, *(m_pBufferManager.get()),
			[](float x, float z) { return 0.3f * ( z * sinf(10.f * x) + x * cosf(10.f * z) ); },
			[](float x, float z) { return 
				glm::normalize(
					glm::vec3( 
						-3.f * z * cosf(10.f * x) - 0.3f * cosf(10.f * z), 
						1.0f,
						-0.3f * sinf(10.f * x) + 3.f * x * sinf(10.f * z) 
							)
				); }
		);
		SceneObj* gridObj = new SceneObj(m_RootObj.get(), gridSprite, "grid");
		gridObj->SetPosition(glm::vec3(4.0f, 0.0f, 0.0f));

		// sampler
		m_pSampler = std::make_unique<Sampler>();
		m_pSampler->Bind(pContext.Get());
		// camera
		m_MainCamera = new PerspectiveCamera(m_RootObj.get(), nullptr, "Camera");
		m_MainCamera->SetPosition(glm::vec3(0.0f, 0.0f, 5.f));
		((PerspectiveCamera*)m_MainCamera)->SetHeight(3.0);
		((PerspectiveCamera*)m_MainCamera)->SetWidth(4.0);

		

		// LoadModel("L:\\OpenGL\\model\\78515\\78515.fbx");

	}
	*/

	void Graphics::DrawTriangle()
	{
		m_DeltaTime = timer.Mark();
		// set render target
		GFX_THROW_INFO_ONLY(pContext->OMSetRenderTargets(1, pTarget.GetAddressOf(), pDepthStencil.Get()));
		// clear depth stencil
		GFX_THROW_INFO_ONLY(pContext->ClearDepthStencilView(pDepthStencil.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0x00));

		m_RootObj->Update(m_DeltaTime);

		m_pLightManager->Flush();
		m_pLightManager->Bind(pContext.Get());

		m_pLightManager->UpdateConstantBuffer();
		m_pLightManager->Bind(pContext.Get());
		GFX_THROW_INFO_ONLY(m_RootObj->RenderV3(m_MainCamera));
	}
	void Graphics::Update()
	{
		m_DeltaTime = timer.Mark();
		m_RootObj->Update(m_DeltaTime);

		m_pLightManager->Flush();
		m_pLightManager->Bind(pContext.Get());

		m_pLightManager->UpdateConstantBuffer();
		m_pLightManager->Bind(pContext.Get());

		m_pMainSceneWin->Update(m_DeltaTime);
	}
	CameraObj* Graphics::GetMainCamera()
	{
		return m_MainCamera;
	}
	void Graphics::InitPipeline()
	{
		
		m_pScreenTex = std::make_unique<OSRTexture>(pDevice.Get(), 1280, 720);
		m_pDepthStencilTex = std::make_unique<DSTexture>(pDevice.Get(), 1280, 720);
		
		// create the window to show off-screen texture
		m_pMainSceneWin = std::make_unique<MainSceneWin>(m_pScreenTex->GetShaderResourceView(), dynamic_cast<Camera3DObj*>(m_MainCamera));

		// create scene for screen render
		Material* mtl = m_pMaterialManager->Add(
			new Material(dynamic_cast<Texture*>(m_pScreenTex.get()),m_pTextureManager.get()), 
			"screen texture");
		Mesh mesh(m_pBufferManager->GetVertexBuffer(BufferManager::Buffers::Vertex_P2_T2_Quad),
			m_pBufferManager->GetIndexBuffer(BufferManager::Buffers::Index_Quad),
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
			m_pShaderManager->GetShader("Textured Shader 2D"),
			mtl);
		SpriteV3 *sprite = m_pSpriteManager->Sprite(std::move(mesh));
		m_pScreenScene = std::make_unique<SceneObj>(nullptr, sprite, "off-screen render");

		m_pCamera2D = new Camera2DObj(m_pScreenScene.get(), nullptr, "screen camera");
		m_pCamera2D->SetViewLeft(-1.0f);
		m_pCamera2D->SetViewRight(1.0f);
		m_pCamera2D->SetViewBottom(-1.0f);
		m_pCamera2D->SetViewTop(1.0f);
		m_pCamera2D->SetViewNear(1.0f);
		m_pCamera2D->SetViewFar(-1.0f);

		// view port
		D3D11_VIEWPORT vp = {};
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		vp.Width = 1280;
		vp.Height = 720;
		vp.MinDepth = 0;
		vp.MaxDepth = 1;

		// init pipeline for screen render
		m_pPipelineScreen = std::make_unique<Screen>(pDevice.Get(), pContext.Get());
		m_pPipelineScreen->AddRenderTaget(pTarget.Get());
		m_pPipelineScreen->SetDepthStencilView(m_pDepthStencilTex->GetDSView());
		m_pPipelineScreen->SetViewPort(&vp);

		// init pipeline for main scene render
		m_pPipeline = std::make_unique<PipelineV1>(pDevice.Get(), pContext.Get());
		m_pPipeline->AddRenderTaget(m_pScreenTex->GetRenderTarget());
		m_pPipeline->SetDepthStencilView(pDepthStencil.Get());

		m_pPipeline->SetViewPort(&vp);


	}
	void Graphics::RunPipeline()
	{
		m_pPipeline->Render(m_MainCamera, m_RootObj.get());
		m_pPipelineScreen->Render(m_pCamera2D, m_pScreenScene.get());
	}
	ID3D11Device* Graphics::GetDevice()
	{
		return pDevice.Get();
	}

	ID3D11DeviceContext* Graphics::GetContext()
	{
		return pContext.Get();
	}



#ifdef  YOI_DEBUG

	DxgiInfoManager& Graphics::GetInfoManager()
	{
		return infoManager;
	}
#endif //  YOI_DEBUG


	Graphics& Graphics::GetInstance()
	{
		return *s_pInstance;
	}
	Shader* Graphics::GetShader(const std::string& shaderTitle)
	{
		if (m_pShaderManager.get())
			return m_pShaderManager->GetShader(shaderTitle);
		else
		{
			FileLogger::Error("Shader Manager didn't init.");
			FileLogger::Flush();
			return nullptr;
		}
	}
	Buffer* Graphics::GetBuffer(BufferManager::Buffers buffer)
	{
		return (GetInstance().m_pBufferManager)->GetBuffer(buffer);
	}
	IndexBuffer* Graphics::GetIndexBuffer(BufferManager::Buffers buffer)
	{
		return (GetInstance().m_pBufferManager)->GetIndexBuffer(buffer);
	}
	VertexBuffer* Graphics::GetVertexBuffer(BufferManager::Buffers buffer)
	{
		return (GetInstance().m_pBufferManager)->GetVertexBuffer(buffer);
	}
	ConstBuffer* Graphics::GetConstBuffer(BufferManager::Buffers buffer)
	{
		return (GetInstance().m_pBufferManager)->GetConstBuffer(buffer);
	}
	Buffer* Graphics::SetBufferData(BufferManager::Buffers buffer, unsigned int size, unsigned int offset, void* src)
	{
		return (GetInstance().m_pBufferManager)->SetBufferData(buffer, size, offset, src);
	}
	MaterialManager* Graphics::GetMaterialManager()
	{
		return m_pMaterialManager.get();
	}
	TextureManager* Graphics::GetTextureManager()
	{
		return m_pTextureManager.get();
	}
	SceneObj* Graphics::LoadModel(const std::string& path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph);
		m_LoadedTex.clear();

		// 查错
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			FileLogger::Error(importer.GetErrorString());
			return nullptr;
		}

		// 获取路径
		int a = path.find_last_of('/');
		int b = path.find_last_of('\\');
		int t = std::max(a, b);
		std::string directory = path.substr(0, t);
		std::string fileName = path.substr(t + 1, path.length() - t - 1);

		// material
		unsigned int materialBase = LoadMaterials(scene, directory);
		// meshed
		std::vector<Mesh> meshes;
		unsigned int meshCount = LoadMeshes(scene, materialBase, meshes);

		SceneObj* obj = ProcessNode(scene->mRootNode, meshes);
		obj->SetObjName(fileName);
		m_RootObj->PushChild(obj);

		std::ostringstream oss;
		oss << "Model file: \"" << path << "\" loaded.";
		FileLogger::Info(oss.str().c_str());

		return obj;
	}

	unsigned int Graphics::LoadMaterials(const aiScene* scene, const std::string& dictionary)
	{
		unsigned int materialCount = m_pMaterialManager->Size();
		aiTextureType texTypes[2] = { aiTextureType_DIFFUSE, aiTextureType_SPECULAR };
		Texture* yoiTextures[2] = { dynamic_cast<Texture*>(m_pTextureManager->GetAt()), dynamic_cast<Texture*>(m_pTextureManager->GetAt()) };
		for (unsigned int i = 0; i < scene->mNumMaterials; i++)
		{
			aiMaterial* material = scene->mMaterials[i];

			// load texture
			for (int j = 0; j < 2; j++)
			{
				int texCount = material->GetTextureCount(texTypes[j]);
				if (texCount > 0)
				{
					aiString textureFile;
					aiReturn ret = material->GetTexture(texTypes[j], 0, &textureFile);
					if (ret != aiReturn_SUCCESS)
					{
						FileLogger::Warn("assimp GetTexture fail");
						yoiTextures[j] = dynamic_cast<Texture*>(m_pTextureManager->GetAt());
					}
					else
					{// reading success

						const char* str = textureFile.C_Str();
						std::wstring wStr = UTF8ToWStr(std::string(str, textureFile.length));

						auto iter = m_LoadedTex.find(wStr);
						if (iter == m_LoadedTex.end())
						{
							std::ostringstream oss;
							oss << dictionary << '/' << str;

							unsigned int index = m_pTextureManager->GetTexCount();
							yoiTextures[j] = m_pTextureManager->LoadTexture(oss.str());
							m_LoadedTex[wStr] = index;
						}
						else
						{
							yoiTextures[j] = dynamic_cast<Texture*>(m_pTextureManager->GetAt((*iter).second));
						}
					}
				}
			}
			m_pMaterialManager->CreateMaterial(dynamic_cast<Texture*>(yoiTextures[0]), dynamic_cast<Texture*>(yoiTextures[1]), m_pTextureManager.get(), 64.0f);
		}

		return materialCount;
	}
	unsigned int Graphics::LoadMeshes(const aiScene* scene, unsigned int materialBaseInd, std::vector<Mesh>& meshes)
	{
		unsigned int i = 0;
		unsigned int vertexCount;

		for (; i < scene->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[i];
			Mesh yoiMesh = LoadMesh(mesh);
			yoiMesh.SetMaterial(m_pMaterialManager->At(materialBaseInd + mesh->mMaterialIndex));
			meshes.emplace_back(std::move(yoiMesh));
		}

		return i;
	}
	Mesh Graphics::LoadMesh(const aiMesh* mesh)
	{
		// float3 position
		// float3 normal
		// float2 texCoord

		// get vertex buffer
		unsigned int vertexCount = mesh->mNumVertices;
		size_t sizeOfOneVertex = 8 * sizeof(float);
		size_t vertexBufferSize = vertexCount * sizeOfOneVertex;
		float* buffer = new float[vertexCount * 8];
		unsigned int offset = 0;

		for (unsigned int i = 0; i < vertexCount; i++)
		{
			offset = i * 8;

			// position
			buffer[offset] = mesh->mVertices[i].x;
			buffer[offset + 1] = mesh->mVertices[i].y;
			buffer[offset + 2] = mesh->mVertices[i].z;

			// normal
			if (mesh->HasNormals())
			{
				buffer[offset + 3] = mesh->mNormals[i].x;
				buffer[offset + 4] = mesh->mNormals[i].y;
				buffer[offset + 5] = mesh->mNormals[i].z;
			}
			else
			{
				buffer[offset + 3] = 0.0f;
				buffer[offset + 4] = 1.0f;
				buffer[offset + 5] = 0.0f;
			}

			// texCoord
			if (mesh->mTextureCoords[0])
			{
				buffer[offset + 6] = mesh->mTextureCoords[0][i].x;
				buffer[offset + 7] = mesh->mTextureCoords[0][i].y;
			}
			else
			{
				buffer[offset + 6] = buffer[offset + 7] = 0.0f;
			}
		}

		// get index buffer
		unsigned int* indexBuffer;
		unsigned int indexCount = 0;
		size_t sizeOfOneIndex = sizeof(unsigned int);
		for (int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace& face = mesh->mFaces[i];
			indexCount += face.mNumIndices;
		}
		size_t indexBufferSize = indexCount * sizeof(unsigned int);
		indexBuffer = new unsigned int[indexCount];
		offset = 0;
		for (int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace& face = mesh->mFaces[i];
			memcpy(indexBuffer + offset, face.mIndices, face.mNumIndices * sizeof(unsigned int));
			offset += face.mNumIndices;
		}

		// load vertex buffer
		D3D11_BUFFER_DESC bufDesc = {};
		bufDesc.ByteWidth = vertexBufferSize;
		bufDesc.Usage = D3D11_USAGE_DEFAULT;
		bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufDesc.CPUAccessFlags = 0u;
		bufDesc.MiscFlags = 0u;
		bufDesc.StructureByteStride = sizeOfOneVertex;

		D3D11_SUBRESOURCE_DATA subData = {};
		subData.pSysMem = buffer;

		VertexBuffer* dxVertexBuffer = dynamic_cast<VertexBuffer*>(m_pBufferManager->AddVertexBuffer(Buffer(pDevice.Get(), &bufDesc, &subData),0u,vertexCount));
		dxVertexBuffer->SetLayout<VertexBuffer::Distrib<float, 3>, VertexBuffer::Distrib<float, 3>, VertexBuffer::Distrib<float, 2>>();

		// load index buffer
		bufDesc.ByteWidth = indexBufferSize;
		bufDesc.Usage = D3D11_USAGE_DEFAULT;
		bufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufDesc.CPUAccessFlags = 0u;
		bufDesc.MiscFlags = 0u;
		bufDesc.StructureByteStride = sizeOfOneIndex;

		subData.pSysMem = indexBuffer;

		IndexBuffer* dxIndexBuffer = dynamic_cast<IndexBuffer*>(m_pBufferManager->AddIndexBuffer(Buffer(pDevice.Get(), &bufDesc, &subData), indexCount, 0u));

		delete[] indexBuffer;
		delete[] buffer;


		return Mesh(dxVertexBuffer, dxIndexBuffer, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			m_pShaderManager->GetShader("Texture Color Shader"));
	}
	SceneObj* Graphics::ProcessNode(const aiNode* node, const std::vector<Mesh>& meshes)
	{
		SceneObj* obj = new SceneObj(nullptr, nullptr);
		SpriteV3* sprite = m_pSpriteManager->Sprite();
		for (int i = 0; i < node->mNumMeshes; i++)
		{
			sprite->AddMesh(meshes[node->mMeshes[i]]);
		}
		obj->SetSprite(sprite);


		for (int i = 0; i < node->mNumChildren; i++)
		{
			char bufObjName[16];
			SceneObj* child = ProcessNode(node->mChildren[i], meshes);
			_itoa(i, bufObjName, 10);
			child->SetObjName(std::string(bufObjName));
			obj->PushChild(child);
		}

		return obj;
	}
}