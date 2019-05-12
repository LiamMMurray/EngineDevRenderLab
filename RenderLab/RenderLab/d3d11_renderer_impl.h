#pragma once
#include "pch.h"
#include "DeviceResources.h"
#include "renderer.h"
#include "view.h"
#include "XTime.h"
#include "pools.h"
#include "Transform.h"
#include <vector>
#include <bitset>
#include "AABB.h"
// NOTE: This header file must *ONLY* be included by renderer.cpp

namespace end
{
	struct particle
	{
		float3 pos = { 0, 0, 0 };
		float4 color = { 0, 0, 0, 0 };
		float3 prev_pos = { 0, 0, 0 };
		float4 prev_color = { 0, 0, 0, 0 };
	};
	struct particleCPUdata
	{
		DirectX::XMVECTOR accel;
		DirectX::XMVECTOR velocity;
		float timer;
	};
	struct emitter
	{
		DirectX::VertexPositionColor StartVertex;
		sorted_pool_t<int16_t, 256> indices;
		float spawn_timer = 0;
	};

	struct renderer::impl
	{
		// platform/api specific members, functions, etc.
		// Device, swapchain, resource views, states, etc. can be members here
		std::unique_ptr<DX::DeviceResources>    m_deviceResources;
		std::unique_ptr<DirectX::CommonStates>	m_states;
		std::unique_ptr<DirectX::Keyboard>		m_keyboard;
		std::unique_ptr<DirectX::Mouse>			m_mouse;
		XTime									m_timer = XTime();
		bool									m_paused = false;
		float									m_totalTime = 0;
		float									m_deltaTime = 0;
		// BEGIN TODO: move these variables to emitter struct
		float									m_particleSpawnInterval = 1/8.f;
		float									m_particleLifeTime = 5.f;
		// END TODO
		sorted_pool_t<particle, 1024>			m_particles;
		float									m_particleTimer;
		pool_t<particle, 1024>					m_shared_pool;
		pool_t<particleCPUdata, 1024>			m_particle_timers;
		std::vector<emitter>					m_emitters;
		std::vector<AABB>						m_AABBs;
		std::bitset<1024>						m_keys;
		unsigned								m_num_terrain_vertices;
		BVH										m_bvh;
		std::vector<end::float3>				m_pos;
		DebugFrustum debug_frustum;

		//ID3D11RenderTargetView*		render_target[VIEW_RENDER_TARGET::COUNT]{};

		//ID3D11DepthStencilView*		depthStencilView[VIEW_DEPTH_STENCIL::COUNT]{};

		std::vector<Transform>		m_transforms;
		ID3D11DepthStencilState*	depthStencilState[STATE_DEPTH_STENCIL::COUNT]{};

		ID3D11Buffer*				vertex_buffer[VERTEX_BUFFER::COUNT]{};

		ID3D11Buffer*				index_buffer[INDEX_BUFFER::COUNT]{};
		
		ID3D11InputLayout*			input_layout[INPUT_LAYOUT::COUNT]{};

		ID3D11VertexShader*			vertex_shader[VERTEX_SHADER::COUNT]{};

		ID3D11Buffer*				constant_buffer[CONSTANT_BUFFER::COUNT]{};

		ID3D11PixelShader*			pixel_shader[PIXEL_SHADER::COUNT]{};

		ID3D11RasterizerState*		rasterState[STATE_RASTERIZER::COUNT]{};

		D3D11_VIEWPORT				view_port[VIEWPORT::COUNT]{};

		/* Add more as needed...
		ID3D11SamplerState*			sampler_state[STATE_SAMPLER::COUNT]{};

		ID3D11BlendState*			blend_state[STATE_BLEND::COUNT]{};
		*/



		// Constructor for renderer implementation
		// 
		void Initialize(native_handle_type window_handle, view_t& view);

		void Update(view_t & v);

		void Render(view_t& v);

		void OnActivated();

		void OnDeactivated();

		void OnSuspending();

		void OnResuming();

		void OnWindowMoved();

		void OnWindowSizeChanged(end::view_t& view);

		void CreateDeviceDependentResources();

		void CreateWindowSizeDependentResources(view_t& view);

		void OnDeviceLost();

		void OnDeviceRestored(view_t& view);

		void XM_CALLCONV DrawGrid(DirectX::FXMVECTOR xAxis, DirectX::FXMVECTOR yAxis, DirectX::FXMVECTOR origin, size_t xdivs, size_t ydivs, DirectX::GXMVECTOR color, view_t& v);

		void Clear();

		~impl();



	};
}