//--------------------------------------------------------------------------------------
// File: VertexTypes.h
//
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkId=248929
//--------------------------------------------------------------------------------------

#pragma once

#if defined(_XBOX_ONE) && defined(_TITLE)
#include <d3d11_x.h>
#else
#include <d3d11_1.h>
#endif

#include <DirectXMath.h>


namespace DirectX
{

	// Vertex struct holding position information.
	struct VertexID
	{
		VertexID() = default;

		VertexID(const VertexID&) = default;
		VertexID& operator=(const VertexID&) = default;

		VertexID(VertexID&&) = default;
		VertexID& operator=(VertexID&&) = default;

		VertexID(UINT const& id)
			: id(id)
		{ }

		VertexID(UINT id)
		{
			VertexID::id = id;
		}

		UINT id;

		static const int InputElementCount = 1;
		static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
	};
    // Vertex struct holding position information.
    struct VertexPosition
    {
        VertexPosition() = default;

        VertexPosition(const VertexPosition&) = default;
        VertexPosition& operator=(const VertexPosition&) = default;

        VertexPosition(VertexPosition&&) = default;
        VertexPosition& operator=(VertexPosition&&) = default;

        VertexPosition(XMFLOAT3 const& position)
            : position(position)
        { }

        VertexPosition(FXMVECTOR position)
        {
            XMStoreFloat3(&this->position, position);
        }

        XMFLOAT3 position;

        static const int InputElementCount = 1;
        static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
    };


    // Vertex struct holding position and color information.
    struct VertexPositionColor
    {
        VertexPositionColor() = default;

        VertexPositionColor(const VertexPositionColor&) = default;
        VertexPositionColor& operator=(const VertexPositionColor&) = default;

        VertexPositionColor(VertexPositionColor&&) = default;
        VertexPositionColor& operator=(VertexPositionColor&&) = default;

        VertexPositionColor(XMFLOAT3 const& position, XMFLOAT4 const& color)
            : position(position),
            color(color)
        { }

        VertexPositionColor(FXMVECTOR position, FXMVECTOR color)
        {
            XMStoreFloat3(&this->position, position);
            XMStoreFloat4(&this->color, color);
        }

        XMFLOAT3 position;
        XMFLOAT4 color;

        static const int InputElementCount = 2;
        static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
    };


    // Vertex struct holding position and texture mapping information.
    struct VertexPositionTexture
    {
        VertexPositionTexture() = default;

        VertexPositionTexture(const VertexPositionTexture&) = default;
        VertexPositionTexture& operator=(const VertexPositionTexture&) = default;

        VertexPositionTexture(VertexPositionTexture&&) = default;
        VertexPositionTexture& operator=(VertexPositionTexture&&) = default;

        VertexPositionTexture(XMFLOAT3 const& position, XMFLOAT2 const& textureCoordinate)
            : position(position),
            textureCoordinate(textureCoordinate)
        { }

        VertexPositionTexture(FXMVECTOR position, FXMVECTOR textureCoordinate)
        {
            XMStoreFloat3(&this->position, position);
            XMStoreFloat2(&this->textureCoordinate, textureCoordinate);
        }

        XMFLOAT3 position;
        XMFLOAT2 textureCoordinate;

        static const int InputElementCount = 2;
        static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
    };


    // Vertex struct holding position and dual texture mapping information.
    struct VertexPositionDualTexture
    {
        VertexPositionDualTexture() = default;

        VertexPositionDualTexture(const VertexPositionDualTexture&) = default;
        VertexPositionDualTexture& operator=(const VertexPositionDualTexture&) = default;

        VertexPositionDualTexture(VertexPositionDualTexture&&) = default;
        VertexPositionDualTexture& operator=(VertexPositionDualTexture&&) = default;

        VertexPositionDualTexture(XMFLOAT3 const& position, XMFLOAT2 const& textureCoordinate0, XMFLOAT2 const& textureCoordinate1)
            : position(position),
            textureCoordinate0(textureCoordinate0),
            textureCoordinate1(textureCoordinate1)
        { }

