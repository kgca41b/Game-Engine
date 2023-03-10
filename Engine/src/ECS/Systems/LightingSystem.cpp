#include "stdafx.h"
#include "LightingSystem.h"

reality::LightingSystem::LightingSystem()
{
	global_light.data.direction = { 0, -1, 0, 0 };
	global_light.data.color = { 1, 1, 1, 1 };
}

reality::LightingSystem::~LightingSystem()
{
}

void reality::LightingSystem::OnCreate(entt::registry& reg)
{
	HRESULT hr;

	D3D11_BUFFER_DESC desc;
	D3D11_SUBRESOURCE_DATA subdata;

	ZeroMemory(&desc, sizeof(desc));
	ZeroMemory(&subdata, sizeof(subdata));

	desc.ByteWidth = sizeof(CbLight::Data);

	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	subdata.pSysMem = &global_light.data;

	hr = DX11APP->GetDevice()->CreateBuffer(&desc, &subdata, global_light.buffer.GetAddressOf());
}

void reality::LightingSystem::OnUpdate(entt::registry& reg)
{
	DX11APP->GetDeviceContext()->PSSetConstantBuffers(0, 1, global_light.buffer.GetAddressOf());
}
