#pragma once
#include "common.h"
#include "DX11App.h"
#include "Scene.h"
#include "DllMacro.h"

namespace KGCA41B {
	class DLL_API Engine
	{
		SINGLETON(Engine)
#define ENGINE KGCA41B::Engine::GetInst()

	public:
		bool OnInit(HINSTANCE hinstance, LPCWSTR title, POINT wnd_size);
		void Run(Scene* scene);
		void OnResized();
		void OnRelease();

	public:
		POINT     GetWindowSize() { return wnd_size; }
		HINSTANCE GetInstanceHandle() { return hinstance; }
		HWND      GetWindowHandle() { return hwnd; }

	private:
		bool InitWindow(HINSTANCE hinstance, LPCWSTR title, POINT wnd_size);

	private:
		HINSTANCE hinstance;
		LPCWSTR   title;
		HWND      hwnd;
		POINT     wnd_size;

	};
}


