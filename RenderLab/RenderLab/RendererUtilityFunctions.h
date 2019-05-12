#pragma once
#include <string>
#include <d3d11_1.h>
#include <d3d11_2.h>
#include <stdint.h>


HRESULT create_vertex_shader_from_cso(
	std::string filename,
	const D3D11_INPUT_ELEMENT_DESC* InputDesc, 
	size_t numElementsInputDesc,
	ID3D11Device* device,
	ID3D11VertexShader** vertexShader,
	ID3D11InputLayout** inputLayout);


HRESULT create_pixel_shader_from_cso(
	std::string filename,
	ID3D11Device* device,
	ID3D11PixelShader** pixelShader
);


HRESULT create_dynamic_constant_buffer(
	D3D11_SUBRESOURCE_DATA* subResource,
	ID3D11Device* device,
	size_t constBufferSize,
	ID3D11Buffer** constantBuffer);

HRESULT create_vertex_buffer(ID3D11Device * device, D3D11_SUBRESOURCE_DATA * subResource, size_t constBufferSize, ID3D11Buffer ** constantBuffer);

HRESULT create_dynamic_vertex_buffer(ID3D11Device * device, D3D11_SUBRESOURCE_DATA * subResource, size_t constBufferSize, ID3D11Buffer ** constantBuffer);

HRESULT create_index_buffer(D3D11_SUBRESOURCE_DATA * subResource, ID3D11Device * device, size_t constBufferSize, ID3D11Buffer ** constantBuffer);


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
);

HRESULT CreateDepthStencilState(ID3D11Device* device, bool depthEnable, bool stencilEnable, bool writeEnable, _Out_ ID3D11DepthStencilState** pResult);
