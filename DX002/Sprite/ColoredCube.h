#pragma once
#include "Mesh.h"

namespace yoi
{
	class ColoredCube : public Mesh
	{
	public:
		ColoredCube();
		~ColoredCube() = default;
		ColoredCube(const ColoredCube&) = default;
		ColoredCube& operator = (const ColoredCube&) = default;
	};
}