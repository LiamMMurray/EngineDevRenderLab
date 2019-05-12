#include "pch.h"
#include "DeviceResources.h"
#include "RendererUtilityFunctions.h"
#include <assert.h>
#include <fstream>
#include "DDSTextureLoader.h"
#include "DirectXHelpers.h"
using namespace std;


//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT create_vertex_shader_from_cso(
	string filename,
	const D3D11_INPUT_ELEMENT_DESC* InputDesc, 
	size_t numElementsInputDesc,
	ID3D11Device* device, 
	ID3D11VertexShader** vertexShader,
	ID3D11InputLayout** inputLayout)
{
	{
		// check if shader files exist	
		ifstream file(filename);
		if (file.fail())
		{
			file.close();
			return E_FAIL;
		}
		file.close();
	}

	{
		// open and read in byte data
		using namespace std;
		streampos begin, end;
		ifstream file(filename, ios::in | ios::binary | ios::ate);

		// allocate memory
		int vsDataSize = file.tellg();
		uint8_t* vsByteCode = new uint8_t[vsDataSize];
		// store Input Layout data
		file.seekg(0, ios::beg);
		file.read((char*)(vsByteCode), vsDataSize);
		file.close();

		// create vertex shader
		ID3D11VertexShader* vs;
		HRESULT hr = device->CreateVertexShader(vsByteCode, vsDataSize, NULL, vertexShader);

		// create input layout
		HRESULT hresult = device->CreateInputLayout(InputDesc, numElementsInputDesc, vsByteCode, vsDataSize, inputLayout);
		delete[] vsByteCode;
		if (FAILED(hresult))
		{
	
			return hresult;
		}
	}

	return S_OK;
}


//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT create_pixel_shader_from_cso(
	string filename, 
	ID3D11Device* device,
	ID3D11PixelShader** pixelShader)
{
	// check if shader files exist
	{
		ifstream file(filename);
		if (file.fail())
		{
			return E_FAIL;
		}
		file.close();
	}
	// open and read in byte data
	{
		unsigned ps_length{ 0 };
		uint8_t* ps_data{ nullptr };

		streampos begin, end;
		ifstream file(filename, ios::in | ios::binary | ios::ate);
		ps_length = file.tellg();
		ps_data = new uint8_t[ps_length];
		file.seekg(0, ios::beg);
		file.read((char*)ps_data, ps_length);
		file.close();

		HRESULT hr = device->CreatePixelShader(ps_data, ps_length, NULL, pixelShader);
		if (FAILED(hr))
		{
			delete[] ps_data;
			return E_FAIL;
		}

		delete[] ps_data;
		return S_OK;
	}
}


//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT create_dynamic_constant_buffer(
	D3D11_SUBRESOURCE_DATA* subResourceData,
	ID3D11Device* device, 
	size_t constBufferSize,
	ID3D11Buffer** constantBuffer)
{
	D3D11_BUFFER_DESC bd;
	bd.ByteWidth = constBufferSize;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	HRESULT hresult = device->CreateBuffer(&bd, subResourceData, constantBuffer);
	if (FAILED(hresult))
		return hresult;

	return S_OK;
}

HRESULT create_vertex_buffer(
	ID3D11Device* device,
	D3D11_SUBRESOURCE_DATA* subResourceData,
	size_t constBufferSize,
	ID3D11Buffer** constantBuffer
)
{
	D3D11_BUFFER_DESC bd;
	bd.ByteWidth = constBufferSize;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	HRESULT hresult = device->CreateBuffer(&bd, subResourceData, constantBuffer);
	if (FAILED(hresult))
		return hresult;

	return S_OK;
}

HRESULT create_dynamic_vertex_buffer(
	ID3D11Device* device,
	D3D11_SUBRESOURCE_DATA* subResourceData,
	size_t constBufferSize,
	ID3D11Buffer** constantBuffer
)
{
	D3D11_BUFFER_DESC bd;
	bd.ByteWidth = constBufferSize;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	HRESULT hresult = device->CreateBuffer(&bd, subResourceData, constantBuffer);
	if (FAILED(hresult))
		return hresult;

	return S_OK;
}

