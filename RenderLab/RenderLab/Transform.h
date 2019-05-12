#pragma once
#include <DirectXMath.h>
using namespace DirectX;

struct Transform
{
	// ========================================================================
	// global space normals
	// ========================================================================
	static const XMVECTOR FORWARD;
	static const XMVECTOR UP;
	static const XMVECTOR RIGHT;
	// ========================================================================
	XMVECTOR forward = FORWARD;
	XMVECTOR up = UP;
	XMVECTOR right = RIGHT;
	// ========================================================================
	XMVECTOR Position = DirectX::g_XMIdentityR3;
	XMVECTOR Rotation = DirectX::g_XMIdentityR3; // quaternion
	XMVECTOR Scale = DirectX::g_XMOne;
	// ========================================================================
	XMMATRIX GetMatrix();
	void MoveOnForwardNormal(float value);
	void MoveOnRightNormal(float value);
	void MoveOnUpNormal(float value);
	void MoveOnGlobalUp(float value);
	void RotateOnGlobalUp(float angle);
	void RotateOnRightNormal(float angle);
	void LookAt(XMVECTOR lookAt);
	void TurnTo(XMVECTOR lookAt, float r);
	XMMATRIX Test(XMVECTOR lookAt);
};
