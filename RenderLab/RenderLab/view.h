#pragma once

#include "math_types.h"
#include "Transform.h"
#include "AABB.h"
namespace end
{

	class view_t
	{
	public:
		// stores properties of a view
		//
		//	view and projection matrices	(REQUIRED)
		//	type information 				(optional) (Orthographic/Perspective/Cubemap/Shadowmap/etc)
		//	render target id(s)				(optional)
		//	viewport id						(optional)
		//		IMPORTANT: 
		//			Do not store an inverted view matrix.
		//			It will be much easier on you, me, and the CPU this way.
		//			When updating your constant buffers, send an inverted copy of the stored view matrix.
		int width;
		int height;

		// TODO m_world which represents the parent transform matrix should probably be a pointer to a matrix
		DirectX::XMMATRIX m_world		= IDENTITY4X4; 
		Transform m_view				= Transform();
		DirectX::XMMATRIX m_projection	= IDENTITY4X4;

		RECT viewport_size;
		float aspectRatio = 0;
		float fovAngleY = 70.0f * DirectX::XM_PI / 180.0f;
		float fNear = .01;
		float fFar = 100;
		// local transform components
		//Transform m_transform = Transform();
		// maintains a visible-set of renderable objects in view (implemented in a future assignment)
		DebugFrustum frustum;

		view_t() {};
	};
}