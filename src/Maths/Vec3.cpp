// SPDX-FileCopyrightText: 2022 Admer Šuko
// SPDX-License-Identifier: MIT

#include "Precompiled.hpp"
using namespace adm;

// ============================
// Vec3::ctor adapter for Vec2
// ============================
constexpr Vec3::Vec3( const Vec2& v, float Z )
	: x( v.x ), y( v.y ), z( Z )
{

}

// ============================
// Vec3::ctor adapter for Vec4
// ============================
constexpr Vec3::Vec3( const Vec4& v )
	: x( v.m.x ), y( v.m.y ), z( v.m.z )
{

}

// ============================
// Vec3::ctor for C strings
// ============================
Vec3::Vec3( const char* string )
{
	if ( nullptr == string )
	{
		// Members are already initialised at this point
		return;
	}

	Lexer lex( string );
	if ( lex.IsEndOfFile() )
	{
		return;
	}
	
	x = std::stof( lex.Next() );
	if ( !lex.IsEndOfFile() )
	{
		y = std::stof( lex.Next() );
	}
	if ( !lex.IsEndOfFile() )
	{
		z = std::stof( lex.Next() );
	}
}

const Vec3 Vec3::Identity 	= Vec3( 1.0f );
const Vec3 Vec3::Zero 		= Vec3( 0.0f );
const Vec3 Vec3::Forward 	= Vec3( 1.0f, 0.0f, 0.0f );
const Vec3 Vec3::Right 		= Vec3( 0.0f,-1.0f, 0.0f );
const Vec3 Vec3::Up 		= Vec3( 0.0f, 0.0f, 1.0f );
