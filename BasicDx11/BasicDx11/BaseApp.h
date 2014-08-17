#pragma once


class BaseResource;
class BaseShader;
class Object;
class DeviceDx11;
class BaseApp
{
public:
	BaseApp();
	~BaseApp();

public:
	void* operator new( size_t size )
	{
		return ( BaseApp* )_aligned_malloc( sizeof( BaseApp ), 16 );
	}

	void operator delete( void* ptr )
	{
		_aligned_free( ptr );
	}

public:
	bool Init( HWND hWnd );
	void Update();
	void Render();
	void CleanupApp();

private:
	bool CreateVertexBuffer();
	bool CreateIndexBuffer();
	bool CreateConstantBuffer();
	bool BuildShader( DeviceDx11* pDevice );	
	bool SetUpDxResource( DeviceDx11* pDevice );

private:
	BaseResource* m_pBaseResource;
	BaseShader* m_pBaseShader;
	Object* m_pObject;

	XMMATRIX m_matView;
	XMMATRIX m_matProjection;	
};