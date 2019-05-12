#include "pch.h"
#include "d3d11_renderer_impl.h"
#include "RendererUtilityFunctions.h"
#include "debug_renderer.h"
#include <random>
#include "load_terrain.h"
using namespace DirectX;

void end::renderer::impl::Initialize(native_handle_type window_handle, view_t& view)
{
	//load_terrain(vertex_shader[VERTEX_SHADER::DEBUG_VERTEX]);
	//load_terrain();
	m_deviceResources = std::make_unique<DX::DeviceResources>();
	m_keyboard = std::make_unique<DirectX::Keyboard>();
	m_mouse = std::make_unique<DirectX::Mouse>();
	m_mouse->SetWindow((HWND)window_handle);
	m_mouse->SetMode(Mouse::Mode::MODE_RELATIVE);
	// TODO:
	// Setup the default view 
	//	default_view should be setup with a perspective projection
	//	default_view should have a view matrix that positions the view at(0, 15, -15) and looks at(0, 0, 0)
	//		IMPORTANT: XMMatrixLookAtLH returns an inverted matrix
	view.m_view.Position = { 0, 15, -15 };
	view.m_view.LookAt({ 0,0,0 });
	view.m_projection = IDENTITY4X4; // initialize to identity for debugging purposes - otherwise 
	// TODO:
	// create device and swap chain
	// create depth-stencil buffer/state/view
	// create rasterizer state
	// create input layout
	// create vertex and pixel shaders	
	m_deviceResources->SetWindow((HWND)window_handle, view.width, view.height);
	m_deviceResources->CreateDeviceResources();
	CreateDeviceDependentResources();

	m_deviceResources->CreateWindowSizeDependentResources();
	CreateWindowSizeDependentResources(view);
	m_timer.Restart();

	m_emitters.push_back(emitter());
	m_emitters.back().StartVertex = VertexPositionColor{
		/*initial position*/float3{-5.f,0,0},
		/*innitial color*/	float4{1,.5f,1,1}
	};

	m_emitters.push_back(emitter());
	m_emitters.back().StartVertex = VertexPositionColor{
		/*initial position*/float3{5.f,0,0},
		/*innitial color*/	float4{1,1,0,1}
	};

	m_transforms.push_back(Transform());
	m_transforms.back().Position = {0,0,0 };
	m_transforms.push_back(Transform());
	m_transforms.push_back(Transform());
	m_transforms.back().Position = {3, -3, 0};

}

