#include "pch.h"
#include "load_terrain.h"
#include <vector>
#include "math_types.h"
#include "RendererUtilityFunctions.h"
#include "AABB.h"
using namespace std;
using namespace end;

float interpolate(float A, float B, float r)
{
	return A + ((B - A) * r);
}
HRESULT load_terrain(ID3D11Device*& device, ID3D11Buffer*& vertex_buffer, unsigned& num_vertices, vector<AABB>& aabbs, BVH& bvh, vector<end::float3>& pos)
{
	{
		fstream terrain_stream("terrain.bin", ios::in | ios::binary | ios::ate);
		unsigned file_size = terrain_stream.tellg();
		if (file_size == (unsigned)-1)
			return E_FAIL;
		terrain_stream.seekg(0);

		char*data = new char[file_size];
		char*data_cur = data;
		char*data_begin = data;
		terrain_stream.read(data, file_size);
		uint32_t vertex_count;


		vertex_count = *((uint32_t*)data_cur);
		num_vertices = (unsigned)vertex_count;
		data_cur += sizeof(uint32_t);

		float maxX = -INFINITY;
		float maxZ = -INFINITY;
		for (uint32_t i = 0; i < vertex_count; i++)
		{
			pos.push_back(*((float3*)data_cur));
			data_cur += sizeof(float3);
			if (pos[i].x > maxX)
				maxX = pos[i].x;
			if (pos[i].z > maxZ)
				maxZ = pos[i].z;
		}
		vector<float3> norms;
		for (uint32_t i = 0; i < vertex_count; i++)
		{
			norms.push_back(*((float3*)data_cur));
			data_cur += sizeof(float3);
		}
		vector<float2> uvs;
		for (uint32_t i = 0; i < vertex_count; i++)
		{
			uvs.push_back(*((float2*)data_cur));
			data_cur += sizeof(float2);
		}

		unsigned vertex_size = sizeof(float3) + sizeof(float3) + sizeof(float2);
		//float3 pos : SV_POSITION;
		//float3 normal : NORMAL;
		//float2 uvs : TEXCOORD;
		char* vertices = new char[vertex_size * vertex_count];
		for (uint32_t i = 0; i < vertex_count; i++)
		{
			pos[i] = float3(pos[i].x - maxX/2, pos[i].y - 15, pos[i].z - maxZ/2);
			memcpy(&vertices[i*vertex_size], &pos[i], sizeof(float3));
			memcpy(&vertices[i*vertex_size+sizeof(float3)], &norms[i], sizeof(float3));
			memcpy(&vertices[i*vertex_size+sizeof(float3)+sizeof(float3)], &uvs[i], sizeof(float2));
		}

		vector < pair<AABB, int> > aabb_pairs;
		// create initial aabb's
		for (uint32_t i = 0; i < vertex_count; i += 3)
		{
			AABB aabb;

			float minX = pos[i].x;
			minX = min(minX, pos[i + 1].x);
			minX = min(minX, pos[i + 2].x);

			float minY = pos[i].y;
			minY = min(minY, pos[i + 1].y);
			minY = min(minY, pos[i + 2].y);

			float minZ = pos[i].z;
			minZ = min(minZ, pos[i + 1].z);
			minZ = min(minZ, pos[i + 2].z);

			float maxX = pos[i].x;
			maxX = max(maxX, pos[i + 1].x);
			maxX = max(maxX, pos[i + 2].x);

			float maxY = pos[i].y;
			maxY = max(maxY, pos[i + 1].y);
			maxY = max(maxY, pos[i + 2].y);

			float maxZ = pos[i].z;
			maxZ = max(maxZ, pos[i + 1].z);
			maxZ = max(maxZ, pos[i + 2].z);

			aabb.pos = {(maxX + minX)/2, (maxY + minY) / 2, (maxZ + minZ) / 2 };
			aabb.extents = {maxX - aabb.pos.x, 2, maxZ - aabb.pos.z};

			aabbs.push_back(aabb); // debug render all aabbs
			aabb_pairs.push_back(make_pair(aabb, i)); // actual aabbs that will make up the bvh
		}
		random_shuffle(aabb_pairs.begin(), aabb_pairs.end());

		bvh.current = new AABB(); 
		bvh.current->pos = aabb_pairs[0].first.pos;
		bvh.current->extents = aabb_pairs[0].first.extents;
		bvh.vertex_index = aabb_pairs[0].second;

		for (size_t i = 1; i < aabb_pairs.size(); i++)
		{
			bvh.insert(&aabb_pairs[i].first, aabb_pairs[i].second);
		}
		
		BVH* itr = &bvh;
		do
		{
			static int count = 0;
			if (itr->current->extents.x == 0 ||
				itr->current->extents.y == 0 ||
				itr->current->extents.z == 0)
				int pause = 0;

			itr = itr->right_child;
			count++;
		} while (itr != nullptr);
		//ID3D11Device* device,
		//	D3D11_SUBRESOURCE_DATA* subResourceData,
		//	size_t constBufferSize,
		//	ID3D11Buffer** constantBuffer
		D3D11_SUBRESOURCE_DATA subresource_data;
		subresource_data.pSysMem = vertices;
		HRESULT hresult = create_vertex_buffer(device, &subresource_data, vertex_size*vertex_count, &vertex_buffer);
		terrain_stream.close();
		delete[] vertices;
		//TriangleMeshBuffer out;
		//std::fstream mesh_file(filename, ios::in | ios::binary | ios::ate);
		//unsigned file_size = mesh_file.tellg();
		//if (file_size == (unsigned)-1)
		//	return out;

		//mesh_file.seekg(0);
		//char*data = new char[file_size];
		//char*data_begin = data;
		//mesh_file.read(data, file_size);

		//out.num_indices = *((unsigned*)data);
		//data += sizeof(unsigned);
		//out.num_vertices = *((unsigned*)data);
		//data += sizeof(unsigned);
		//out.indices = new unsigned[out.num_indices];
		//out.vertices = new DEFAULT_VIN[out.num_vertices];
		//memcpy(out.indices, data, out.num_indices * sizeof(unsigned));
		//data += out.num_indices * sizeof(unsigned);
		//memcpy(out.vertices, data, out.num_vertices * sizeof(DEFAULT_VIN));
		//delete[] data_begin;
		//return out;
	}
	return S_OK;
}
auto get_closest_aabb_pair_manhattan_distance(std::vector<AABB>& aabbs)
{

	// for every AABB 
	for (int i = 0; i < aabbs.size(); i++)
	{
		// get the manhattan distance for every other aabb
		for (int j = 0; j < aabbs.size(); j++)
		{
			if (i == j)
				continue;
			//float manhattan_distance = ManhattanDistance(aabbs[i], aabbs[j]);
		//	manhattan_distance 
		}
	}
}

