#pragma once
#include "common.h"

namespace KGCA41B
{
	struct Vertex
	{
		Vertex() = default;
		~Vertex() = default;

		XMFLOAT3   p; // ��ġ 12
		XMFLOAT3   n; // �븻 24
		XMFLOAT4   c; // �÷� 40
		XMFLOAT2   t; // �ؽ�ó 48


	};

	struct SkinnedVertex : public Vertex
	{
		XMFLOAT4   i; // �ε��� 64
		XMFLOAT4   w; // ����ġ 80

		SkinnedVertex operator +=(const Vertex& vertex)
		{
			p = vertex.p;
			n = vertex.n;
			c = vertex.c;
			t = vertex.t;

			return *this;
		}
	};
}