void end::renderer::impl::CreateDeviceDependentResources()
{
	auto context = m_deviceResources->GetD3DDeviceContext();
	auto device = m_deviceResources->GetD3DDevice();

	m_states = std::make_unique<DirectX::CommonStates>(device);

	HRESULT hresult = CreateDepthStencilState(device, true, true, true, &depthStencilState[STATE_DEPTH_STENCIL::DEFAULT]);
	DX::ThrowIfFailed(hresult);
	//------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------
	hresult = create_vertex_shader_from_cso("Shaders/Compiled/vs_debug.cso", VertexPositionNormalTexture::InputElements, VertexPositionNormalTexture::InputElementCount, device, &vertex_shader[VERTEX_SHADER::DEBUG_VERTEX], &input_layout[INPUT_LAYOUT::DEBUG_VERTEX]);
	DX::ThrowIfFailed(hresult);
	hresult = create_pixel_shader_from_cso("Shaders/Compiled/ps_debug.cso", device, &pixel_shader[PIXEL_SHADER::DEBUG_VERTEX]);
	DX::ThrowIfFailed(hresult);
	hresult = create_vertex_shader_from_cso("Shaders/Compiled/vs_cube.cso", VertexID::InputElements, VertexID::InputElementCount, device, &vertex_shader[VERTEX_SHADER::BOX_VERTEX], &input_layout[INPUT_LAYOUT::BOX_VERTEX]);
	DX::ThrowIfFailed(hresult);
	hresult = create_pixel_shader_from_cso("Shaders/Compiled/ps_cube.cso", device, &pixel_shader[PIXEL_SHADER::BOX_VERTEX]);
	DX::ThrowIfFailed(hresult);
	hresult = load_terrain(device, vertex_buffer[VERTEX_BUFFER::DEBUG_VERTEX], m_num_terrain_vertices, m_AABBs, m_bvh, m_pos);
	DX::ThrowIfFailed(hresult);

	D3D11_SUBRESOURCE_DATA vertexBufferInitData;
	vertexBufferInitData.pSysMem = new VertexID[36];
	for (int i = 0; i < 36; i++)
	{
		((VertexID*)vertexBufferInitData.pSysMem)[i].id = i;
	}
	hresult = create_vertex_buffer(device, &vertexBufferInitData, sizeof(UINT) * 36, &vertex_buffer[VERTEX_BUFFER::BOX_VERTEX]);
	delete[] vertexBufferInitData.pSysMem;
	DX::ThrowIfFailed(hresult);

	D3D11_SUBRESOURCE_DATA worldViewProjConstBuffer;
	worldViewProjConstBuffer.pSysMem = new XMFLOAT4X4[3];
	((XMFLOAT4X4*)worldViewProjConstBuffer.pSysMem)[0] = IDENTITY4X4;
	((XMFLOAT4X4*)worldViewProjConstBuffer.pSysMem)[1] = IDENTITY4X4;
	((XMFLOAT4X4*)worldViewProjConstBuffer.pSysMem)[2] = IDENTITY4X4;
	hresult = create_dynamic_constant_buffer(&worldViewProjConstBuffer, device, sizeof(XMFLOAT4X4) * 3, &constant_buffer[CONSTANT_BUFFER::MVP]);
	delete[] worldViewProjConstBuffer.pSysMem;
	DX::ThrowIfFailed(hresult);
	//------------------------------------------------------------------------------------------------
	hresult = create_vertex_shader_from_cso("Shaders/Compiled/vs_color.cso", VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, device, &vertex_shader[VERTEX_SHADER::COLORED_VERTEX], &input_layout[INPUT_LAYOUT::COLORED_VERTEX]);
	DX::ThrowIfFailed(hresult);
	hresult = create_pixel_shader_from_cso("Shaders/Compiled/ps_color.cso", device, &pixel_shader[PIXEL_SHADER::COLORED_VERTEX]);
	DX::ThrowIfFailed(hresult);
	hresult = create_dynamic_vertex_buffer(device, nullptr, debug_renderer::get_line_vert_capacity()*sizeof(VertexPositionColor), &vertex_buffer[VERTEX_BUFFER::COLORED_VERTEX]);
	DX::ThrowIfFailed(hresult);
	//------------------------------------------------------------------------------------------------
}

