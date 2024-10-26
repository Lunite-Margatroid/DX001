#pragma once
#include "pch.h"
#include "Application.h"

namespace yoi
{
	class Sandbox : public Application
	{
	public:
		Sandbox();

		void DoFrame() override;
	};
}