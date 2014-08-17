#include "stdafx.h"
#include "BaseResource.h"
#include "CommonUse.h"


BaseResource::BaseResource( ) 
: m_pRenderTargetView( nullptr ), m_pDepthStencilView( nullptr ), m_pDepthStencilBuffer( nullptr ),
  m_pVertexLayout( nullptr ),
  m_pVertexBuffer( nullptr ), m_pIndexBuffer( nullptr ), m_pConstantBuffer( nullptr )
{
}

BaseResource::~BaseResource()
{	
	CleanupResource();
}

bool BaseResource::CreateRenderTargetView( ID3D11Device* pDevice, IDXGISwapChain* pSwapChain )
{
	if( pDevice && pSwapChain )
	{
		ID3D11Texture2D* pBackBuffer = nullptr;
		if( SUCCEEDED( pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast< void** >( &pBackBuffer ) ) )
			&& SUCCEEDED( pDevice->CreateRenderTargetView( pBackBuffer, 0, &m_pRenderTargetView ) ) )
		{
			pBackBuffer->Release();
			return true;
		}	
	}
	return false;
}

bool BaseResource::CreateDepthStencilView( ID3D11Device* pDevice, int width, int height )
{
	if( pDevice )
	{
		D3D11_TEXTURE2D_DESC depthStencilDesc;

		depthStencilDesc.Width = width;
		depthStencilDesc.Height = height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		if( SUCCEEDED( pDevice->CreateTexture2D( &depthStencilDesc, 0, &m_pDepthStencilBuffer ) )
			&& SUCCEEDED( pDevice->CreateDepthStencilView( m_pDepthStencilBuffer, 0, &m_pDepthStencilView ) ) )
		{
			return true;
		}
	}

	return false;
}

void BaseResource::InitViewport( int width, int height )
{
	m_ScreenViewport.TopLeftX = 0;
	m_ScreenViewport.TopLeftY = 0;
	m_ScreenViewport.Width = static_cast< float >( width );
	m_ScreenViewport.Height = static_cast< float >( height );
	m_ScreenViewport.MinDepth = 0.0f;
	m_ScreenViewport.MaxDepth = 1.0f;
}

void BaseResource::CleanupResource()
{
	Safe_Release( m_pRenderTargetView );
	Safe_Release( m_pDepthStencilView );
	Safe_Release( m_pDepthStencilBuffer );
	
	Safe_Release( m_pVertexLayout );
	Safe_Release( m_pVertexBuffer );
	Safe_Release( m_pIndexBuffer );	
	Safe_Release( m_pConstantBuffer );
}

bool BaseResource::CreateInputLayout( ID3D11Device* pDevice, const D3D11_INPUT_ELEMENT_DESC* pLayout, UINT numElements, ID3DBlob* pVSBlob )
{
	if( pDevice && pVSBlob && pLayout &&
		SUCCEEDED( pDevice->CreateInputLayout( pLayout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &m_pVertexLayout ) ) )
	{
		return true;
	}	
	
	return false;
}

bool BaseResource::CreateVertexBuffer( ID3D11Device* pDevice, int totalVertexSize, void* pVertices )
{
	D3D11_BUFFER_DESC vbd;
	ZeroMemory( &vbd, sizeof( vbd ) );
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = totalVertexSize;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory( &InitData, sizeof( InitData ) );
	InitData.pSysMem = pVertices;

	HRESULT hr = pDevice->CreateBuffer( &vbd, &InitData, &m_pVertexBuffer );
	
	return SUCCEEDED( hr );	
}

bool BaseResource::CreateIndexBuffer( ID3D11Device* pDevice, int totalIndexSize, void* pIndices )
{
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = totalIndexSize;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pIndices;
	HRESULT hr = pDevice->CreateBuffer( &ibd, &InitData, &m_pIndexBuffer );
	
	return SUCCEEDED( hr );
}

bool BaseResource::CreateConstantBuffer( ID3D11Device* pDevice, int constantSize )
{
	D3D11_BUFFER_DESC cbd;
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.ByteWidth = constantSize;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = 0;
	cbd.MiscFlags = 0;
	cbd.StructureByteStride = 0;

	HRESULT hr = pDevice->CreateBuffer( &cbd, NULL, &m_pConstantBuffer );
	
	return SUCCEEDED( hr );
}
