#pragma once

class BaseShader
{
public:
	BaseShader();
	~BaseShader();

public:		
	bool CreateVertexShader( ID3D11Device* pDevice, LPCWSTR szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppVSBlob );
	bool CreatePixelShader( ID3D11Device* pDevice, LPCWSTR szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppPSBlob );
	inline ID3D11VertexShader* GetVertexShader(){ return m_pVertexShader; }
	inline ID3D11PixelShader* GetPixelShader( ){ return m_pPixelShader; }	
	void CleanupShader();

private:
	bool BuildShader( LPCWSTR szFileName, LPCSTR szEntryPoint, LPCSTR szShaderMode, ID3DBlob** ppBlob );

private:	
	ID3D11VertexShader*     m_pVertexShader;
	ID3D11PixelShader*      m_pPixelShader;
};

