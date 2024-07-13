#pragma once
#include "SpriteV1.h"

namespace yoi
{
	class SpriteV1Cube : public SpriteV1
	{
	private:
		// ∂®“Â∂•µ„
		struct Vertex
		{
			struct
			{
				float x, y, z;
			}Postion;
			struct
			{
				unsigned char r, g, b, a;
			}Color;
		};
	public:
		SpriteV1Cube(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
		~SpriteV1Cube() = default;
		SpriteV1Cube(const SpriteV1Cube&) = delete;
		SpriteV1Cube& operator = (SpriteV1Cube&) = delete;

		virtual void RenderV1(const glm::mat4& modelTrans, const glm::mat4& vpTrans);

	};
}