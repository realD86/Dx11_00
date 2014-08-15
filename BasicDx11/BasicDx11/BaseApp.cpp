#include "stdafx.h"
#include "BaseApp.h"
#include "CommonUse.h"
#include "DeviceDx11.h"
#include "BaseResource.h"
#include "BaseShader.h"


struct SimpleVertex
{
	XMFLOAT3 Pos;
};

BaseApp::BaseApp()
: m_pBaseResource( nullptr ), m_pBaseShader( nullptr )
{
}

BaseApp::~BaseApp()
{
	CleanupApp();
}

bool BaseApp::Init( HWND hWnd )
{
	DeviceDx11& refDevice = DeviceDx11::GetInstance();

	if( refDevice.InitDevice( hWnd ) )
		return false;

	Safe_Delete( m_pBaseResource );
	m_pBaseResource = new BaseResource();
	if( m_pBaseResource )
	{
		if( !m_pBaseResource->CreateRenderTargetView( refDevice.GetDevice(), refDevice.GetSwapChain() ) 
			|| !m_pBaseResource->CreateDepthStencilView( refDevice.GetDevice(), WINDOW_WIDTH, WINDOW_HEIGHT ) )
		{
			return false;			
		}

		ID3D11RenderTargetView* pRenderTargetView = m_pBaseResource->GetRenderTargetView();
		ID3D11DepthStencilView* pDepthStencilView = m_pBaseResource->GetDepthStencilView();
		refDevice.GetDeviceContext()->OMSetRenderTargets( 1, &pRenderTargetView, pDepthStencilView );
		
		m_pBaseResource->InitViewport( WINDOW_WIDTH, WINDOW_HEIGHT );

		{
			refDevice.GetDeviceContext( )->RSSetViewports( 1, &m_pBaseResource->GetViewport() );			
		}
	}

	Safe_Delete( m_pBaseShader );
	m_pBaseShader = new BaseShader();
	if( m_pBaseShader )
	{
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT numElements = ARRAYSIZE( layout );

		{
			ID3DBlob* pVSBlob = nullptr;
			bool bResult1 = m_pBaseShader->CreateVertexShader( refDevice.GetDevice(), L"Tutorial03.fx", "VS", "vs_5_0", &pVSBlob );
			bool bResult2 = m_pBaseResource->CreateInputLayout( refDevice.GetDevice(), layout, numElements, pVSBlob );
			Safe_Release( pVSBlob );
			
			if( !bResult1 || !bResult2 )
				return false;
		}

		{
			ID3DBlob* pPSBlob = nullptr;
			bool bResult = m_pBaseShader->CreatePixelShader( refDevice.GetDevice(), L"Tutorial03.fx", "PS", "ps_5_0", &pPSBlob );
			Safe_Release( pPSBlob );

			if( !bResult )
				return false;
		}
	}	

	if( !CreateVertexBuffer() )
	{
		return false;
	}
	
	if(	!CreateIndexBuffer() )
	{
		return false;
	}
	
	return true;
}

void BaseApp::Update()
{

}

void BaseApp::Render()
{
	UINT stride = sizeof( SimpleVertex );
	UINT offset = 0;

	DeviceDx11& refDevice = DeviceDx11::GetInstance();
	ID3D11Buffer* pVertexBuffer = m_pBaseResource->GetVertexBuffer();
	refDevice.GetDeviceContext()->IASetVertexBuffers( 0, 1, &pVertexBuffer, &stride, &offset );

	ID3D11Buffer* pIndexBuffer = m_pBaseResource->GetIndexBuffer();
	refDevice.GetDeviceContext()->IASetIndexBuffer( pIndexBuffer, DXGI_FORMAT_R32_UINT, 0 );

	refDevice.GetDeviceContext( )->IASetInputLayout( m_pBaseResource->GetInputLayout() );
	refDevice.GetDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	float ClearColor[ 4 ] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red,green,blue,alpha
	refDevice.GetDeviceContext( )->ClearRenderTargetView( m_pBaseResource->GetRenderTargetView(), ClearColor );
	refDevice.GetDeviceContext( )->ClearDepthStencilView( m_pBaseResource->GetDepthStencilView( ), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );

	refDevice.GetDeviceContext()->VSSetShader( m_pBaseShader->GetVertexShader(), NULL, 0 );
	refDevice.GetDeviceContext()->PSSetShader( m_pBaseShader->GetPixelShader(), NULL, 0 );
	refDevice.GetDeviceContext( )->Draw( 3, 0 );

	refDevice.GetSwapChain()->Present( 0, 0 );
}

void BaseApp::CleanupApp()
{
	Safe_Delete( m_pBaseResource );
	Safe_Delete( m_pBaseShader );
}

bool BaseApp::CreateVertexBuffer()
{
	SimpleVertex vertices[] =
	{
		XMFLOAT3( 0.0f, 0.5f, 0.5f ),
		XMFLOAT3( 0.5f, -0.5f, 0.5f ),
		XMFLOAT3( -0.5f, -0.5f, 0.5f ),
	};

	int totalVertexSize = sizeof( vertices );
	
	if( m_pBaseResource )
	{
		DeviceDx11& refDevice = DeviceDx11::GetInstance();
		if( m_pBaseResource->CreateVertexBuffer( refDevice.GetDevice(), totalVertexSize, vertices ) )
		{
			return true;
		}
	}
	
	return false;
}

bool BaseApp::CreateIndexBuffer()
{
	UINT indices[] =
	{
		0, 1, 2
	};

	int totalIndexSize = sizeof( indices );

	if( m_pBaseResource )
	{
		DeviceDx11& refDevice = DeviceDx11::GetInstance();
		if( m_pBaseResource->CreateIndexBuffer( refDevice.GetDevice(), totalIndexSize, indices ) )
		{
			return true;
		}
	}
	
	return false;
}

