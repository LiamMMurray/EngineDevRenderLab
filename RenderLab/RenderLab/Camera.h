#pragma once
#include <DirectXMath.h>

struct Camera
{
	static float ScreenWidth;
	static float ScreenHeight;

	DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixIdentity();
	float client_width = 0.f;
	float client_height = 0.f;
	float verticalFOV = DirectX::XM_2PI / 8.f; // 45 degrees
	float aspectRatio = 0.f;
	float maxDrawDistance = 1000.f;
	float minDrawDistance = .1f;

	float maxFOV = DirectX::XM_2PI / 3.f;
	float minFOV = DirectX::XM_2PI / 16.f;

	DirectX::XMMATRIX view = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX projection = DirectX::XMMatrixIdentity();

	void UpdateViewMatrix(DirectX::XMMATRIX transform_matrix);
	void IncrementFOV(float value);
	void UpdateProjectionMatrix();
};
