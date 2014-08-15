#pragma once

template <typename T>
void Safe_Delete( T& p )
{
	if( p )
	{
		delete	p;
		p = nullptr;
	}
}

template <typename T>
void Safe_Release( T& p )
{
	if( p )
	{
		while( p->Release() ){}
		p = nullptr;		
	}
}