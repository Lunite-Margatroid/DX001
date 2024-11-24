#pragma once

namespace yoi
{
	struct Box2D
	{
		float Left, Top, Right,Bottom;
		void* NativePtr;
		Box2D(float left = 0.f, float top = 0.f, float right = 0.f, float bottom = 0.0f, void* ptr = nullptr)
			:Left(left), Top(top), Right(right), Bottom(bottom), NativePtr(ptr)
		{
			if (Left > Right)
				std::swap(Left, Right);
			if (Bottom > Top)
				std::swap(Bottom, Top);
		}

		inline bool operator == (const Box2D& other)
		{
			return Left == other.Left && Top == other.Top && Right == other.Right && Bottom == other.Bottom && NativePtr == other.NativePtr;
		}
	};

	inline bool PointBox2DOverlap(const float x, const float y, const Box2D& box)
	{
		return x <= box.Right && x >= box.Left && y >= box.Bottom && y <= box.Top;
	}

	inline bool Box2DOverlap(const Box2D& a, const Box2D& b)
	{
		bool left = a.Right < b.Left;
		bool right = a.Left > b.Right;
		bool up = a.Bottom > b.Top;
		bool down = a.Top < b.Bottom;

		return !(left || right || up || down);
	}

	class QuadTree
	{
	private:
		struct Node
		{
			std::vector<Box2D*> Boxes;
			Node* Children;

			Node()
			{
				Children = nullptr;
			}

			void Release()
			{
				if (Children)
				{
					Children[0].Release();
					Children[1].Release();
					Children[2].Release();
					Children[3].Release();
				}
				
			}

			~Node()
			{
				Release();
				delete Children;
			}
		};

		Box2D m_Area;

		int m_Capacity;
		int m_MaxDepth;
		Node m_Root;

		Box2D* InsertBox(Box2D* box, Node* node, const float left, const float top, const float right, const float bottom, const int depth = 1);

		std::list<Box2D*> DetectPoint(const float x, const float y, Node* node, const float left, const float top , const float right, const float bottom);

	public:
		QuadTree(const Box2D& area = Box2D(-1000.f, 1000.f, 1000.f, -1000.f), int capacity = 4, int maxDepth = 8);
		QuadTree(const QuadTree&) = delete;
		QuadTree& operator = (const QuadTree&) = delete;
		~QuadTree() = default;

		std::list<Box2D*> DetectPoint(const float x, const float y);
		std::list<Box2D*> DetectBox2D(const Box2D & box);
		Box2D* InsertBox(Box2D* box);
		void RemoveBox(const Box2D* box);
	
	};
}