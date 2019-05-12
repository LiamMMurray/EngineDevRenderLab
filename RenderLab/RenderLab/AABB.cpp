#include "AABB.h"
#include "debug_renderer.h"
using namespace end;
using namespace DirectX;
void AABB::render(const DebugFrustum& frustum) const
{
	EXPANDEDAABB a;
	// Upper/Lower	Back/Front	Left/Right
	// U/L			B/F			L/R
	a.UBL = { pos.x + -extents.x, pos.y + extents.y, pos.z + extents.z };
	a.UBR = { pos.x + extents.x, pos.y + extents.y, pos.z + extents.z };
	a.UFL = { pos.x + -extents.x, pos.y + extents.y, pos.z + -extents.z };
	a.UFR = { pos.x + extents.x, pos.y + extents.y, pos.z + -extents.z };
	a.LBL = { pos.x + -extents.x, pos.y + -extents.y, pos.z + extents.z };
	a.LBR = { pos.x + extents.x, pos.y + -extents.y, pos.z + extents.z };
	a.LFL = { pos.x + -extents.x, pos.y + -extents.y, pos.z + -extents.z };
	a.LFR = { pos.x + extents.x, pos.y + -extents.y, pos.z + -extents.z };


	float4 color = { 1, 0, 0, 1 };
	if (frustum.hitTest(this))
		color = { 0, 1, 0, 1 }; 

	// TOP SQUARE
	debug_renderer::add_line(a.UBL, a.UBR, color);
	debug_renderer::add_line(a.UFL, a.UFR, color);
	debug_renderer::add_line(a.UBL, a.UFL, color);
	debug_renderer::add_line(a.UBR, a.UFR, color);
	// BOT SQUARE
	debug_renderer::add_line(a.LBL, a.LBR, color);
	debug_renderer::add_line(a.LFL, a.LFR, color);
	debug_renderer::add_line(a.LBL, a.LFL, color);
	debug_renderer::add_line(a.LBR, a.LFR, color);
	// VERTICAL LINES
	debug_renderer::add_line(a.LBL, a.UBL, color);
	debug_renderer::add_line(a.LFL, a.UFL, color);
	debug_renderer::add_line(a.LBR, a.UBR, color);
	debug_renderer::add_line(a.LFR, a.UFR, color);
}

// returns true if AABB is outside left frustum plane
bool DebugFrustum::leftFrustumPlaneCullTest(const AABB& aabb) const
{
	XMVECTOR frustumNormal = XMVector3Normalize(XMVector3Cross(Far.BL - Near.BL, Near.TL - Near.BL));
	float ExtentsDot = XMVector3Dot(XMLoadFloat3(&aabb.extents), XMVectorAbs(frustumNormal)).m128_f32[0];
	float PlaneToPlaneDot = XMVector3Dot(frustumNormal, Near.TL).m128_f32[0];
	float SphereToPlaneDot = XMVector3Dot(frustumNormal, XMLoadFloat3(&aabb.pos)).m128_f32[0];
	float D = PlaneToPlaneDot - SphereToPlaneDot;

	if (D > ExtentsDot)
		return false;
	else if (D > -ExtentsDot)
		return false;

	return true;
}
// returns true if AABB is outside right frustum plane
bool DebugFrustum::rightFrustumPlaneCullTest(const AABB& aabb) const
{
	XMVECTOR frustumNormal = XMVector3Normalize(XMVector3Cross(Near.TR - Near.BR, Far.BR - Near.BR));
	float ExtentsDot = XMVector3Dot(XMLoadFloat3(&aabb.extents), XMVectorAbs(frustumNormal)).m128_f32[0];
	float PlaneToPlaneDot = XMVector3Dot(frustumNormal, Near.TR).m128_f32[0];
	float SphereToPlaneDot = XMVector3Dot(frustumNormal, XMLoadFloat3(&aabb.pos)).m128_f32[0];
	float D = PlaneToPlaneDot - SphereToPlaneDot;

	if (D > ExtentsDot)
		return false;
	else if (D > -ExtentsDot)
		return false;

	return true;
}

bool DebugFrustum::topFrustumPlaneCullTest(const AABB& aabb) const
{
	XMVECTOR frustumNormal = XMVector3Normalize(XMVector3Cross(Far.TR - Near.TL, Near.TR - Near.TL));
	float ExtentsDot = XMVector3Dot(XMLoadFloat3(&aabb.extents), XMVectorAbs(frustumNormal)).m128_f32[0];
	float PlaneToPlaneDot = XMVector3Dot(frustumNormal, Near.TR).m128_f32[0];
	float SphereToPlaneDot = XMVector3Dot(frustumNormal, XMLoadFloat3(&aabb.pos)).m128_f32[0];
	float D = PlaneToPlaneDot - SphereToPlaneDot;

	if (D > ExtentsDot)
		return false;
	else if (D > -ExtentsDot)
		return false;

	return true;
}

bool DebugFrustum::bottomtFrustumPlaneCullTest(const AABB& aabb) const
{
	XMVECTOR frustumNormal = XMVector3Normalize(XMVector3Cross(Near.BR - Near.BL, Far.BR - Near.BL));
	float ExtentsDot = XMVector3Dot(XMLoadFloat3(&aabb.extents), XMVectorAbs(frustumNormal)).m128_f32[0];
	float PlaneToPlaneDot = XMVector3Dot(frustumNormal, Near.BR).m128_f32[0];
	float SphereToPlaneDot = XMVector3Dot(frustumNormal, XMLoadFloat3(&aabb.pos)).m128_f32[0];
	float D = PlaneToPlaneDot - SphereToPlaneDot;

	if (D > ExtentsDot)
		return false;
	else if (D > -ExtentsDot)
		return false;

	return true;
}

