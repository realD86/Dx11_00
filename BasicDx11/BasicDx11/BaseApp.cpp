#include "stdafx.h"
#include "BaseApp.h"
#include "CommonUse.h"
#include "DeviceDx11.h"
#include "BaseResource.h"
#include "BaseShader.h"
#include "Object.h"

namespace Colors
{
	XMGLOBALCONST XMVECTORF32 White = { 1.0f, 1.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Black = { 0.0f, 0.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Red = { 1.0f, 0.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Green = { 0.0f, 1.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Blue = { 0.0f, 0.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Yellow = { 1.0f, 1.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Cyan = { 0.0f, 1.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Magenta = { 1.0f, 0.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Silver = { 0.75f, 0.75f, 0.75f, 1.0f };
	XMGLOBALCONST XMVECTORF32 LightSteelBlue = { 0.69f, 0.77f, 0.87f, 1.0f };
	XMGLOBALCONST XMVECTORF32 DefaultColor = { 0.0f, 0.125f, 0.3f, 1.0f };
}

BaseApp::BaseApp()
	: m_pBaseResource(nullptr), m_pBaseShader(nullptr), m_pObject(nullptr)
{
	m_matView = XMMatrixIdentity();
	m_matProjection = XMMatrixIdentity();
}

BaseApp::~BaseApp()
{
	CleanupApp();
}

bool BaseApp::Init( HWND hWnd )
{
	DeviceDx11& refDevice = DeviceDx11::GetInstance();

	if( !refDevice.InitDevice( hWnd ) )
	{
		return false;
	}
	
	if( !SetUpDxResource( &refDevice ) )
	{
			return false;
			}

	if( !BuildShader( &refDevice ) )
	{
		return false;
	}

	m_pObject = new Object();
	m_pObject->Init();

	if( !CreateVertexBuffer() )
	{
		return false;
	}
	
	if(	!CreateIndexBuffer() )
	{
		return false;
	}

	if( !CreateConstantBuffer() )
	{
		return false;
	}		

	return true;
}

bool BaseApp::SetUpDxResource( DeviceDx11* pDevice )
{
	Safe_Delete( m_pBaseResource );
	m_pBaseResource = new BaseResource();
	if( pDevice && m_pBaseResource )
	{
		if( !m_pBaseResource->CreateRenderTargetView( pDevice->GetDevice(), pDevice->GetSwapChain() )
			|| !m_pBaseResource->CreateDepthStencilView( pDevice->GetDevice(), WINDOW_WIDTH, WINDOW_HEIGHT ) )
		{
			return false;
		}

		ID3D11RenderTargetView* pRenderTargetView = m_pBaseResource->GetRenderTargetView();
		ID3D11DepthStencilView* pDepthStencilView = m_pBaseResource->GetDepthStencilView();
		pDevice->GetDeviceContext()->OMSetRenderTargets( 1, &pRenderTargetView, pDepthStencilView );

		m_pBaseResource->InitViewport( WINDOW_WIDTH, WINDOW_HEIGHT );

		{
			pDevice->GetDeviceContext()->RSSetViewports( 1, &m_pBaseResource->GetViewport() );
		}
		return true;
	}

	return false;
}

bool BaseApp::BuildShader( DeviceDx11* pDevice )
{
	Safe_Delete( m_pBaseShader );
	m_pBaseShader = new BaseShader();

	bool bVSResult = false;
	bool bPSResult = false;
	bool bILResult = false;
	if( pDevice && m_pBaseShader && m_pBaseResource )
	{
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT numElements = ARRAYSIZE( layout );
				
		ID3DBlob* pVSBlob = nullptr;
		bVSResult = m_pBaseShader->CreateVertexShader( pDevice->GetDevice(), L"Tutorial03.fx", "VS", "vs_5_0", &pVSBlob );
		bILResult = m_pBaseResource->CreateInputLayout( pDevice->GetDevice(), layout, numElements, pVSBlob );
		Safe_Release( pVSBlob );
		
		ID3DBlob* pPSBlob = nullptr;
		bPSResult = m_pBaseShader->CreatePixelShader( pDevice->GetDevice(), L"Tutorial03.fx", "PS", "ps_5_0", &pPSBlob );
		Safe_Release( pPSBlob );		
	}

	if( !bVSResult || !bPSResult || !bILResult )
		return false;
	else
		return true;
}

void BaseApp::Update()
{	
	XMVECTOR Eye = XMVectorSet( 0.0f, 1.0f, -5.0f, 0.0f );
	XMVECTOR At = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	XMVECTOR Up = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	m_matView = XMMatrixLookAtLH( Eye, At, Up );
		
	m_matProjection = XMMatrixPerspectiveFovLH( XM_PIDIV2, WINDOW_WIDTH / ( FLOAT )WINDOW_HEIGHT, 0.01f, 100.0f );

	if( m_pObject )
	{
		m_pObject->Update();
	}
}

void BaseApp::Render()
{
	DeviceDx11& refDevice = DeviceDx11::GetInstance();

	ID3D11Buffer* pCBuffer = m_pBaseResource->GetConstantBuffer();
	
	refDevice.GetDeviceContext()->ClearRenderTargetView( m_pBaseResource->GetRenderTargetView(), reinterpret_cast< const float* >( &Colors::DefaultColor ) );
	refDevice.GetDeviceContext()->ClearDepthStencilView( m_pBaseResource->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );

	refDevice.GetDeviceContext()->VSSetShader( m_pBaseShader->GetVertexShader(), NULL, 0 );
	refDevice.GetDeviceContext()->PSSetShader( m_pBaseShader->GetPixelShader(), NULL, 0 );

	refDevice.GetDeviceContext()->IASetInputLayout( m_pBaseResource->GetInputLayout() );
	refDevice.GetDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	
	if( m_pObject && pCBuffer )
	{	
		ConstantBuffer cb;
		cb.mWorld = XMMatrixTranspose( m_pObject->GetWorld() );
		cb.mView = XMMatrixTranspose( m_matView );
		cb.mProjection = XMMatrixTranspose( m_matProjection );
		refDevice.GetDeviceContext()->UpdateSubresource( pCBuffer, 0, NULL, &cb, 0, 0 );	//버퍼에 내용 채우기
		refDevice.GetDeviceContext()->VSSetConstantBuffers( 0, 1, &pCBuffer );	//버퍼 vs에 셋팅하기
	}

	{
		UINT stride = sizeof( SimpleVertex );
		UINT offset = 0;
		ID3D11Buffer* pVertexBuffer = m_pBaseResource->GetVertexBuffer();
		refDevice.GetDeviceContext()->IASetVertexBuffers( 0, 1, &pVertexBuffer, &stride, &offset );
	}

	{
		ID3D11Buffer* pIndexBuffer = m_pBaseResource->GetIndexBuffer();
		refDevice.GetDeviceContext()->IASetIndexBuffer( pIndexBuffer, DXGI_FORMAT_R16_UINT, 0 );
	}	
	
	refDevice.GetDeviceContext()->DrawIndexed( m_pObject->GetIndicesArrayNum(), 0, 0 );

	refDevice.GetSwapChain()->Present( 0, 0 );
}

void BaseApp::CleanupApp()
{
	Safe_Delete( m_pBaseResource );
	Safe_Delete( m_pBaseShader );
	Safe_Delete( m_pObject );	
}

bool BaseApp::CreateVertexBuffer()
{
	if( m_pObject )
	{
		if( m_pBaseResource )
		{
			DeviceDx11& refDevice = DeviceDx11::GetInstance();			
			if( m_pBaseResource->CreateVertexBuffer( refDevice.GetDevice(), m_pObject->GetVertecesSize(), m_pObject->GetVertices() ) )
			{
				return true;
			}
		}
	}	

	return false;
}

bool BaseApp::CreateIndexBuffer()
{	
	if( m_pObject )
	{
		if( m_pBaseResource )
		{
			DeviceDx11& refDevice = DeviceDx11::GetInstance();
			if( m_pBaseResource->CreateIndexBuffer( refDevice.GetDevice(), m_pObject->GetIndicesSize(), m_pObject->GetIndices() ) )
			{
				return true;
			}
		}
	}

	return false;
}

bool BaseApp::CreateConstantBuffer()
{
	if( m_pBaseResource )
	{		
		DeviceDx11& refDevice = DeviceDx11::GetInstance();
		if( m_pBaseResource->CreateConstantBuffer( refDevice.GetDevice(), sizeof(ConstantBuffer) ) )
		{
			return true;
		}
	}

	return false;
}


