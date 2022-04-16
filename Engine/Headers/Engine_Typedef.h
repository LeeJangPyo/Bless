#pragma once

namespace Engine
{

	typedef signed char					_byte;
	typedef unsigned char				_ubyte;

	typedef signed short				_short;
	typedef unsigned short				_ushort;

	typedef signed int					_int;
	typedef unsigned int				_uint;

	typedef signed long					_long;
	typedef unsigned long				_ulong;

	typedef float						_float;
	typedef double						_double;

	typedef bool						_bool;
	typedef wchar_t						_tchar;
	typedef	char						_char;

	typedef D3DXVECTOR2					_vec2;
	typedef D3DXVECTOR3					_vec3;
	typedef D3DXVECTOR4					_vec4;

	/*typedef struct D3DXVECTOR3_DERIVED : public D3DXVECTOR3
	{
		float Get_Length();
		float Get_Distance(const D3DXVECTOR3_DERIVED& vTarget);
	}_vec3;*/	

	typedef D3DXMATRIX					_matrix;
}