bool DebugFrustum::frontFrustumPlaneCullTest(const AABB& aabb) const
{
	XMVECTOR frustumNormal = XMVector3Normalize(XMVector3Cross(Near.TL - Near.BL, Near.BR - Near.BL));
	float ExtentsDot = XMVector3Dot(XMLoadFloat3(&aabb.extents), XMVectorAbs(frustumNormal)).m128_f32[0];
	float PlaneToPlaneDot = XMVector3Dot(frustumNormal, Near.BL).m128_f32[0];
	float SphereToPlaneDot = XMVector3Dot(frustumNormal, XMLoadFloat3(&aabb.pos)).m128_f32[0];
	float D = PlaneToPlaneDot - SphereToPlaneDot;

	if (D > ExtentsDot)
		return false;
	else if (D > -ExtentsDot)
		return false;

	return true;
}

bool DebugFrustum::backFrustumPlaneCullTest(const AABB& aabb) const
{
	XMVECTOR frustumNormal = XMVector3Normalize(XMVector3Cross(Far.BR - Far.BL, Far.TL - Far.BL));
	float ExtentsDot = XMVector3Dot(XMLoadFloat3(&aabb.extents), XMVectorAbs(frustumNormal)).m128_f32[0];
	float PlaneToPlaneDot = XMVector3Dot(frustumNormal, Far.BL).m128_f32[0];
	float SphereToPlaneDot = XMVector3Dot(frustumNormal, XMLoadFloat3(&aabb.pos)).m128_f32[0];
	float D = PlaneToPlaneDot - SphereToPlaneDot;

	if (D > ExtentsDot)
		return false;
	else if (D > -ExtentsDot)
		return false;

	return true;
}

void AABB::append(const AABB * aabb)
{
	if (BVH::count == 8)
		int pause = 0;
	float test = pos.x - extents.x;
	float minX = min(aabb->pos.x - aabb->extents.x, pos.x - extents.x);
	float maxX = max(aabb->pos.x + aabb->extents.x, pos.x + extents.x);

	float minY = min(aabb->pos.y - aabb->extents.y, pos.y - extents.y);
	float maxY = max(aabb->pos.y + aabb->extents.y, pos.y + extents.y);

	float minZ = min(aabb->pos.z - aabb->extents.z, pos.z - extents.z);
	float maxZ = max(aabb->pos.z + aabb->extents.z, pos.z + extents.z);

	float averageX = (minX + maxX) / 2;
	float averageY = (minY + maxY) / 2;
	float averageZ = (minZ + maxZ) / 2;

	pos = { averageX, averageY, averageZ };
	if (BVH::count == 8)
		int pause = 0;
	if (extents.x < .3 || extents.y < .3 < extents.z < .3)
		int pause = 0;
	extents = { abs(maxX - minX)/2, abs(maxY - minY)/2, abs(maxZ - minZ)/2 };
	if (extents.x < .3 || extents.y < .3 < extents.z < .3)
		int pause = 0;
}

int BVH::count = 0;

bool DebugFrustum::hitTest(const AABB * aabb) const
{
	if (backFrustumPlaneCullTest(*aabb))
		return false;
	else if (frontFrustumPlaneCullTest(*aabb))
		return false;
	else if (leftFrustumPlaneCullTest(*aabb))
		return false;
	else if (rightFrustumPlaneCullTest(*aabb))
		return false;
	else if (bottomtFrustumPlaneCullTest(*aabb))
		return false;
	else if (topFrustumPlaneCullTest(*aabb))
		return false;

	return true;
}

void BVH::insert(AABB * aabb, int vertex_index)
{
	if (right_child != nullptr)
	{
		manhattan_distance(aabb->pos, left_child->current->pos) < manhattan_distance(aabb->pos, right_child->current->pos)
			? left_child->insert(aabb, vertex_index) : right_child->insert(aabb, vertex_index);

		recursive_parent_append(aabb);
		count++; // static
	}
	else
	{
		left_child = new BVH();
		if (left_child->parent == this)
			int pause = 0;
		left_child->parent = this;
		left_child->current = new AABB();
		left_child->current->extents = current->extents;
		left_child->current->pos = current->pos;
		left_child->vertex_index = this->vertex_index;

		right_child = new BVH();
		if (right_child->parent == this)
			int pause = 0;
		right_child->parent = this;
		right_child->current = new AABB();
		right_child->current->extents = aabb->extents;
		right_child->current->pos = aabb->pos;
		right_child->vertex_index = vertex_index;
		count++; // static
	}
}

void BVH::render(const DebugFrustum & debugFrustum, std::vector<end::float3>& vertices) const
{
	if (!debugFrustum.hitTest(current))
		return;
	else
	{
		current->render(debugFrustum);
		if (right_child != nullptr)
		{
			right_child->render(debugFrustum, vertices);
			left_child->render(debugFrustum, vertices);
		}
		else
		{
			end::float3 pos1 = vertices[vertex_index];
			end::float3 pos2 = vertices[vertex_index+1];
			end::float3 pos3 = vertices[vertex_index+2];
			pos1.y += .7f;
			pos2.y += .7f;
			pos3.y += .7f;
			debug_renderer::add_line(pos1, pos2, {1,1,1,1});
			debug_renderer::add_line(pos2, pos3, { 1,1,1,1 });
			debug_renderer::add_line(pos3, pos1, { 1,1,1,1 });
		}
	}

}
