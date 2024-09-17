#include "pch.h"
#include "SpriteManager.h"
#include "Logger\FileLogger.h"

namespace yoi
{
	SpriteManager::~SpriteManager()
	{
		for (SpriteV3* sprite : m_Sprites)
			delete sprite;
	}

	SpriteV3* SpriteManager::Copy(SpriteV3* sprite)
	{
		if (m_Sprites.find(sprite) == m_Sprites.end())
		{// didn't contain
			m_Sprites.insert(sprite);
		}
		SpriteV3* newSprite = new SpriteV3(sprite);
		m_Sprites.insert(newSprite);
		return newSprite;
	}

	SpriteV3* SpriteManager::CreateGrid(int raw, int col, Shader* shader, Material* material, BufferManager& bufferManager, const std::function<float(float, float)>& GenHeight, const std::function<glm::vec3(float, float)>& GenNormal)
	{
		if (raw <= 2 && col <= 2)
		{
			FileLogger::Warn("The Grid is too small");
			return nullptr;
		}

		struct Vertex
		{
			struct
			{
				float x, y, z;
			}Position;

			struct
			{
				float x, y, z;
			}Normal;

			struct
			{
				float u, v;
			} TexCoord;
		};

		/********************* init index buffer ***************************/
		// additional index for primitive restart

		size_t countIndex = (raw - 1) * col * 2 + raw - 1;
		size_t sizePerIndex = sizeof(unsigned);
		size_t sizeIndexBuffer = countIndex * sizeof(unsigned int);

		unsigned int* indexData = new unsigned int[countIndex];
		size_t counter = 0;
		for (int i = 0; i < raw - 1; i++)
		{
			for (int j = 0; j < col; j++)
			{
				indexData[counter++] = j + i * col;
				indexData[counter++] = j + ( i + 1 ) * col;
			}
			indexData[counter++] = 0xffffffff;
		}

		/******************* init vertex buffer **************************/

		size_t countVertex = raw * col;
		size_t sizePerVertex = sizeof(Vertex);
		size_t sizeVertexBuffer = sizePerVertex * countVertex;
		Vertex* vertexData = new Vertex[countVertex];

		float gridWidth = 1.0f / max(raw - 1, col - 1);
		float origin[2] = { - gridWidth * (col >> 1), -gridWidth * (raw >> 1)};
		counter = 0;

		float metaColWidth = 1.0f / (col - 1);
		float metaRawWidth = 1.0f / (raw - 1);

		for(int i =0;i < raw;i++)
			for (int j = 0; j < col; j++)
			{
				Vertex& tVertex = vertexData[counter ++];

				tVertex.Position.x = origin[0] + j * gridWidth;
				tVertex.Position.z = origin[1] + i * gridWidth;
				tVertex.Position.y = GenHeight(tVertex.Position.x, tVertex.Position.z);

				glm::vec3 tNormal = GenNormal(tVertex.Position.x, tVertex.Position.z);

				tVertex.Normal.x = tNormal.x;
				tVertex.Normal.y = tNormal.y;
				tVertex.Normal.z = tNormal.z;

				tVertex.TexCoord.u = metaColWidth * j;
				tVertex.TexCoord.v = metaRawWidth * i;

			}

		/***************** load buffer *************************/
		// load vertex buffer

		D3D11_BUFFER_DESC bd = {};
		bd.ByteWidth = sizeVertexBuffer;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.StructureByteStride = sizePerVertex;

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vertexData;

		ID3D11Buffer* vertexBuffer = bufferManager.AddBuffer(&bd, &sd);
		// load index buffer

		bd.ByteWidth = sizeIndexBuffer;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.StructureByteStride = sizePerIndex;
		
		sd.pSysMem = indexData;

		ID3D11Buffer* indexBuffer = bufferManager.AddBuffer(&bd, &sd);

		/*********** create mesh ******************/

		Mesh mesh(vertexBuffer, indexBuffer, sizePerVertex, 0, 0, countIndex, 0, 0, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, shader, material);
		
		/************create sprite ****************/

		SpriteV3* sprite = Sprite(mesh);

		delete[] vertexData;
		delete[] indexData;
		return sprite;
	}
	WaterWave* SpriteManager::CreateWave(int width, 
		ShaderManager& shaderManager,
		BufferManager& bufferManager,
		TextureManager& texManager,
		MaterialManager& materialManager,
		Texture* diffuseTexture ,
		Texture* specularTexture ,
		const std::function<float(float, float)>& GenHeight ,
		const std::function<glm::vec3(float, float)>& GenNormal
	)
	{
		if (width <= 2)
		{
			FileLogger::Warn("The Grid is too small");
			return nullptr;
		}

		struct Vertex
		{
			struct
			{
				float x, y, z;
			}Position;

			struct
			{
				float x, y, z;
			}Normal;

			struct
			{
				float u, v;
			} TexCoord;
		};

		/********************* init index buffer ***************************/
		// additional index for primitive restart

		size_t countIndex = (width - 1) * width * 2 + width - 1;
		size_t sizePerIndex = sizeof(unsigned);
		size_t sizeIndexBuffer = countIndex * sizeof(unsigned int);

		unsigned int* indexData = new unsigned int[countIndex];
		size_t counter = 0;
		for (int i = 0; i < width - 1; i++)
		{
			for (int j = 0; j < width; j++)
			{
				indexData[counter++] = j + i * width;
				indexData[counter++] = j + (i + 1) * width;
			}
			indexData[counter++] = 0xffffffff;
		}

		/******************* init vertex buffer **************************/

		size_t countVertex = width * width;
		size_t sizePerVertex = sizeof(Vertex);
		size_t sizeVertexBuffer = sizePerVertex * countVertex;
		Vertex* vertexData = new Vertex[countVertex];

		float gridWidth = 1.0f / (width - 1);
		float origin[2] = { -gridWidth * (width >> 1), -gridWidth * (width >> 1) };
		counter = 0;

		float metaColWidth = 1.0f / (width - 1);
		float metaRawWidth = 1.0f / (width - 1);

		for (int i = 0; i < width; i++)
			for (int j = 0; j < width; j++)
			{
				Vertex& tVertex = vertexData[counter++];

				tVertex.Position.x = origin[0] + j * gridWidth;
				tVertex.Position.z = origin[1] + i * gridWidth;
				tVertex.Position.y = GenHeight(tVertex.Position.x, tVertex.Position.z);

				glm::vec3 tNormal = GenNormal(tVertex.Position.x, tVertex.Position.z);

				tVertex.Normal.x = tNormal.x;
				tVertex.Normal.y = tNormal.y;
				tVertex.Normal.z = tNormal.z;

				tVertex.TexCoord.u = metaColWidth * j;
				tVertex.TexCoord.v = metaRawWidth * i;

			}

		/***************** load buffer *************************/
		// load vertex buffer

		D3D11_BUFFER_DESC bd = {};
		bd.ByteWidth = sizeVertexBuffer;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.StructureByteStride = sizePerVertex;

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vertexData;

		ID3D11Buffer* vertexBuffer = bufferManager.AddBuffer(&bd, &sd);
		// load index buffer

		bd.ByteWidth = sizeIndexBuffer;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.StructureByteStride = sizePerIndex;

		sd.pSysMem = indexData;

		ID3D11Buffer* indexBuffer = bufferManager.AddBuffer(&bd, &sd);

		/*********** create texture **************/
		if (diffuseTexture == nullptr)
			diffuseTexture = dynamic_cast<Texture*>(texManager.GetAt(0));
		if (specularTexture == nullptr)
			specularTexture = dynamic_cast<Texture*>(texManager.GetAt(0));
		Texture* normalTexture = dynamic_cast<Texture*>(texManager.GetAt(1));
		CSUTexture* heightTexture = new CSUTexture(width, width);
		
		/************ create material *************/
		Material* material = materialManager.CreateMaterial(
			diffuseTexture, 
			specularTexture, 
			normalTexture, 
			dynamic_cast<Texture*>(heightTexture),
			64.0f);


		/*********** create mesh ******************/

		Mesh mesh(vertexBuffer, indexBuffer, sizePerVertex, 0, 0, countIndex, 0, 0, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, 
			shaderManager.GetShader("Water Wave Render Shader"), material);

		/************create sprite ****************/


		WaterWave* sprite = new WaterWave(
			heightTexture,
			shaderManager.GetComputeShader("Water Wave Update"),
			shaderManager.GetComputeShader("Water Wave Disturb"),
			shaderManager.GetComputeShader("Water Wave Calm"),
			width,
			gridWidth
		);
		sprite->AddMesh(mesh);

		m_Sprites.insert(dynamic_cast<SpriteV3*>(sprite));

		delete[] vertexData;
		delete[] indexData;
		return sprite;
	}
}
