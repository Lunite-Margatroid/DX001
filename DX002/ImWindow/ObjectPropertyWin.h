#pragma once
#include "ImWindow.h"
#include "SceneObject\SceneObj.h"

namespace yoi
{
	class ObjectPropertyWin : public ImWindow
	{
	protected:
		SceneObj* m_RootObj;
		SceneObj* m_SelectObj;

		void DrawObjTree(const SceneObj* node);
	public:
		ObjectPropertyWin(SceneObj* rootObj);
		~ObjectPropertyWin() = default;
		ObjectPropertyWin(const ObjectPropertyWin&) = delete;
		ObjectPropertyWin& operator = (const ObjectPropertyWin&) = delete;

		void ShowWindow() override;
		const char* GetWindowTitle() const override;
	};
}