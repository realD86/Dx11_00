#pragma once

class BaseResource
{
public:
	BaseResource();
	~BaseResource();
public:
	inline ID3D11RenderTargetView*  GetRenderTargetView()	const	{ return m_pRenderTargetView;	}
	inline ID3D11DepthStencilView*	GetDepthStencilView()	const	{ return m_pDepthStencilView;	}
	inline ID3D11Buffer*			GetVertexBuffer()		const	{ return m_pVertexBuffer;		}
	inline ID3D11Buffer*			GetIndexBuffer()		const	{ return m_pIndexBuffer;		}
	inline ID3D11InputLayout*		GetInputLayout()		const	{ return m_pVertexLayout;		}
	inline D3D11_VIEWPORT&			GetViewport()					{ return m_ScreenViewport;		}

	bool CreateRenderTargetView( ID3D11Device* pDevice, IDXGISwapChain* pSwapChain );
	bool CreateDepthStencilView( ID3D11Device* pDevice, int width, int height );
	bool CreateInputLayout( ID3D11Device* pDevice, const D3D11_INPUT_ELEMENT_DESC* pLayout, UINT numElements, ID3DBlob* pVSBlob );
	void InitViewport( int width, int height );

	bool CreateVertexBuffer( ID3D11Device* pDevice, int totalVertexSize, void* pVertices );
	bool CreateIndexBuffer( ID3D11Device* pDevice, int totalIndexSize, void* pIndices );
	void CleanupResource();

private:	
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11DepthStencilView* m_pDepthStencilView;
	ID3D11Texture2D*		m_pDepthStencilBuffer;
	D3D11_VIEWPORT			m_ScreenViewport;

	ID3D11InputLayout*		m_pVertexLayout;
	ID3D11Buffer*			m_pVertexBuffer;
	ID3D11Buffer*			m_pIndexBuffer;
};

