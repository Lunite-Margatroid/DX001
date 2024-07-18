#pragma once
#include "Mesh.h"

namespace yoi
{
	class TexturedCube : public Mesh
	{
	public:
		TexturedCube();
		~TexturedCube() = default;
		TexturedCube(const TexturedCube&) = default;
		TexturedCube& operator = (const TexturedCube &) = default;
	};
}