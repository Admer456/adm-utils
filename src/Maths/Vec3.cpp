
#include "Precompiled.hpp"
using namespace adm;

Vec3::Vec3( const char* string )
{
	if ( nullptr == string )
	{
		*this = Vec3::Zero;
		return;
	}

	std::sscanf( string, "%f %f %f", &x, &y, &z );
}

const Vec3 Vec3::Identity 	= Vec3( 1.0f );
const Vec3 Vec3::Zero 		= Vec3( 0.0f );
const Vec3 Vec3::Forward 	= Vec3( 1.0f, 0.0f, 0.0f );
const Vec3 Vec3::Right 		= Vec3( 0.0f, 1.0f, 0.0f );
const Vec3 Vec3::Up 		= Vec3( 0.0f, 0.0f, 1.0f );