HRESULT create_index_buffer(
	D3D11_SUBRESOURCE_DATA* subResourceData,
	ID3D11Device* device,
	size_t constBufferSize,
	ID3D11Buffer** constantBuffer
)
{
	D3D11_BUFFER_DESC bd;
	bd.ByteWidth = constBufferSize;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	HRESULT hresult = device->CreateBuffer(&bd, subResourceData, constantBuffer);
	if (FAILED(hresult))
		return hresult;

	return S_OK;
}
//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT initialize_directx(
	int width,
	int height,
	HWND hwnd, 
	D3D11_CREATE_DEVICE_FLAG flags,
	IDXGISwapChain** swap_chain,
	ID3D11Device** device,
	ID3D11DeviceContext** device_context, 
	ID3D11Texture2D** back_buffer,
	ID3D11RenderTargetView** render_target_view
)
{
	ID3D11Texture2D* temp_back_buffer;
	if (back_buffer == NULL)
		back_buffer = &temp_back_buffer;

	*swap_chain = nullptr;
	*device = nullptr;
	*device_context = nullptr;
	*back_buffer = nullptr;
	*render_target_view = nullptr;

	HRESULT hresult;

	DXGI_MODE_DESC back_buffer_desc;
	back_buffer_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	back_buffer_desc.Width = width;
	back_buffer_desc.Height = height;
	back_buffer_desc.RefreshRate.Numerator = 0;
	back_buffer_desc.RefreshRate.Denominator = 0;
	back_buffer_desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	back_buffer_desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	DXGI_SWAP_CHAIN_DESC swap_chain_desc;
	swap_chain_desc.BufferCount = 2;
	swap_chain_desc.BufferDesc = back_buffer_desc;
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING | DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swap_chain_desc.OutputWindow = hwnd;
	swap_chain_desc.SampleDesc = DXGI_SAMPLE_DESC{ 1, 0 };
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	swap_chain_desc.Windowed = true;

	D3D_FEATURE_LEVEL feature_levels[7]
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	hresult = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags, feature_levels, 7, D3D11_SDK_VERSION, &swap_chain_desc, swap_chain, device, NULL, device_context);
	if (FAILED(hresult))
		return hresult;

	hresult = (*swap_chain)->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)back_buffer);
	if (FAILED(hresult))
		return hresult;
	
	hresult = (*device)->CreateRenderTargetView(*back_buffer, NULL, render_target_view);
	if (FAILED(hresult))
		return hresult;

	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.MaxDepth = 1.0f;
	vp.MinDepth = 0.0;
	vp.Width = width;
	vp.Height = height;
	(*device_context)->RSSetViewports(1, &vp);
	return S_OK;
}
//--------------------------------------------------------------------------------------
// Based off the commonstates.h depth stencil state create functions
//--------------------------------------------------------------------------------------
HRESULT CreateDepthStencilState(ID3D11Device* device, bool depthEnable, bool stencilEnable, bool writeEnable, _Out_ ID3D11DepthStencilState** pResult)
{
	D3D11_DEPTH_STENCIL_DESC desc = {};

	desc.DepthEnable = depthEnable ? TRUE : FALSE;
	desc.DepthWriteMask = writeEnable ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
	desc.DepthFunc = D3D11_COMPARISON_LESS;

	desc.StencilEnable = stencilEnable ? TRUE : FALSE;
	desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR; 
	desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;

	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;

	HRESULT hr = device->CreateDepthStencilState(&desc, pResult);

	if (SUCCEEDED(hr))
		DirectX::SetDebugObjectName(*pResult, "RendererUtilityFunctions:DepthStencilState");

	return hr;
}