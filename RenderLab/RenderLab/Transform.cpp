#include "pch.h"
#include "Transform.h"
using namespace DirectX;

XMMATRIX Transform::GetMatrix()
{
	return XMMatrixScalingFromVector(Scale)*XMMatrixRotationQuaternion(Rotation)*XMMatrixTranslationFromVector(Position);
}
void Transform::MoveOnForwardNormal(float value)
{
	Position = XMVectorAdd(Position, value*forward);
}
void Transform::MoveOnRightNormal(float value)
{
	Position = XMVectorAdd(Position, value*right);
}
void Transform::MoveOnUpNormal(float value)
{
	Position = XMVectorAdd(Position, value*up);
}
void Transform::MoveOnGlobalUp(float value)
{
	Position = XMVectorAdd(Position, value*UP);
}
void Transform::RotateOnGlobalUp(float angle)
{
	Rotation = XMQuaternionMultiply(Rotation, XMQuaternionRotationAxis(UP, angle));
	XMMATRIX m = XMMatrixRotationQuaternion(Rotation);
	forward = XMVector4Transform(FORWARD, m);
	right = XMVector4Transform(RIGHT, m);
}
void Transform::RotateOnRightNormal(float angle)
{
	Rotation = XMQuaternionMultiply(Rotation, XMQuaternionRotationAxis(right, angle));
	XMMATRIX m = XMMatrixRotationQuaternion(Rotation);
	forward = XMVector4Transform(FORWARD, m);
	right = XMVector4Transform(RIGHT, m);
}
void Transform::LookAt(XMVECTOR lookAt)
{
	if (XMVector3Length(lookAt - Position).m128_f32[0] != 0)
	{
		XMVECTOR newForward = XMVector3Normalize(lookAt - Position);
		XMVECTOR newRight = XMVector3Normalize(XMVector3Cross(UP,newForward));
		XMVECTOR newUp = XMVector3Normalize(XMVector3Cross(newForward, newRight));

		XMMATRIX r;

		r.r[0] = newRight;
		r.r[1] = newUp;
		r.r[2] = newForward;

		Rotation = DirectX::XMQuaternionRotationMatrix(r);
	}
}
void Transform::TurnTo(XMVECTOR lookAt, float ratio)
{
	if (XMVector3Length(lookAt - Position).m128_f32[0] != 0)
	{
		XMVECTOR newForward = XMVector3Normalize(lookAt - Position);
		XMVECTOR newRight = XMVector3Normalize(XMVector3Cross(UP, newForward));
		XMVECTOR newUp = XMVector3Normalize(XMVector3Cross(newForward, newRight));

		XMMATRIX r;

		r.r[0] = newRight;
		r.r[1] = newUp;
		r.r[2] = newForward;

		XMVECTOR targetRotation = DirectX::XMQuaternionRotationMatrix(r);
		Rotation = XMQuaternionSlerp(Rotation, targetRotation, ratio);
	}
}
XMMATRIX Transform::Test(XMVECTOR lookAt)
{
	return XMMatrixLookAtLH(Position, lookAt, UP);
}
const XMVECTOR Transform::FORWARD{ 0.0f, 0.0f, 1.0f, 1.0f };
const XMVECTOR Transform::RIGHT{ 1.0f, 0.0f, 0.0f, 1.0f };
const XMVECTOR Transform::UP{ 0.0f, 1.0f, 0.0f, 1.0f };
