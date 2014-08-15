#pragma once

class BaseResource;
class BaseShader;
class BaseApp
{
public:
	BaseApp();
	~BaseApp();

public:
	bool Init( HWND hWnd );
	void Update();
	void Render();
	void CleanupApp();

public:
	bool CreateVertexBuffer();
	bool CreateIndexBuffer();
private:
	BaseResource* m_pBaseResource;
	BaseShader* m_pBaseShader;	
};