void end::renderer::impl::CreateWindowSizeDependentResources(end::view_t& view)
{
	view.viewport_size = m_deviceResources->GetOutputSize();
	view.aspectRatio = float(view.viewport_size.right) / float(view.viewport_size.bottom);
	view.fovAngleY = 70.0f * DirectX::XM_PI / 180.0f;

	// This is a simple example of change that can be made when the app is in
	// portrait or snapped view.
	if (view.aspectRatio < 1.0f)
	{
		view.fovAngleY *= 2.0f;
	}

	// This sample makes use of a right-handed coordinate system using row-major matrices.
	view.m_projection = DirectX::XMMatrixPerspectiveFovLH(
		view.fovAngleY,
		view.aspectRatio,
		view.fNear,
		view.fFar
	);

	// initialize debug frustum values

	//Hnear = 2 * tan(fov / 2) * nearDist
	//Wnear = Hnear * viewRatio
	//Hfar = 2 * tan(fov / 2) * farDist
	//Wfar = Hfar * viewRatio
	debug_frustum.debug_near = 1;
	debug_frustum.debug_far = 4;
	
	debug_frustum.Height_Near = 2 * tan(view.fovAngleY / 2) * debug_frustum.debug_near;
	debug_frustum.Width_Near = debug_frustum.Height_Near * view.aspectRatio;

	debug_frustum.Height_Far = 2 * tan(view.fovAngleY / 2) * debug_frustum.debug_far;
	debug_frustum.Width_Far = debug_frustum.Height_Far *  view.aspectRatio;

	
	debug_frustum.Near.TL = XMVectorSet( -debug_frustum.Width_Near / 2.0f, debug_frustum.Height_Near / 2, debug_frustum.debug_near, 1);
	debug_frustum.Near.TR = XMVectorSet(debug_frustum.Width_Near / 2, debug_frustum.Height_Near / 2, debug_frustum.debug_near, 1);
	debug_frustum.Near.BL = XMVectorSet(-debug_frustum.Width_Near / 2, -debug_frustum.Height_Near / 2, debug_frustum.debug_near, 1);
	debug_frustum.Near.BR = XMVectorSet(debug_frustum.Width_Near / 2, -debug_frustum.Height_Near / 2, debug_frustum.debug_near, 1);

	debug_frustum.Far.TL = { -debug_frustum.Width_Far / 2, debug_frustum.Height_Far / 2, debug_frustum.debug_far };
	debug_frustum.Far.TR = { debug_frustum.Width_Far / 2, debug_frustum.Height_Far / 2, debug_frustum.debug_far };
	debug_frustum.Far.BL = { -debug_frustum.Width_Far / 2, -debug_frustum.Height_Far / 2, debug_frustum.debug_far };
	debug_frustum.Far.BR = { debug_frustum.Width_Far / 2, -debug_frustum.Height_Far / 2, debug_frustum.debug_far };
}

