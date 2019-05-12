#include "pch.h"
#include "renderer.h"

#ifdef FSGD_END_USE_D3D
#include "d3d11_renderer_impl.h"
#endif

namespace end
{
	void renderer::Initialize(native_handle_type window_handle, int width, int height)
	{
		default_view.width = width;
		default_view.height = height;
		m_pImpl = new impl();
		m_pImpl->Initialize(window_handle, default_view);
	}

	renderer::~renderer()
	{
		delete m_pImpl;
	};
	void renderer::Update()
	{
		m_pImpl->Update(default_view);
	}
	void renderer::render()
	{
		// draw views...
		m_pImpl->Render(default_view);
		// draw views...
		// draw views...
	}
	void renderer::OnActivated()
	{
		m_pImpl->OnActivated();
	}
	void renderer::OnDeactivated()
	{
		m_pImpl->OnDeactivated();
	}
	void renderer::OnSuspending()
	{
		m_pImpl->OnSuspending();
	}
	void renderer::OnResuming()
	{
		m_pImpl->OnResuming();
	}
	void renderer::OnWindowMoved()
	{
		m_pImpl->OnWindowMoved();
	}
	void renderer::OnWindowSizeChanged(int width, int height)
	{
		default_view.width = width;
		default_view.height = height;
		m_pImpl->OnWindowSizeChanged(default_view);
	}
	void renderer::GetDefaultSize(int& width, int& height) const
	{
		width = 800;
		height = 600;
	}
}