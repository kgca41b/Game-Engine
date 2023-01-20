#include "DX11App.h"

bool DX11App::Init(POINT _bufferSize, HWND _hWnd)
{
    HRESULT hr;

    //=== ���丮 ����

    hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)dxgiFactory.GetAddressOf());
    if (FAILED(hr))
        return false;

    //=== ����ü�� �� ����̽� ����

    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = _hWnd;
    swapChainDesc.Windowed = true;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    swapChainDesc.BufferDesc.Width = _bufferSize.x;
    swapChainDesc.BufferDesc.Height = _bufferSize.y;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    D3D_FEATURE_LEVEL pFeatureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
    };

    hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        D3D11_CREATE_DEVICE_BGRA_SUPPORT,
        pFeatureLevels,
        1,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        dx11SwapChain.GetAddressOf(),
        dx11Device.GetAddressOf(),
        NULL,
        dx11Context.GetAddressOf()
    );
    if (FAILED(hr))
        return false;

    //=== ����ü�����κ��� ���� �ؽ�ó�� ����Ÿ�� �� ����

    ID3D11Texture2D* texture;
    hr = dx11SwapChain.Get()->GetBuffer(NULL, __uuidof(ID3D11Texture2D), (void**)(&texture));
    if (FAILED(hr))
        return false;

    hr = dx11Device.Get()->CreateRenderTargetView(texture, nullptr, dx11RTView.GetAddressOf());
    if (FAILED(hr))
        return false;

    ZeroMemory(texture, sizeof(ID3D11Texture2D));

    //=== ����_���ٽ� ���� ����

    D3D11_TEXTURE2D_DESC textureDesc;
    ZeroMemory(&textureDesc, sizeof(textureDesc));
    textureDesc.Width = _bufferSize.x;
    textureDesc.Height = _bufferSize.y;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;
    textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    hr = dx11Device->CreateTexture2D(&textureDesc, 0, &texture);
    if (FAILED(hr))
        return false;

    //=== ����_���ٽ� �� ����

    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
    ZeroMemory(&dsvDesc, sizeof(dsvDesc));
    dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    hr = dx11Device->CreateDepthStencilView(texture, &dsvDesc, dx11DSView.GetAddressOf());
    if (FAILED(hr))
        return false;

    texture->Release();

	return true;
}

void DX11App::PreRender(bool _solidOn, bool _alphaOn, bool _depthOn)
{
    if (_alphaOn)
        dx11Context->OMSetBlendState(DXStates.AlphaBlend(), 0, -1);

    if (_depthOn)
        dx11Context->OMSetDepthStencilState(DXStates.DepthDefault(), 0xff);

    if (_solidOn)
        dx11Context->RSSetState(DXStates.CullClockwise());
    else
        dx11Context->RSSetState(DXStates.Wireframe());

    dx11Context->OMSetRenderTargets(1, dx11RTView.GetAddressOf(), dx11DSView.Get());
    dx11Context->ClearRenderTargetView(GetRenderTargetView(), clearcolor);
    dx11Context->ClearDepthStencilView(GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1, 0);
    dx11Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void DX11App::PostRender(bool _vsyncOn)
{
    if (_vsyncOn)
        dx11SwapChain.Get()->Present(1, 0);
    else
        dx11SwapChain.Get()->Present(0, 0);
}

bool DX11App::OnResize(HWND _hWnd, UINT newX, UINT newY)
{
    if (dx11Device == nullptr)
        return false;

    HRESULT hr;

    //=== ����Ÿ�� �� ������ ���� ����
    dx11Context.Get()->OMSetRenderTargets(0, 0, 0);
    dx11RTView.ReleaseAndGetAddressOf();
    dx11DSView.ReleaseAndGetAddressOf();

    dx11Context.Get()->ClearState();
    dx11Context.Get()->Flush();

    //=== ����� ��������
    DXGI_SWAP_CHAIN_DESC currentSD;
    hr = dx11SwapChain.Get()->GetDesc(&currentSD);
    hr = dx11SwapChain.Get()->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

    //=== ������� �ؽ�ó�� ���ο� ����Ÿ�� �� ����
    ID3D11Texture2D* texture = nullptr;
    hr = dx11SwapChain.Get()->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&texture);
    hr = dx11Device.Get()->CreateRenderTargetView(texture, NULL, dx11RTView.GetAddressOf());

    //=== ����_���ٽ� ���� ����
    DXGI_SWAP_CHAIN_DESC scd;
    dx11SwapChain.Get()->GetDesc(&scd);
    D3D11_TEXTURE2D_DESC td;
    ZeroMemory(&td, sizeof(td));
    td.Width = scd.BufferDesc.Width;
    td.Height = scd.BufferDesc.Height;
    td.MipLevels = 1;
    td.ArraySize = 1;
    td.Format = DXGI_FORMAT_R24G8_TYPELESS;
    td.SampleDesc.Count = 1;
    td.Usage = D3D11_USAGE_DEFAULT;
    td.CPUAccessFlags = 0;
    td.MiscFlags = 0;
    td.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    hr = dx11Device.Get()->CreateTexture2D(&td, NULL, &texture);

    //=== ����_���ٽ� �� ����.
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
    ZeroMemory(&dsvDesc, sizeof(dsvDesc));
    dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    hr = dx11Device.Get()->CreateDepthStencilView(texture, &dsvDesc, dx11DSView.GetAddressOf());


    texture->Release();

    //=== ����Ʈ ����
    viewPort.Width = (float)newX;
    viewPort.Height = (float)newY;
    viewPort.MinDepth = 0.0f;
    viewPort.MaxDepth = 1.0f;
    viewPort.TopLeftX = 0.0f;
    viewPort.TopLeftY = 0.0f;
    dx11Context.Get()->RSSetViewports(1, &viewPort);
    return true;
}