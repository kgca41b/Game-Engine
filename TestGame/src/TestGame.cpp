#include "TestGame.h"

using namespace KGCA41B;

void TestGame::OnInit()
{
	FbxLoader fbx_loader;

	string filepath = "G:/Repos/Game-Engine/RumbaDancing.fbx";
	if (fbx_loader.LoadFromFbxFile(filepath))
	{
		int mesh_count = fbx_loader.out_mesh_list.size();
		int vertex_count = 0;
		int bind_pose_count = 0;
		for (auto mesh : fbx_loader.out_mesh_list)
		{
			vertex_count += mesh->skinned_vertices.size();
			bind_pose_count += mesh->bind_poses.size();
		}

		OutputDebugString(L"========================================================\n");
		OutputDebugString(L"FBX �޽� �ε� ����\n");
		OutputDebugString((L"�޽� ���� : " + to_wstring(mesh_count) + L"\n").c_str());
		OutputDebugString((L"�� ���� ���� : " + to_wstring(vertex_count) + L"\n").c_str());
		OutputDebugString((L"���ε� ���� ���� : " + to_wstring(bind_pose_count) + L"\n").c_str());

		fbx_loader.LoadAnimation(FbxTime::eFrames60);

		int anim_count = fbx_loader.out_anim_list.size();
		int frame_count = fbx_loader.out_anim_list[0]->end_frame;

		OutputDebugString(L"FBX �ִϸ��̼� �ε� ����\n");
		OutputDebugString((L"�ִϸ��̼� ���� : " + to_wstring(anim_count) + L"\n").c_str());
		OutputDebugString((L"ù ��° �ִϸ��̼� ������ �� : " + to_wstring(frame_count) + L"\n").c_str());

	}
	fbx_loader.Destroy();
}

void TestGame::OnUpdate()
{
}

void TestGame::OnRender()
{
}

void TestGame::OnRelease()
{
}
