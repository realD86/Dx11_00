#include "stdafx.h"
#include "BaseShader.h"
#include "CommonUse.h"

BaseShader::BaseShader()
: m_pVertexShader( nullptr ), m_pPixelShader( nullptr )
{
}


BaseShader::~BaseShader()
{
	CleanupShader();
}

bool BaseShader::BuildShader( LPCWSTR szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlob )
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )	
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob;
	hr = D3DX11CompileFromFile( szFileName, NULL, NULL, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, NULL, ppBlob, &pErrorBlob, NULL );

	if( FAILED( hr ) )
	{
		if( pErrorBlob != NULL )
			OutputDebugStringA( ( char* )pErrorBlob->GetBufferPointer() );

		Safe_Release( pErrorBlob );

		return false;
	}

	Safe_Release( pErrorBlob );		
		
	if( FAILED( hr ) )
	{
		MessageBox( NULL,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );	
		return false;
	}

	return true;
}

bool BaseShader::CreateVertexShader( ID3D11Device* pDevice, LPCWSTR szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppVSBlob )
{
	if( pDevice )
	{
		Safe_Release( m_pVertexShader );

		if( BuildShader( szFileName, szEntryPoint, szShaderModel, ppVSBlob ) )
		{				
			if( SUCCEEDED( pDevice->CreateVertexShader( ( *ppVSBlob )->GetBufferPointer(), ( *ppVSBlob )->GetBufferSize(), NULL, &m_pVertexShader ) ) )
			{
				return true;
			}			
		}
	}

	return false;	
}

bool BaseShader::CreatePixelShader( ID3D11Device* pDevice, LPCWSTR szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppPSBlob )
{
	if( pDevice )
	{		
		Safe_Release( m_pPixelShader );

		if( BuildShader( szFileName, szEntryPoint, szShaderModel, ppPSBlob ) )
		{
			if( SUCCEEDED( pDevice->CreatePixelShader( ( *ppPSBlob )->GetBufferPointer(), ( *ppPSBlob )->GetBufferSize(), NULL, &m_pPixelShader ) ) )
			{
				return true;
			}
		}
	}

	return false;
}

void BaseShader::CleanupShader()
{	
	Safe_Release( m_pVertexShader );
	Safe_Release( m_pPixelShader );
}