        VertexPositionDualTexture(FXMVECTOR position,
                                  FXMVECTOR textureCoordinate0,
                                  FXMVECTOR textureCoordinate1)
        {
            XMStoreFloat3(&this->position, position);
            XMStoreFloat2(&this->textureCoordinate0, textureCoordinate0);
            XMStoreFloat2(&this->textureCoordinate1, textureCoordinate1);
        }

        XMFLOAT3 position;
        XMFLOAT2 textureCoordinate0;
        XMFLOAT2 textureCoordinate1;

        static const int InputElementCount = 3;
        static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
    };


    // Vertex struct holding position and normal vector.
    struct VertexPositionNormal
    {
        VertexPositionNormal() = default;

        VertexPositionNormal(const VertexPositionNormal&) = default;
        VertexPositionNormal& operator=(const VertexPositionNormal&) = default;

        VertexPositionNormal(VertexPositionNormal&&) = default;
        VertexPositionNormal& operator=(VertexPositionNormal&&) = default;

        VertexPositionNormal(XMFLOAT3 const& position, XMFLOAT3 const& normal)
            : position(position),
            normal(normal)
        { }

        VertexPositionNormal(FXMVECTOR position, FXMVECTOR normal)
        {
            XMStoreFloat3(&this->position, position);
            XMStoreFloat3(&this->normal, normal);
        }

        XMFLOAT3 position;
        XMFLOAT3 normal;

        static const int InputElementCount = 2;
        static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
    };


    // Vertex struct holding position, color, and texture mapping information.
    struct VertexPositionColorTexture
    {
        VertexPositionColorTexture() = default;

        VertexPositionColorTexture(const VertexPositionColorTexture&) = default;
        VertexPositionColorTexture& operator=(const VertexPositionColorTexture&) = default;

        VertexPositionColorTexture(VertexPositionColorTexture&&) = default;
        VertexPositionColorTexture& operator=(VertexPositionColorTexture&&) = default;

        VertexPositionColorTexture(XMFLOAT3 const& position, XMFLOAT4 const& color, XMFLOAT2 const& textureCoordinate)
            : position(position),
            color(color),
            textureCoordinate(textureCoordinate)
        { }

        VertexPositionColorTexture(FXMVECTOR position, FXMVECTOR color, FXMVECTOR textureCoordinate)
        {
            XMStoreFloat3(&this->position, position);
            XMStoreFloat4(&this->color, color);
            XMStoreFloat2(&this->textureCoordinate, textureCoordinate);
        }

        XMFLOAT3 position;
        XMFLOAT4 color;
        XMFLOAT2 textureCoordinate;

        static const int InputElementCount = 3;
        static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
    };


    // Vertex struct holding position, normal vector, and color information.
    struct VertexPositionNormalColor
    {
        VertexPositionNormalColor() = default;

        VertexPositionNormalColor(const VertexPositionNormalColor&) = default;
        VertexPositionNormalColor& operator=(const VertexPositionNormalColor&) = default;

        VertexPositionNormalColor(VertexPositionNormalColor&&) = default;
        VertexPositionNormalColor& operator=(VertexPositionNormalColor&&) = default;

        VertexPositionNormalColor(XMFLOAT3 const& position, XMFLOAT3 const& normal, XMFLOAT4 const& color)
            : position(position),
            normal(normal),
            color(color)
        { }

        VertexPositionNormalColor(FXMVECTOR position, FXMVECTOR normal, FXMVECTOR color)
        {
            XMStoreFloat3(&this->position, position);
            XMStoreFloat3(&this->normal, normal);
            XMStoreFloat4(&this->color, color);
        }

        XMFLOAT3 position;
        XMFLOAT3 normal;
        XMFLOAT4 color;

        static const int InputElementCount = 3;
        static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
    };


    // Vertex struct holding position, normal vector, and texture mapping information.
    struct VertexPositionNormalTexture
    {
        VertexPositionNormalTexture() = default;

        VertexPositionNormalTexture(const VertexPositionNormalTexture&) = default;
        VertexPositionNormalTexture& operator=(const VertexPositionNormalTexture&) = default;

        VertexPositionNormalTexture(VertexPositionNormalTexture&&) = default;
        VertexPositionNormalTexture& operator=(VertexPositionNormalTexture&&) = default;

