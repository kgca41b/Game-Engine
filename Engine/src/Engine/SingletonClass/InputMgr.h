#pragma once
#include "common.h"
#include "DllMacro.h"
#include <dinput.h>

namespace KGCA41B
{
	enum
	{
		KEY_NO_STATE,
		MOUSE_NO_STATE
	};
	class DLL_API InputMgr
	{
		SINGLETON(InputMgr);
#define DINPUT KGCA41B::InputMgr::GetInst()
	public:
		bool Init(HWND hwnd, HINSTANCE hinstacne);
		int Update();

		bool IsKeyPressed(UCHAR dik);
		int  GetKeyEvent(UCHAR dik);

		POINT GetMousePosition();
		XMFLOAT2 GetMouseVelocity();
		XMINT3 GetMouseButton();
		int GetMouseWheel();

	private:
		ComPtr<IDirectInput8> dinput;
		ComPtr<IDirectInputDevice8> di_keyboard;
		ComPtr<IDirectInputDevice8> di_mouse;

		UCHAR keyboard_state[256];
		DIMOUSESTATE mouse_state;
		POINT screen_size;

		POINT last_mousepos;
		POINT current_mousepos;

	};

}