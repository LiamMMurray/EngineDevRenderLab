#include "debug_renderer.h"
#include <assert.h>
#include <array>

// Anonymous namespace
namespace
{
	// Declarations in an anonymous namespace are global BUT only have internal linkage.
	// In other words, these variables are global but are only visible in this source file.

	// Maximum number of debug lines at one time (i.e: Capacity)
	constexpr size_t MAX_LINE_VERTS = 512000; 

	// CPU-side buffer of debug-line verts
	// Copied to the GPU and reset every frame.
	size_t line_vert_count = 0;
	std::array< end::colored_vertex, MAX_LINE_VERTS> line_verts;
}

namespace end
{
	namespace debug_renderer
	{
		void add_line(float3 point_a, float3 point_b, float4 color_a, float4 color_b)
		{
			line_verts[line_vert_count] = { point_a, color_a };
			line_verts[line_vert_count + 1] = { point_b, color_b };
			line_vert_count += 2;
		}

		void clear_lines()
		{
			line_vert_count = 0;
		}

		const colored_vertex* get_line_verts()
		{ 
			return line_verts.data();
		}

		size_t get_line_vert_count() 
		{ 
			return line_vert_count;
		}

		size_t get_line_vert_capacity()
		{
			return MAX_LINE_VERTS;
		}
	}
}