        VertexPositionNormalTexture(XMFLOAT3 const& position, XMFLOAT3 const& normal, XMFLOAT2 const& textureCoordinate)
            : position(position),
            normal(normal),
            textureCoordinate(textureCoordinate)
        { }

        VertexPositionNormalTexture(FXMVECTOR position, FXMVECTOR normal, FXMVECTOR textureCoordinate)
        {
            XMStoreFloat3(&this->position, position);
            XMStoreFloat3(&this->normal, normal);
            XMStoreFloat2(&this->textureCoordinate, textureCoordinate);
        }

        XMFLOAT3 position;
        XMFLOAT3 normal;
        XMFLOAT2 textureCoordinate;

        static const int InputElementCount = 3;
        static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
    };


    // Vertex struct holding position, normal vector, color, and texture mapping information.
    struct VertexPositionNormalColorTexture
    {
        VertexPositionNormalColorTexture() = default;

        VertexPositionNormalColorTexture(const VertexPositionNormalColorTexture&) = default;
        VertexPositionNormalColorTexture& operator=(const VertexPositionNormalColorTexture&) = default;

        VertexPositionNormalColorTexture(VertexPositionNormalColorTexture&&) = default;
        VertexPositionNormalColorTexture& operator=(VertexPositionNormalColorTexture&&) = default;

        VertexPositionNormalColorTexture(XMFLOAT3 const& position, XMFLOAT3 const& normal, XMFLOAT4 const& color, XMFLOAT2 const& textureCoordinate)
            : position(position),
            normal(normal),
            color(color),
            textureCoordinate(textureCoordinate)
        { }

        VertexPositionNormalColorTexture(FXMVECTOR position, FXMVECTOR normal, FXMVECTOR color, CXMVECTOR textureCoordinate)
        {
            XMStoreFloat3(&this->position, position);
            XMStoreFloat3(&this->normal, normal);
            XMStoreFloat4(&this->color, color);
            XMStoreFloat2(&this->textureCoordinate, textureCoordinate);
        }

        XMFLOAT3 position;
        XMFLOAT3 normal;
        XMFLOAT4 color;
        XMFLOAT2 textureCoordinate;

        static const int InputElementCount = 4;
        static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
    };


    // Vertex struct for Visual Studio Shader Designer (DGSL) holding position, normal,
    // tangent, color (RGBA), and texture mapping information
    struct VertexPositionNormalTangentColorTexture
    {
        VertexPositionNormalTangentColorTexture() = default;

        VertexPositionNormalTangentColorTexture(const VertexPositionNormalTangentColorTexture&) = default;
        VertexPositionNormalTangentColorTexture& operator=(const VertexPositionNormalTangentColorTexture&) = default;

        VertexPositionNormalTangentColorTexture(VertexPositionNormalTangentColorTexture&&) = default;
        VertexPositionNormalTangentColorTexture& operator=(VertexPositionNormalTangentColorTexture&&) = default;

        XMFLOAT3 position;
        XMFLOAT3 normal;
        XMFLOAT4 tangent;
        uint32_t color;
        XMFLOAT2 textureCoordinate;

        VertexPositionNormalTangentColorTexture(XMFLOAT3 const& position, XMFLOAT3 const& normal, XMFLOAT4 const& tangent, uint32_t rgba, XMFLOAT2 const& textureCoordinate)
            : position(position),
            normal(normal),
            tangent(tangent),
            color(rgba),
            textureCoordinate(textureCoordinate)
        {
        }

        VertexPositionNormalTangentColorTexture(FXMVECTOR position, FXMVECTOR normal, FXMVECTOR tangent, uint32_t rgba, CXMVECTOR textureCoordinate)
            : color(rgba)
        {
            XMStoreFloat3(&this->position, position);
            XMStoreFloat3(&this->normal, normal);
            XMStoreFloat4(&this->tangent, tangent);
            XMStoreFloat2(&this->textureCoordinate, textureCoordinate);
        }

        VertexPositionNormalTangentColorTexture(XMFLOAT3 const& position, XMFLOAT3 const& normal, XMFLOAT4 const& tangent, XMFLOAT4 const& color, XMFLOAT2 const& textureCoordinate)
            : position(position),
            normal(normal),
            tangent(tangent),
            color{},
            textureCoordinate(textureCoordinate)
        {
            SetColor(color);
        }

