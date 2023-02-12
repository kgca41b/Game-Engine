#pragma once
#include "stdafx.h"
#include <reactphysics3d/reactphysics3d.h>
#include <d3d11.h>

namespace KGCA41B
{
	struct Vertex
	{
		XMFLOAT3   p;
		XMFLOAT3   n;
		XMFLOAT4   c;
		XMFLOAT2   t;
	};

	struct SkinnedVertex
	{
		XMFLOAT3   p;
		XMFLOAT3   n;
		XMFLOAT4   c;
		XMFLOAT2   t;
		XMFLOAT4   i;
		XMFLOAT4   w;

		SkinnedVertex operator +=(const Vertex& vertex)
		{
			this->p = vertex.p;
			this->n = vertex.n;
			this->c = vertex.c;
			this->t = vertex.t;

			return *this;
		}
	};

	template <typename VertexType>
	struct SingleMesh
	{
		vector<VertexType> vertices;
		ComPtr<ID3D11Buffer> vertex_buffer;
		vector<UINT> indices;
		ComPtr<ID3D11Buffer> index_buffer;
	};

	struct CbTransform
	{
		struct Data
		{
			XMMATRIX world_matrix;
		} data;

		ComPtr<ID3D11Buffer> buffer;
	};

	struct CbViewProj
	{
		struct Data
		{
			XMMATRIX view_matrix;
			XMMATRIX projection_matrix;
		} data;
		ComPtr<ID3D11Buffer> buffer;
	};

	struct CbSkeleton
	{
		struct Data
		{
			XMMATRIX  mat_skeleton[255];
		} data;
		ComPtr<ID3D11Buffer> buffer;
	};

	struct CbLight
	{
		struct Data
		{
			XMVECTOR light_direction;
			float light_bright;
		} data;
		ComPtr<ID3D11Buffer> buffer;
	};

	// Physics
	struct MouseRay
	{
		reactphysics3d::Vector3 start_point;
		reactphysics3d::Vector3 end_point;
	};

	// Sound

	enum SoundType
	{
		SFX,
		MUSIC,
	};

	struct Sound
	{
		string sound_filename;
		SoundType type;
		bool looping;
		FMOD::Channel* channel;
		FMOD::Sound* sound;
		UINT total_time;
		UINT current_time;
	public:
		Sound() : sound_filename(""), channel(nullptr), sound(nullptr), total_time(0), current_time(0) { }
		Sound(Sound& sound) = default;
	};

	struct SoundQueue
	{
		string		sound_filename;
		SoundType	sound_type;
		float		sound_volume;
		bool		is_looping;
	};

	enum class AxisType
	{
		FROWARD,
		RIGHT,
		UP,
		YAW,
		PITCH,
		ROLL,

		IDLE
	};

	enum class ActionType
	{
		JUMP,
		SIT,
		DASH,

		ATTACK,
		THROW,

		SKILL1,
		SKILL2,
		SKILL3,

		IDLE
	};
}


static void XMtoRP(XMVECTOR& xmv, reactphysics3d::Vector3& rpv)
{
	rpv.x = xmv.m128_f32[0];
	rpv.y = xmv.m128_f32[1];
	rpv.z = xmv.m128_f32[2];
}

static void XMtoRP(XMVECTOR& xmv, reactphysics3d::Vector2& rpv)
{
	rpv.x = xmv.m128_f32[0];
	rpv.y = xmv.m128_f32[1];
}

static void RPtoXM(reactphysics3d::Vector3& rpv, XMVECTOR& xmv)
{
	xmv.m128_f32[0] = rpv.x;
	xmv.m128_f32[1] = rpv.y;
	xmv.m128_f32[2] = rpv.z;
}

static void RPtoXM(reactphysics3d::Vector2& rpv, XMVECTOR& xmv)
{
	xmv.m128_f32[0] = rpv.x;
	xmv.m128_f32[1] = rpv.y;
}