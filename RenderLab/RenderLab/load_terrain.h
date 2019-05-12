#pragma oncef
#include "pch.h"
#include "AABB.h"
#include <fstream>
#include <vector>

HRESULT load_terrain(ID3D11Device*& device, ID3D11Buffer*& vertex_shader, unsigned& num_vertices, std::vector<AABB>& aabbs, BVH& bvh, std::vector<end::float3>& pos);