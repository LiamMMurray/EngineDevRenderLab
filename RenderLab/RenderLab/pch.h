//
// pch.h
// Header for standard system include files.
//

#pragma once

#include <WinSDKVer.h>
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#include <SDKDDKVer.h>

// Use the C++ standard templated min/max
#define NOMINMAX

// DirectX apps don't need GDI
#define NODRAWTEXT
#define NOGDI
#define NOBITMAP

// Include <mcx.h> if you need this
#define NOMCX

// Include <winsvc.h> if you need this
#define NOSERVICE

// WinHelp is deprecated
#define NOHELP

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <wrl/client.h>

#include <d3d11_1.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DXGI.lib")

#include <algorithm>
#include <exception>
#include <memory>
#include <stdexcept>

#include <stdio.h>

#include "Keyboard.h"
#include "Mouse.h"
#include "StepTimer.h"
#include "CommonStates.h"
#include "VertexTypes.h"
#include "PlatformHelpers.h"

namespace DX
{
	// Helper class for COM exceptions
	class com_exception : public std::exception
	{
	public:
		com_exception(HRESULT hr) : result(hr) {}

		virtual const char* what() const override
		{
			static char s_str[64] = {};
			sprintf_s(s_str, "Failure with HRESULT of %08X", static_cast<unsigned int>(result));
			return s_str;
		}

	private:
		HRESULT result;
	};

	// Helper utility converts D3D API failures into exceptions.
	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			throw com_exception(hr);
		}
	}
}

// above is the code included in the DirectXTK pch
// below are things I added 

#pragma region User helper functions
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#define IDENTITY4X4			\
{							\
	1.f, 0.f, 0.f, 0.f,		\
	0.f, 1.f, 0.f, 0.f,		\
	0.f, 0.f, 1.f, 0.f,		\
	0.f, 0.f, 0.f, 1.f		\
}

#define REQUIRE_SEMICOLON	\
enum{}

#define COMRELEASE(X)		\
if (X != nullptr)			\
{							\
	X->Release();			\
	X = nullptr;			\
} REQUIRE_SEMICOLON
							
#define COMRELEASE_ALL(X)	\
for (auto c : X)			\
{							\
	COMRELEASE(c);			\
} REQUIRE_SEMICOLON

static inline float randf()
{
	return (float)rand() / (float)RAND_MAX;
}
static inline float randfRange(float min, float max)
{
	return min + (max - min) * randf();
}
#pragma endregion