void end::renderer::impl::Update(view_t& v)
{
	m_deltaTime = m_timer.Delta() * !m_paused;
	m_totalTime += m_deltaTime;

	for (emitter& e : m_emitters)
	{
		e.spawn_timer += m_deltaTime;
		if (e.spawn_timer > m_particleSpawnInterval)
		{
			e.spawn_timer -= m_particleSpawnInterval;
			uint16_t index = e.indices.alloc();
			uint16_t index1 = e.indices[index] = m_shared_pool.alloc();
			uint16_t index2 = m_particle_timers.alloc();
			m_particle_timers[index2].timer = 0.f;
			m_particle_timers[index2].accel = { 0, -8, 0 };
			m_particle_timers[index2].velocity = { randfRange(-2.f,2.f), 15, randfRange(-2.f,2.f) };
			m_shared_pool[e.indices[index]].pos = e.StartVertex.position;
			m_shared_pool[e.indices[index]].color = e.StartVertex.color;
			m_shared_pool[e.indices[index]].prev_pos = e.StartVertex.position;
			m_shared_pool[e.indices[index]].prev_color = e.StartVertex.color;
		}

		for (size_t i = 0; i < e.indices.size(); i++)
		{
			m_particle_timers[e.indices[i]].timer += m_deltaTime;
			if (m_particle_timers[e.indices[i]].timer > m_particleLifeTime)
			{
				m_particle_timers.free(e.indices[i]);
				m_shared_pool.free(e.indices[i]);
				e.indices.free(i);
			}
		}

		for (size_t i = 0; i < e.indices.size(); i++)
		{
			XMVECTOR currentPosition;

			XMVECTOR AccelerationTSquared = m_particle_timers[e.indices[i]].accel * (m_particle_timers[e.indices[i]].timer*m_particle_timers[e.indices[i]].timer);
			currentPosition =
				/* acceleration*(t^2)	*/		(AccelerationTSquared)+
				/* velocity*t			*/		(m_particle_timers[e.indices[i]].velocity*m_particle_timers[e.indices[i]].timer) +
				/* initialPosition		*/		DirectX::XMLoadFloat3(&e.StartVertex.position);

			XMVECTOR AverageVelocity = (XMVectorSubtract(currentPosition, DirectX::XMLoadFloat3(&m_shared_pool[e.indices[i]].prev_pos))) / m_deltaTime;

			XMFLOAT3 averageVelocityMagnitude;
			DirectX::XMStoreFloat3(&averageVelocityMagnitude, XMVector3Length(AverageVelocity));

			// get a scaled normal vector from A to B
			/*Normalize(PosA - prevPos	*/
			XMVECTOR trailingPoint = (XMVector3Normalize(XMVectorSubtract(DirectX::XMLoadFloat3(&m_shared_pool[e.indices[i]].prev_pos), currentPosition))) * .08f * averageVelocityMagnitude.x;
			// Add the scaled A relative vector to A in order to get the new position
			trailingPoint = trailingPoint + currentPosition;
			// Store the final result


			XMFLOAT3 A;
			XMFLOAT3 B;
			DirectX::XMStoreFloat3(&A, currentPosition);
			DirectX::XMStoreFloat3(&B, trailingPoint);

			debug_renderer::add_line(
				A,
				B,
				m_shared_pool[e.indices[i]].color, m_shared_pool[e.indices[i]].prev_color
			);

			m_shared_pool[e.indices[i]].prev_pos = A;
		}
	}

	const auto& keyState = m_keyboard->GetState();
	static float camera_speed = 5.f;
	static float look_speed = 5.f * .001f;

	if (keyState.LeftShift || keyState.RightShift)
		camera_speed = 20.f;
	else
		camera_speed = 5.f;
	if (keyState.W)
		v.m_view.MoveOnForwardNormal(m_deltaTime*camera_speed);
	if (keyState.A)
		v.m_view.MoveOnRightNormal(-m_deltaTime * camera_speed);
	if (keyState.S)
		v.m_view.MoveOnForwardNormal(-m_deltaTime * camera_speed);
	if (keyState.D)
		v.m_view.MoveOnRightNormal(m_deltaTime*camera_speed);
	if (keyState.E)
		v.m_view.MoveOnGlobalUp(-m_deltaTime * camera_speed);
	if (keyState.Q)
		v.m_view.MoveOnGlobalUp(m_deltaTime*camera_speed);
	static int i = 0;
	if (keyState.D1)
		i = 0;
	if (keyState.D2)
		i = 1;

	const auto& mouseState = m_mouse->GetState();
	v.m_view.RotateOnGlobalUp(mouseState.x *look_speed);
	v.m_view.RotateOnRightNormal(mouseState.y *look_speed);

	Transform& t = m_transforms[i];
	if (keyState.LeftShift || keyState.RightShift)
	{
		if (keyState.Up)
			t.RotateOnRightNormal(-m_deltaTime);
		if (keyState.Left)
			t.RotateOnGlobalUp(-m_deltaTime);
		if (keyState.Down)
			t.RotateOnRightNormal(m_deltaTime);
		if (keyState.Right)
			t.RotateOnGlobalUp(m_deltaTime);
	}
	else
	{
		if (keyState.Up)
			t.Position += Transform::FORWARD * m_deltaTime * 10;
		if (keyState.Left)
			t.Position += Transform::RIGHT * -m_deltaTime * 10;
		if (keyState.Down)
			t.Position += Transform::FORWARD * -m_deltaTime * 10;
		if (keyState.Right)
			t.Position += Transform::RIGHT * m_deltaTime * 10;
		if (keyState.PageUp)
			t.Position += Transform::UP * m_deltaTime * 10;
		if (keyState.PageDown)
			t.Position += Transform::UP * -m_deltaTime * 10;
	}


	m_transforms[1].LookAt(m_transforms[0].Position);
	m_transforms[2].TurnTo(m_transforms[0].Position, m_deltaTime);
	if (keyState.Home)
		t.RotateOnGlobalUp(m_deltaTime);
	if (keyState.End)
		t.RotateOnGlobalUp(-m_deltaTime);
	// draw transforms on debug renderer
	for (Transform&t : m_transforms)
	{
		XMFLOAT3 p1;
		XMFLOAT3 p2;
		DirectX::XMStoreFloat3(&p1, t.Position);

		DirectX::XMStoreFloat3(&p2, t.Position + XMVector4Transform(Transform::UP, XMMatrixRotationQuaternion(t.Rotation)) * 2);
		debug_renderer::add_line(p1, p2, { 0,1,0,1 });

		DirectX::XMStoreFloat3(&p2, t.Position + XMVector4Transform(Transform::RIGHT, XMMatrixRotationQuaternion(t.Rotation)) * 2);
		debug_renderer::add_line(p1, p2, { 1,0,0,1 });

		DirectX::XMStoreFloat3(&p2, t.Position + XMVector4Transform(Transform::FORWARD, XMMatrixRotationQuaternion(t.Rotation)) * 2);
		debug_renderer::add_line(p1, p2, { 0,0,1,1 });
	}

	m_timer.Signal();
}
void end::renderer::impl::Render(view_t& v)
{
	// TODO:
	// Apply view properties
	//	Set and clear render targets
	//	Set viewport(s), etc
	auto context = m_deviceResources->GetD3DDeviceContext();

	Clear();
	context->OMSetDepthStencilState(depthStencilState[STATE_DEPTH_STENCIL::DEFAULT], 1);
	//context->RSSetState(m_states->CullClockwise());
	m_deviceResources->PIXBeginEvent(L"Render");
	// **SKIP**:
	// Draw batches in visible set (Implemented In a future assignment)


	// TODO:
	// Using the vs_cube/ps_cube shaders, Draw 36 vertices to render a cube.
	// The vs_cube shader contains all the vertex data, so no vertex buffers are required
	D3D11_MAPPED_SUBRESOURCE subresource_data;
	UINT vertexStride;
	UINT vertexOffset;

	context->Map(constant_buffer[CONSTANT_BUFFER::MVP], 0, D3D11_MAP_WRITE_DISCARD, NULL, &subresource_data);
	ZeroMemory(subresource_data.pData, sizeof(XMFLOAT4X4) * 3);
	DirectX::XMStoreFloat4x4(((XMFLOAT4X4*)subresource_data.pData), XMMatrixTranspose(IDENTITY4X4));
	DirectX::XMStoreFloat4x4(((XMFLOAT4X4*)subresource_data.pData + 1), XMMatrixTranspose(XMMatrixInverse(NULL, v.m_view.GetMatrix())));
	DirectX::XMStoreFloat4x4(((XMFLOAT4X4*)subresource_data.pData + 2), XMMatrixTranspose(v.m_projection));
	context->Unmap(constant_buffer[CONSTANT_BUFFER::MVP], 0);
	context->VSSetConstantBuffers(0, 1, &constant_buffer[CONSTANT_BUFFER::MVP]);
	//---------------------------------------------------------------------------------------------------------
	m_deviceResources->PIXBeginEvent(L"Terrain");
	//---------------------------------------------------------------------------------------------------------
	vertexStride = sizeof(VertexPositionNormalTexture);
	int test = sizeof(float3) + sizeof(float3) + sizeof(float2);
	vertexOffset = 0;
	context->IASetInputLayout(input_layout[INPUT_LAYOUT::DEBUG_VERTEX]);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->VSSetShader(vertex_shader[VERTEX_SHADER::DEBUG_VERTEX], nullptr, 0);
	context->PSSetShader(pixel_shader[PIXEL_SHADER::DEBUG_VERTEX], nullptr, 0);

	context->IASetVertexBuffers(0, 1, &vertex_buffer[VERTEX_BUFFER::DEBUG_VERTEX], &vertexStride, &vertexOffset);
	context->Draw(m_num_terrain_vertices, 0);
	//---------------------------------------------------------------------------------------------------------
	m_deviceResources->PIXEndEvent();
	m_deviceResources->PIXBeginEvent(L"Cube");	
	//---------------------------------------------------------------------------------------------------------
	vertexStride = sizeof(UINT);
	vertexOffset = 0;
	context->IASetInputLayout(input_layout[INPUT_LAYOUT::BOX_VERTEX]);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->VSSetShader(vertex_shader[VERTEX_SHADER::BOX_VERTEX], nullptr, 0);
	context->PSSetShader(pixel_shader[PIXEL_SHADER::BOX_VERTEX], nullptr, 0);

	context->IASetVertexBuffers(0, 1, &vertex_buffer[VERTEX_BUFFER::BOX_VERTEX], &vertexStride, &vertexOffset);
	context->Draw(36, 0);
	//---------------------------------------------------------------------------------------------------------
	m_deviceResources->PIXEndEvent();			
	m_deviceResources->PIXBeginEvent(L"Debug Renderer");
	//---------------------------------------------------------------------------------------------------------
	const XMVECTORF32 xaxis = { 5.f,	0.f,	0.f };
	const XMVECTORF32 yaxis = { 0.f,	0.f,	5.f };

	DrawGrid(xaxis, yaxis, g_XMZero, 20, 20, { .3f*(sinf(m_totalTime) + 1) / 2,0,.3f*(cos(m_totalTime) + 1) / 2 }, v);


	DebugFrustum temp_frustum;
	temp_frustum.Near.TL = XMVector3Transform(debug_frustum.Near.TL, m_transforms[0].GetMatrix());
	temp_frustum.Near.TR = XMVector3Transform(debug_frustum.Near.TR, m_transforms[0].GetMatrix());
	temp_frustum.Near.BL = XMVector3Transform(debug_frustum.Near.BL, m_transforms[0].GetMatrix());
	temp_frustum.Near.BR = XMVector3Transform(debug_frustum.Near.BR, m_transforms[0].GetMatrix());

	temp_frustum.Far.TL = XMVector3Transform(debug_frustum.Far.TL, m_transforms[0].GetMatrix());
	temp_frustum.Far.TR = XMVector3Transform(debug_frustum.Far.TR, m_transforms[0].GetMatrix());
	temp_frustum.Far.BL = XMVector3Transform(debug_frustum.Far.BL, m_transforms[0].GetMatrix());
	temp_frustum.Far.BR = XMVector3Transform(debug_frustum.Far.BR, m_transforms[0].GetMatrix());
	// draw debug frustum
	{
		XMFLOAT3 p1;
		XMFLOAT3 p2;
		// Near
		DirectX::XMStoreFloat3(&p1, temp_frustum.Near.BL);
		DirectX::XMStoreFloat3(&p2, temp_frustum.Near.BR);
		debug_renderer::add_line(p1, p2, { 1,1,0,1 });

		DirectX::XMStoreFloat3(&p1, temp_frustum.Near.TL);
		DirectX::XMStoreFloat3(&p2, temp_frustum.Near.TR);
		debug_renderer::add_line(p1, p2, { 1,1,0,1 });

		DirectX::XMStoreFloat3(&p1, temp_frustum.Near.BL);
		DirectX::XMStoreFloat3(&p2, temp_frustum.Near.TL);
		debug_renderer::add_line(p1, p2, { 1,1,0,1 });

		DirectX::XMStoreFloat3(&p1, temp_frustum.Near.BR);
		DirectX::XMStoreFloat3(&p2, temp_frustum.Near.TR);
		debug_renderer::add_line(p1, p2, { 1,1,0,1 });
		// Far
		DirectX::XMStoreFloat3(&p1, temp_frustum.Far.BL);
		DirectX::XMStoreFloat3(&p2, temp_frustum.Far.BR);
		debug_renderer::add_line(p1, p2, { 1,1,0,1 });

		DirectX::XMStoreFloat3(&p1, temp_frustum.Far.TL);
		DirectX::XMStoreFloat3(&p2, temp_frustum.Far.TR);
		debug_renderer::add_line(p1, p2, { 1,1,0,1 });

		DirectX::XMStoreFloat3(&p1, temp_frustum.Far.BL);
		DirectX::XMStoreFloat3(&p2, temp_frustum.Far.TL);
		debug_renderer::add_line(p1, p2, { 1,1,0,1 });

		DirectX::XMStoreFloat3(&p1, temp_frustum.Far.BR);
		DirectX::XMStoreFloat3(&p2, temp_frustum.Far.TR);
		debug_renderer::add_line(p1, p2, { 1,1,0,1 });
		// Near to Far
		DirectX::XMStoreFloat3(&p1, temp_frustum.Far.BL);
		DirectX::XMStoreFloat3(&p2, temp_frustum.Near.BL);
		debug_renderer::add_line(p1, p2, { 1,1,0,1 });

		DirectX::XMStoreFloat3(&p1, temp_frustum.Far.BR);
		DirectX::XMStoreFloat3(&p2, temp_frustum.Near.BR);
		debug_renderer::add_line(p1, p2, { 1,1,0,1 });

		DirectX::XMStoreFloat3(&p1, temp_frustum.Far.TL);
		DirectX::XMStoreFloat3(&p2, temp_frustum.Near.TL);
		debug_renderer::add_line(p1, p2, { 1,1,0,1 });

		DirectX::XMStoreFloat3(&p1, temp_frustum.Far.TR);
		DirectX::XMStoreFloat3(&p2, temp_frustum.Near.TR);
		debug_renderer::add_line(p1, p2, { 1,1,0,1 });
	}

	//for (auto& a : m_AABBs)
	//{
	//	a.render(temp_frustum);
	//}

	m_bvh.render(temp_frustum, m_pos);
	

	vertexStride = sizeof(VertexPositionColor);
	vertexOffset = 0;
	context->IASetVertexBuffers(0, 1, &vertex_buffer[VERTEX_BUFFER::COLORED_VERTEX], &vertexStride, &vertexOffset);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	context->IASetInputLayout(input_layout[INPUT_LAYOUT::COLORED_VERTEX]);
	context->VSSetShader(vertex_shader[VERTEX_SHADER::COLORED_VERTEX], nullptr, 0);
	context->PSSetShader(pixel_shader[PIXEL_SHADER::COLORED_VERTEX], nullptr, 0);

	subresource_data;
	context->Map(vertex_buffer[VERTEX_BUFFER::COLORED_VERTEX], 0, D3D11_MAP_WRITE_DISCARD, NULL, &subresource_data);
	ZeroMemory(subresource_data.pData, sizeof(VertexPositionColor)*debug_renderer::get_line_vert_count());
	auto line_verts = debug_renderer::get_line_verts();
	for (int i = 0; i < debug_renderer::get_line_vert_count(); i++)
	{
		((VertexPositionColor*)subresource_data.pData)[i].position = line_verts[i].position;
		((VertexPositionColor*)subresource_data.pData)[i].color = line_verts[i].color;
	}
	context->Unmap(vertex_buffer[VERTEX_BUFFER::COLORED_VERTEX], 0);

	context->Map(constant_buffer[CONSTANT_BUFFER::MVP], 0, D3D11_MAP_WRITE_DISCARD, NULL, &subresource_data);
	ZeroMemory(subresource_data.pData, sizeof(XMFLOAT4X4) * 3);
	DirectX::XMStoreFloat4x4(((XMFLOAT4X4*)subresource_data.pData), XMMatrixTranspose(XMMatrixTranslation(0, -.5f, 0)));
	DirectX::XMStoreFloat4x4(((XMFLOAT4X4*)subresource_data.pData + 1), XMMatrixTranspose(XMMatrixInverse(NULL, v.m_view.GetMatrix())));
	DirectX::XMStoreFloat4x4(((XMFLOAT4X4*)subresource_data.pData + 2), XMMatrixTranspose(v.m_projection));
	context->Unmap(constant_buffer[CONSTANT_BUFFER::MVP], 0);

	context->Draw(debug_renderer::get_line_vert_count(), 0);
	debug_renderer::clear_lines();
	//---------------------------------------------------------------------------------------------------------
	m_deviceResources->PIXEndEvent();
	//---------------------------------------------------------------------------------------------------------

	m_deviceResources->PIXEndEvent();
	m_deviceResources->Present();
}

