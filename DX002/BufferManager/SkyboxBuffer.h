#pragma once
namespace yoi
{
	template<>
	void InitBuffer<BufferManager::Buffers::Vertex_P3_Skybox>(BufferManager* bufManager)
	{
		struct Vertex
		{
			float x, y, z;
		};

		Vertex vertices[24] = 
		{
			// x +
			{1.0f, 1.0f, -1.0f	},
			{1.0f, 1.0f, 1.0f	},
			{1.0f, -1.0f, -1.0f },
			{1.0f, -1.0f, 1.0f	},

			// x -
			{-1.0f, 1.0f, 1.0f	},
			{-1.0f, 1.0f, -1.0f	},
			{-1.0f, -1.0f, 1.0f},
			{-1.0f, -1.0f, -1.0f},

			// y +
			{-1.0f, 1.0f, 1.0f},
			{1.0f, 1.0f, 1.0f},
			{-1.0f, 1.0f, -1.0f},
			{1.0f, 1.0f, -1.0f	},

			// y -
			{-1.0f, -1.0f, -1.0f},
			{1.0f, -1.0f, -1.0f},
			{-1.0f, -1.0f, 1.0f},
			{1.0f, -1.0f, 1.0f},

			// z +
			{1.0f, 1.0f, 1.0f	},
			{-1.0f, 1.0f,1.0f	},
			{1.0f, -1.0f, 1.0f	},
			{-1.0f, -1.0f, 1.0f},

			// z-
			{-1.0f, 1.0f, -1.0f},
			{1.0f, 1.0f, -1.0f	},
			{-1.0f, -1.0f, -1.0f},
			{1.0f, -1.0f, -1.0f}
		};

		unsigned int indices[36] = {0, 1, 2, 2, 1, 3};
		for (int i = 6; i < 36; i++)
		{
			indices[i] = indices[i - 6] + 4;
		}

		VertexBuffer* vertexBuffer = bufManager->AddVertexBuffer(
			yoi::BufferManager::Buffers::Vertex_P3_Skybox, 
			vertices,
			sizeof(Vertex),
			24u);
		vertexBuffer->SetLayout<VertexBuffer::Distrib<float, 3>>();

		bufManager->AddIndexBuffer(
			yoi::BufferManager::Buffers::Index_Skybox,
			indices,
			36);
	}
}