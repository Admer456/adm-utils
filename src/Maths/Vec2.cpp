
#include "Precompiled.hpp"
using namespace adm;

// ============================
// Vec2::ctor for C strings
// ============================
Vec2::Vec2( const char* string )
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
}

const Vec2 Vec2::Identity 	= Vec2( 1.0f );
const Vec2 Vec2::Zero 		= Vec2( 0.0f );
const Vec2 Vec2::Forward 	= Vec2( 1.0f, 0.0f );
const Vec2 Vec2::Right 		= Vec2( 0.0f, 1.0f );

