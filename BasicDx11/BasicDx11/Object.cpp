#include "stdafx.h"
#include "Object.h"

Object::Object()
{	
}


Object::~Object()
{	
}

void Object::Init()
{
	m_matWorld = XMMatrixIdentity();
	
	m_vertices[0] = { XMFLOAT3( -1.0f, 1.0f, -1.0f ), XMFLOAT4( 0.0f, 0.0f, 1.0f, 1.0f ) };
	m_vertices[1] = { XMFLOAT3( 1.0f, 1.0f, -1.0f ), XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f ) };
	m_vertices[2] = { XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT4( 0.0f, 1.0f, 1.0f, 1.0f ) };
	m_vertices[3] = { XMFLOAT3( -1.0f, 1.0f, 1.0f ), XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f ) };
	m_vertices[4] = { XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT4( 1.0f, 0.0f, 1.0f, 1.0f ) };
	m_vertices[5] = { XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT4( 1.0f, 1.0f, 0.0f, 1.0f ) };
	m_vertices[6] = { XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f ) };
	m_vertices[7] = { XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f ) };

	m_indices[0] = { 3, 1, 0 };
	m_indices[1] = { 2, 1, 3 };
	m_indices[2] = { 0, 5, 4 };
	m_indices[3] = { 1, 5, 0 };
	m_indices[4] = { 3, 4, 7 };
	m_indices[5] = { 0, 4, 3 };
	m_indices[6] = { 1, 6, 5 };
	m_indices[7] = { 2, 6, 1 };
	m_indices[8] = { 2, 7, 6 };
	m_indices[9] = { 3, 7, 2 };
	m_indices[10] = { 6, 4, 5 };
	m_indices[11] = { 7, 4, 6 };	
}

void Object::Update()
{	
	static float t = 0.0f;
		
	static DWORD dwTimeStart = 0;
	DWORD dwTimeCur = GetTickCount();
	if( dwTimeStart == 0 )
		dwTimeStart = dwTimeCur;
	t = ( dwTimeCur - dwTimeStart ) / 1000.0f;	

	m_matWorld = XMMatrixRotationY( t );
}

void Object::Render()
{

}

void Object::CleanupObject()
{

}

void Object::AdjustConstant()
{

}
