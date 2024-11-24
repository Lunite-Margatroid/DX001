#include "pch.h"
#include "QuadTree.h"

namespace yoi
{
	yoi::QuadTree::QuadTree(const Box2D& area, int capacity, int maxDepth)
		:m_Area(area), m_Capacity(capacity), m_MaxDepth(maxDepth)
	{
	}
	std::list<Box2D*> QuadTree::DetectPoint(const float x, const float y)
	{
		return DetectPoint(x, y, &m_Root, m_Area.Left, m_Area.Top, m_Area.Right, m_Area.Bottom);
	}
	Box2D* QuadTree::InsertBox(Box2D* box)
	{
		return InsertBox(box, &m_Root, m_Area.Left, m_Area.Top, m_Area.Right, m_Area.Bottom);
	}

	Box2D* QuadTree::InsertBox(Box2D* box, Node* node, const float left, const float top, const float right, const float bottom, const int depth)
	{
		if (box->Left < left || box->Right > right || box->Top > top || box->Bottom < bottom)
			return nullptr;

		const float centerX = (left + right) / 2.f;
		const float centerY = (top + bottom) / 2.f;
		if (node->Children)// is not a leaf node
		{
			if (
				InsertBox(box, node->Children, left, top, centerX, centerY, depth + 1) || // left top
				InsertBox(box, node->Children + 1, centerX, top, right, centerY, depth + 1) || // right top
				InsertBox(box, node->Children + 2, left, centerY, centerX, bottom, depth + 1) || // left bottom
				InsertBox(box, node->Children + 3, centerX, centerY, right, bottom, depth + 1)// right bottom
				)
			{}
			else
			{
				node->Boxes.push_back(box);
			}
		}
		else // is a leaf node
		{
			if (node->Boxes.size() < m_Capacity || depth >= m_MaxDepth)
			{
				node->Boxes.push_back(box);
			}
			else
			{
				std::vector<Box2D*> tempBoxes = node->Boxes;
				node->Boxes.clear();
				node->Children = new Node[4];
				for (Box2D* box : tempBoxes)
				{
					InsertBox(box, node, left, top, right, bottom, depth);
				}
			}
		}
	}
	std::list<Box2D*> QuadTree::DetectPoint(const float x, const float y, Node* node, const float left, const float top, const float right, const float bottom)
	{
		std::list<Box2D*> boxes;
		for (auto* box : node->Boxes)
		{
			PointBox2DOverlap(x, y, *box);
		}
		
		if (node->Children)
		{
			const float centerX = (left + right) / 2;
			const float centerY = (top + bottom) / 2;
			// left top
			boxes.merge(DetectPoint(x, y, node->Children, left, top, centerX, centerY));
			// right top
			boxes.merge(DetectPoint(x, y, node->Children + 1, centerX, top, right, centerY));
			// left bottom
			boxes.merge(DetectPoint(x, y, node->Children + 2, left, centerY, centerX, bottom));
			// right bottom
			boxes.merge(DetectPoint(x, y, node->Children + 3, centerX, centerY, right, bottom));
		}
		return std::list<Box2D*>();
	}
}
