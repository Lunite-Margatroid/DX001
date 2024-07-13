#include "pch.h"
#include "ObjectPropertyWin.h"

namespace yoi
{
	ObjectPropertyWin::ObjectPropertyWin(SceneObj* rootObj)
		:m_RootObj(rootObj), m_SelectObj(rootObj)
	{
	}
	void ObjectPropertyWin::ShowWindow()
	{
		ImGui::Begin(GetWindowTitle(), &m_Open);
		DrawObjTree(m_RootObj);
		m_SelectObj->RenderImGui();
		ImGui::End();
	}
	const char* ObjectPropertyWin::GetWindowTitle() const
	{
		return "Scene Object Property";
	}
	void ObjectPropertyWin::DrawObjTree(const SceneObj* node)
	{
		ImGuiTreeNodeFlags flag = ImGuiTreeNodeFlags_None;
		flag |= ImGuiTreeNodeFlags_DefaultOpen;
		bool isSelected = m_SelectObj == node;
		bool isLeaf = !node->HasChild();
		// set leaf node if no child
		if (isLeaf)
		{
			flag |= ImGuiTreeNodeFlags_Leaf;
		}

		// if selected set selected node
		if (isSelected)
		{
			flag |= ImGuiTreeNodeFlags_Selected;
		}

		if (ImGui::TreeNodeEx(node->GetObjName().c_str(), flag))
		{
			// double click on selected node
			if (isSelected && ImGui::IsItemClicked())
			{
				// double click event
				if (m_IO.MouseDoubleClicked[0])
				{

				}
			}
			// set selected if not selected when clicked
			if (!isSelected && ImGui::IsItemClicked())
			{
				m_SelectObj = const_cast<SceneObj*>(node);
			}
			
			// dragging event
			if (ImGui::BeginDragDropSource())
			{
				ImGui::Text(node->GetObjName().c_str());
				const SceneObj** ppSceneObj = const_cast<const SceneObj**>(new SceneObj*);
				*ppSceneObj = node;
				ImGui::SetDragDropPayload("ppSceneObj", ppSceneObj, sizeof(void*));
				delete ppSceneObj;
				ImGui::EndDragDropSource();
			}
			if (ImGui::BeginDragDropTarget())
			{
				if (ImGuiPayload* data = const_cast<ImGuiPayload*>(ImGui::AcceptDragDropPayload("SceneObj_ptr")))
				{
					if (data->IsDelivery())
					{
						SceneObj* obj = *reinterpret_cast<SceneObj**>(data->Data);
						// obj is not root
						if (obj->GetParent())
						{
							const_cast<SceneObj*>(node)->PushChild(obj);
						}
					}
				}
				ImGui::EndDragDropTarget();
			}

			if (!isLeaf)
			{
				for (const SceneObj* obj : node->GetChildren())
				{
					DrawObjTree(obj);
				}
			}

			ImGui::TreePop();
		}
	}
}
