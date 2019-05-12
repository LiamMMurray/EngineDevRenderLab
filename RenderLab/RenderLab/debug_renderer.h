#pragma once

#include "math_types.h"
#include "VertexTypes.h"
// Interface to the debug renderer
namespace end
{
	namespace debug_renderer
	{
		void add_line(float3 point_a, float3 point_b, float4 color_a, float4 color_b);

		inline void add_line(float3 p, float3 q, float4 color) { add_line(p, q, color, color); }

		inline void add_line(colored_vertex v_a, colored_vertex v_b) { add_line(v_a.position, v_b.position, v_a.color, v_b.color); }

		void clear_lines();

		const colored_vertex* get_line_verts();

		size_t get_line_vert_count();

		size_t get_line_vert_capacity();
	}
}