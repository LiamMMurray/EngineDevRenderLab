#include "pch.h"
#include "Camera.h"
#include <algorithm>
using namespace DirectX;
using namespace std;

void Camera::UpdateViewMatrix(DirectX::XMMATRIX transformMatrix)
{
	view = XMMatrixInverse(NULL, transformMatrix);
}
void Camera::IncrementFOV(float value)
{
	value = verticalFOV - value;
	value = max(value, minFOV);
	value = min(value, maxFOV);
	verticalFOV = value;
}
void Camera::UpdateProjectionMatrix()
{
	aspectRatio = client_width / client_height;
	projection = XMMatrixPerspectiveFovLH(
		verticalFOV,
		aspectRatio,
		minDrawDistance,
		maxDrawDistance
	);
}
float Camera::ScreenWidth = 0;
float Camera::ScreenHeight = 0;