void  end::renderer::impl::Clear()
{
	m_deviceResources->PIXBeginEvent(L"Clear");

	// Clear the views.
	auto context = m_deviceResources->GetD3DDeviceContext();
	auto renderTarget = m_deviceResources->GetRenderTargetView();
	auto depthStencil = m_deviceResources->GetDepthStencilView();

	context->ClearRenderTargetView(renderTarget, DirectX::Colors::DimGray);
	context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetRenderTargets(1, &renderTarget, depthStencil);

	// Set the viewport.
	auto viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	m_deviceResources->PIXEndEvent();
}

void end::renderer::impl::OnActivated()
{
	m_paused = false;
}

void end::renderer::impl::OnDeactivated()
{
	m_paused = true;
}

void end::renderer::impl::OnSuspending()
{
	m_paused = false;
}

void end::renderer::impl::OnResuming()
{
	m_paused = false;
}

void end::renderer::impl::OnWindowMoved()
{
	auto r = m_deviceResources->GetOutputSize();
	m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}

void  end::renderer::impl::OnWindowSizeChanged(end::view_t& view)
{
	if (!m_deviceResources->WindowSizeChanged(view.width, view.height))
		return;

	CreateWindowSizeDependentResources(view);
}


end::renderer::impl::~impl()
{
	COMRELEASE_ALL(depthStencilState);
	COMRELEASE_ALL(vertex_buffer);
	COMRELEASE_ALL(index_buffer);
	COMRELEASE_ALL(input_layout);
	COMRELEASE_ALL(vertex_shader);
	COMRELEASE_ALL(constant_buffer);
	COMRELEASE_ALL(pixel_shader);
	COMRELEASE_ALL(rasterState);
}


