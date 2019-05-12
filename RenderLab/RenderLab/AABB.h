#pragma once
#include <DirectXMath.h>
#include "math_types.h"
#include <list>
#include <vector>
struct AABB;
struct EXPANDEDAABB;
struct RECT3
{
	DirectX::XMVECTOR TL = DirectX::g_XMZero;
	DirectX::XMVECTOR TR = DirectX::g_XMZero;
	DirectX::XMVECTOR BL = DirectX::g_XMZero;
	DirectX::XMVECTOR BR = DirectX::g_XMZero;
};

struct DebugFrustum
{
	/*cache these values only recalculate them when the their view dependent values change */

	//Hnear = 2 * tan(fov / 2) * nearDist
	//Hfar = 2 * tan(fov / 2) * farDist
	//Wnear = Hnear * viewRatio
	//Wfar = Hfar * viewRatio
	float debug_near;
	float debug_far;
	float Width_Near;
	float Height_Near;
	float Width_Far;
	float Height_Far;

	//RECT3 Near = RECT3();
	//RECT3 Far = RECT3();

	RECT3 Near = RECT3();
	RECT3 Far = RECT3();

	bool hitTest(const AABB*) const;

private:
	bool leftFrustumPlaneCullTest(const AABB& aabb) const;
	bool rightFrustumPlaneCullTest(const AABB& aabb) const;
	bool topFrustumPlaneCullTest(const AABB& aabb) const;
	bool bottomtFrustumPlaneCullTest(const AABB& aabb) const;
	bool frontFrustumPlaneCullTest(const AABB& aabb) const;
	bool backFrustumPlaneCullTest(const AABB& aabb) const;
};
struct AABB
{
	end::float3 pos = { 0, 0, 0 };
	end::float3 extents = { 0, 0, 0 };
	void render(const DebugFrustum& frustum) const;

	void append(const AABB* aabb);
};
struct EXPANDEDAABB
{
	end::float3 UBL; // upper back left
	end::float3 UBR; // upper back right
	end::float3 UFL; // upper front left
	end::float3 UFR; // upper front right
	end::float3 LBL; // lower back left
	end::float3 LBR; // ...
	end::float3 LFL;
	end::float3 LFR;
};
struct BVH
{
	static int count;
	//AABB* parent = nullptr;
	AABB* current = nullptr;
	int vertex_index;
	BVH* parent = nullptr;
	BVH* left_child = nullptr;
	BVH* right_child = nullptr;

	void insert(AABB* aabb, int vertex_index);
	
	void render(const DebugFrustum& debugFrustum, std::vector<end::float3>&) const;
	
	void recursive_parent_append(AABB* aabb)
	{
		if (parent)
			parent->recursive_parent_append(aabb);
		if (current->extents.x < .3 || current->extents.y < .3 || current->extents.z < .3)
			int pause = 0;
		current->append(aabb);
	}

	float manhattan_distance(end::float3 A, end::float3 B)
	{
		return abs(A.x - B.x) + abs(A.y - B.y) + abs(A.z - B.z);
	}


};
