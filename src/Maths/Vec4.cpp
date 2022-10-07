// SPDX-FileCopyrightText: 2022 Admer Šuko
// SPDX-License-Identifier: MIT

#include "Precompiled.hpp"
using namespace adm;

// ============================
// Vec4::ctor for C strings
// ============================
Vec4::Vec4( const char* string )
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
	
	m.x = std::stof( lex.Next() );
	if ( !lex.IsEndOfFile() )
	{
		m.y = std::stof( lex.Next() );
	}
	if ( !lex.IsEndOfFile() )
	{
		m.z = std::stof( lex.Next() );
	}
	if ( !lex.IsEndOfFile() )
	{
		m.w = std::stof( lex.Next() );
	}
}

const Vec4 Vec4::Identity 	= Vec4( 1.0f );
const Vec4 Vec4::Zero 		= Vec4( 0.0f );

const Vec4 Vec4::Red		= Vec4( 1.0f, 0.0f, 0.0f, 1.0f );
const Vec4 Vec4::Orange		= Vec4( 1.0f, 0.5f, 0.0f, 1.0f );
const Vec4 Vec4::Yellow		= Vec4( 1.0f, 0.9f, 0.0f, 1.0f );
const Vec4 Vec4::Green		= Vec4( 0.0f, 1.0f, 0.0f, 1.0f );
const Vec4 Vec4::LightGreen = Vec4( 0.4f, 1.0f, 0.4f, 1.0f );
const Vec4 Vec4::Blue		= Vec4( 0.0f, 0.0f, 1.0f, 1.0f );
const Vec4 Vec4::LightBlue	= Vec4( 0.3f, 0.8f, 1.0f, 1.0f );
const Vec4 Vec4::Cyan		= Vec4( 0.0f, 1.0f, 1.0f, 1.0f );
const Vec4 Vec4::Pink		= Vec4( 0.9f, 0.5f, 0.9f, 1.0f );
const Vec4 Vec4::Purple		= Vec4( 0.5f, 0.1f, 0.5f, 1.0f );
const Vec4 Vec4::Grey		= Vec4( 0.5f, 0.5f, 0.5f, 1.0f );
const Vec4 Vec4::White		= Vec4( 1.0f, 1.0f, 1.0f, 1.0f );
const Vec4 Vec4::Black		= Vec4( 0.0f, 0.0f, 0.0f, 1.0f );
