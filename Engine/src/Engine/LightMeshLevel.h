#pragma once
#include "Components.h"
#include "DllMacro.h"
#include "DataTypes.h"
#include "ResourceMgr.h"
#include "InstancedObject.h"

namespace reality
{
	class DLL_API LightMeshLevel
	{
	public:
		LightMeshLevel();
		~LightMeshLevel();

	public:
		bool Create(string mesh_id, string vs_id);
		void Update();
		void Render();
		void Destroy();

	public:
		CbTransform level_transform;

	public:
		shared_ptr<LightMesh> level_mesh;
		shared_ptr<VertexShader> vertex_shader;
	};
}