        VertexPositionNormalTangentColorTexture(FXMVECTOR position, FXMVECTOR normal, FXMVECTOR tangent, CXMVECTOR color, CXMVECTOR textureCoordinate) :
            color{}
        {
            XMStoreFloat3(&this->position, position);
            XMStoreFloat3(&this->normal, normal);
            XMStoreFloat4(&this->tangent, tangent);
            XMStoreFloat2(&this->textureCoordinate, textureCoordinate);

            SetColor(color);
        }

        void __cdecl SetColor(XMFLOAT4 const& icolor) { SetColor(XMLoadFloat4(&icolor)); }
        void XM_CALLCONV SetColor(FXMVECTOR icolor);

        static const int InputElementCount = 5;
        static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
    };


    // Vertex struct for Visual Studio Shader Designer (DGSL) holding position, normal,
    // tangent, color (RGBA), texture mapping information, and skinning weights
    struct VertexPositionNormalTangentColorTextureSkinning : public VertexPositionNormalTangentColorTexture
    {
        VertexPositionNormalTangentColorTextureSkinning() = default;

        VertexPositionNormalTangentColorTextureSkinning(const VertexPositionNormalTangentColorTextureSkinning&) = default;
        VertexPositionNormalTangentColorTextureSkinning& operator=(const VertexPositionNormalTangentColorTextureSkinning&) = default;

        VertexPositionNormalTangentColorTextureSkinning(VertexPositionNormalTangentColorTextureSkinning&&) = default;
        VertexPositionNormalTangentColorTextureSkinning& operator=(VertexPositionNormalTangentColorTextureSkinning&&) = default;

        uint32_t indices;
        uint32_t weights;

        VertexPositionNormalTangentColorTextureSkinning(
            XMFLOAT3 const& position,
            XMFLOAT3 const& normal,
            XMFLOAT4 const& tangent,
            uint32_t rgba,
            XMFLOAT2 const& textureCoordinate,
            XMUINT4 const& indices,
            XMFLOAT4 const& weights)
            : VertexPositionNormalTangentColorTexture(position, normal, tangent, rgba, textureCoordinate),
            indices{},
            weights{}
        {
            SetBlendIndices(indices);
            SetBlendWeights(weights);
        }

        VertexPositionNormalTangentColorTextureSkinning(
            FXMVECTOR position,
            FXMVECTOR normal,
            FXMVECTOR tangent,
            uint32_t rgba,
            CXMVECTOR textureCoordinate,
            XMUINT4 const& indices,
            CXMVECTOR weights)
            : VertexPositionNormalTangentColorTexture(position, normal, tangent, rgba, textureCoordinate),
            indices{},
            weights{}
        {
            SetBlendIndices(indices);
            SetBlendWeights(weights);
        }

        VertexPositionNormalTangentColorTextureSkinning(
            XMFLOAT3 const& position,
            XMFLOAT3 const& normal,
            XMFLOAT4 const& tangent,
            XMFLOAT4 const& color,
            XMFLOAT2 const& textureCoordinate,
            XMUINT4 const& indices,
            XMFLOAT4 const& weights)
            : VertexPositionNormalTangentColorTexture(position, normal, tangent, color, textureCoordinate),
            indices{},
            weights{}
        {
            SetBlendIndices(indices);
            SetBlendWeights(weights);
        }

        VertexPositionNormalTangentColorTextureSkinning(
            FXMVECTOR position,
            FXMVECTOR normal,
            FXMVECTOR tangent,
            CXMVECTOR color,
            CXMVECTOR textureCoordinate,
            XMUINT4 const& indices,
            CXMVECTOR weights)
            : VertexPositionNormalTangentColorTexture(position, normal, tangent, color, textureCoordinate),
            indices{},
            weights{}
        {
            SetBlendIndices(indices);
            SetBlendWeights(weights);
        }

        void __cdecl SetBlendIndices(XMUINT4 const& iindices);

        void __cdecl SetBlendWeights(XMFLOAT4 const& iweights) { SetBlendWeights(XMLoadFloat4(&iweights)); }
        void XM_CALLCONV SetBlendWeights(FXMVECTOR iweights);

        static const int InputElementCount = 7;
        static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
    };
}
