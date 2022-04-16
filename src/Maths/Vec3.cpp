
#include "Precompiled.hpp"
using namespace adm;

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
