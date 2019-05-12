#pragma once
#include <memory>
#include "view.h"

#define FSGD_END_USE_D3D

namespace end
{
	// APIs/Platforms have their own types for representing a handle to a 'window'
	// They will generally all fit in the 8-bytes of a void*.
	// HWND is actually just a typedef/alias for a void*.
	using native_handle_type = void*;

	// Interface to the renderer
	class renderer
	{
	public:

		void Initialize(native_handle_type window_handle, int width, int height);

		~renderer();

		void Update();

		void render();

		// Messages
		void OnActivated();
		void OnDeactivated();
		void OnSuspending();
		void OnResuming();
		void OnWindowMoved();
		void OnWindowSizeChanged(int width, int height);
		void GetDefaultSize(int& width, int& height) const;
		void KeyboardMessage(UINT keyInput);
		view_t default_view = view_t();
		// Rendering loop timer.
	private:

		// PImpl idiom ("Pointer to implementation")

		// 'impl' will define the implementation for the renderer elsewhere
		struct impl;

		// Pointer to the implementation
		impl* m_pImpl;
	};

	// The following types just defines scopes for enum values.
	// The enum values can be used as indices in arrays.
	// These enum values can be added to as needed.

	struct VIEWPORT {
		enum { DEFAULT, COUNT };
	};

	struct CONSTANT_BUFFER {
		enum { MVP, COUNT };
	};

	struct VERTEX_SHADER {
		enum { BOX_VERTEX, DEBUG_VERTEX, COLORED_VERTEX, COUNT };
	};

	struct PIXEL_SHADER {
		enum { BOX_VERTEX, DEBUG_VERTEX, COLORED_VERTEX, COUNT };
	};

	struct VIEW_RENDER_TARGET {
		enum { DEFAULT, COUNT };
	};

	struct INDEX_BUFFER {
		enum { DEFAULT, COUNT };
	};

	struct INPUT_LAYOUT {
		enum { BOX_VERTEX, DEBUG_VERTEX, COLORED_VERTEX, COUNT };
	};

	struct STATE_RASTERIZER {
		enum { DEFAULT, COUNT };
	};

	struct VIEW_DEPTH_STENCIL {
		enum { DEFAULT, COUNT };
	};

	struct STATE_DEPTH_STENCIL {
		enum { DEFAULT, COUNT };
	};

	struct VERTEX_BUFFER {
		enum { DEBUG_VERTEX, BOX_VERTEX, COLORED_VERTEX, COUNT };
	};

	/* Add more as needed...
	enum STATE_SAMPLER{ DEFAULT = 0, COUNT };

	enum STATE_BLEND{ DEFAULT = 0, COUNT };
	*/

	struct KEYS_BITSET
	{
		enum { W, A, S, D };
	};
}