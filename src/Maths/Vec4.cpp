
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
	
	x = std::stof( lex.Next() );
	if ( !lex.IsEndOfFile() )
	{
		y = std::stof( lex.Next() );
	}
	if ( !lex.IsEndOfFile() )
	{
		z = std::stof( lex.Next() );
	}
	if ( !lex.IsEndOfFile() )
	{
		w = std::stof( lex.Next() );
	}
}

const Vec4 Vec4::Identity 	= Vec4( 1.0f );
const Vec4 Vec4::Zero 		= Vec4( 0.0f );
