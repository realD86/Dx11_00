#pragma once

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

struct SimpleIndex
{
	WORD _1, _2, _3;
};

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
};

class Object
{
public:
	Object();
	~Object();

public:
	void Init();
	void Update();
	void Render();
	void CleanupObject();

public:
	inline int GetVertecesSize(){ return sizeof( m_vertices ); }
	inline int GetIndicesSize(){ return sizeof( m_indices ); }
	inline int GetIndicesArrayNum(){ return sizeof( m_indices ) / sizeof( WORD ) ; }
	inline SimpleVertex* GetVertices() { return &m_vertices[0]; }
	inline SimpleIndex* GetIndices() { return &m_indices[0]; }

public:
	void* operator new(size_t size)
	{
		return ( Object* )_aligned_malloc( sizeof( Object ), 16 );
	}

	void operator delete( void* ptr )
	{
		_aligned_free( ptr );
	}
public:
	inline const XMMATRIX& GetWorld() { return m_matWorld; }	
	void AdjustConstant();

private:
	SimpleVertex m_vertices[8];
	SimpleIndex m_indices[12];
	XMMATRIX m_matWorld;
};