void end::renderer::impl::OnDeviceLost()
{

}

void end::renderer::impl::OnDeviceRestored(end::view_t& view)
{
	CreateDeviceDependentResources();

	CreateWindowSizeDependentResources(view);
}

// draw grid function based on directXTK
void XM_CALLCONV end::renderer::impl::DrawGrid(FXMVECTOR xAxis, FXMVECTOR yAxis, FXMVECTOR origin, size_t xdivs, size_t ydivs, GXMVECTOR color, view_t& v)
{
	auto context = m_deviceResources->GetD3DDeviceContext();

	xdivs = std::max<size_t>(1, xdivs);
	ydivs = std::max<size_t>(1, ydivs);

	for (size_t i = 0; i <= xdivs; ++i)
	{
		float fPercent = float(i) / float(xdivs);
		fPercent = (fPercent * 2.0f) - 1.0f;
		XMVECTOR vScale = XMVectorScale(xAxis, fPercent);
		vScale = XMVectorAdd(vScale, origin);

		VertexPositionColor v1(XMVectorSubtract(vScale, yAxis), color);
		VertexPositionColor v2(XMVectorAdd(vScale, yAxis), color);
		debug_renderer::add_line(v1, v2);
	}

	for (size_t i = 0; i <= ydivs; i++)
	{
		float fPercent = float(i) / float(ydivs);
		fPercent = (fPercent * 2.0f) - 1.0f;
		XMVECTOR vScale = XMVectorScale(yAxis, fPercent);
		vScale = XMVectorAdd(vScale, origin);

		VertexPositionColor v1(XMVectorSubtract(vScale, xAxis), color);
		VertexPositionColor v2(XMVectorAdd(vScale, xAxis), color);
		debug_renderer::add_line(v1, v2);
	}
}


