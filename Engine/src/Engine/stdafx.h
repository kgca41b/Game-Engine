#pragma once
#pragma warning(disable : 4275) 
#pragma warning(disable : 4819) 
#pragma warning(disable : 4267) 
#pragma warning(disable : 4101) 
#pragma warning(disable : 4251) 
#pragma warning(disable : 4244) 
#pragma warning(disable : 4099)
#pragma warning(disable : 26495)
#pragma warning(disable : 26451)

#include <reactphysics3d/reactphysics3d.h>
#include <fbxsdk.h>
#include "fmod.hpp"
#include <d3d11.h>
#include <list>
#include <vector>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <algorithm>
#include <functional>
#include <wrl.h>

#include <windows.h>
#include <tchar.h>
#include <typeinfo>
#include <atlconv.h>
#include <DirectXMath.h>
#include <iostream>
#include <locale>
#include <codecvt>
#include <functional>
#include <sstream>

#include "GlobalFunctions.h"

using namespace Microsoft::WRL;
using namespace DirectX;
using namespace std;

#define SINGLETON(type)\
public:\
static type* GetInst()\
{\
	static type mgr;\
	return &mgr;\
}\
private:\
type() {}\
~type() {}\
type(const type&) = delete;\
type& operator=(const type&) = delete;