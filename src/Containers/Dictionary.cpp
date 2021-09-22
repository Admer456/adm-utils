
#include "Precompiled.hpp"
using namespace adm;

Dictionary::Dictionary()
{
	Clear();
}

Dictionary::Dictionary( Dictionary&& dict ) noexcept
{
	pairs = std::move( dict.pairs );
}

Dictionary::Dictionary( const Dictionary& dict )
{
	pairs = dict.pairs;
}

Dictionary::~Dictionary()
{
	Clear();
}

std::string Dictionary::GetString( const char* keyname, const char* default ) const
{
	if ( !KeyExists( keyname ) )
	{
		return default;
	}

	return pairs.at( keyname );
}

bool Dictionary::GetString( const char* keyname, std::string& out ) const
{
	if ( !KeyExists( keyname ) )
	{
		return false;
	}

	out = pairs.at( keyname );
	return true;
}

void Dictionary::SetString( const char* keyname, std::string_view value )
{
	pairs[keyname] = value;
}

const char* Dictionary::GetCString( const char* keyname, const char* default ) const
{
	if ( !KeyExists( keyname ) )
	{
		return default;
	}

	return pairs.at( keyname ).c_str();
}

bool Dictionary::GetCString( const char* keyname, char* out, int length ) const
{
	if ( !KeyExists( keyname ) )
	{
		return false;
	}

	strncpy( out, GetStringDirect( keyname ), length );
	return true;
}

void Dictionary::SetCString( const char* keyname, const char* value )
{
	pairs[keyname] = value;
}

float Dictionary::GetFloat( const char* keyname, const float& default ) const
{
	if ( !KeyExists( keyname ) )
	{
		return default;
	}

	return std::atof( GetStringDirect( keyname ) );
}

bool Dictionary::GetFloat( const char* keyname, float& out ) const
{
	if ( !KeyExists( keyname ) )
	{
		return false;
	}

	out = std::atof( GetStringDirect( keyname ) );
	return true;
}

void Dictionary::SetFloat( const char* keyname, float value )
{
	pairs[keyname] = std::to_string( value );
}

int Dictionary::GetInteger( const char* keyname, const int& default ) const
{
	if ( !KeyExists( keyname ) )
	{
		return default;
	}

	return std::atoi( GetStringDirect( keyname ) );
}

bool Dictionary::GetInteger( const char* keyname, int& out ) const
{
	if ( !KeyExists( keyname ) )
	{
		return false;
	}

	out = std::atoi( GetStringDirect( keyname ) );
	return true;
}

void Dictionary::SetInteger( const char* keyname, int value )
{
	SetString( keyname, std::to_string( value ) );
}

bool Dictionary::GetBool( const char* keyname, const bool& default ) const
{
	if ( !KeyExists( keyname ) )
	{
		return default;
	}

	return std::atoi( GetStringDirect( keyname ) ) == 0;
}

bool Dictionary::GetBool( const char* keyname, bool& out ) const
{
	if ( !KeyExists( keyname ) )
	{
		return false;
	}

	out = std::atoi( GetStringDirect( keyname ) ) != 0;
	return true;
}

void Dictionary::SetBool( const char* keyname, bool value )
{
	SetCString( keyname, value ? "1" : "0" );
}

Vec3 Dictionary::GetVec3( const char* keyname, const Vec3& default ) const
{
	if ( !KeyExists( keyname ) )
	{
		return default;
	}

	return Vec3( GetStringDirect( keyname ) );
}

bool Dictionary::GetVec3( const char* keyname, Vec3& out ) const
{
	if ( !KeyExists( keyname ) )
	{
		return false;
	}

	out = Vec3( GetStringDirect( keyname ) );
	return true;
}

void Dictionary::SetVec3( const char* keyname, const Vec3& value )
{
	pairs[keyname] = std::to_string( value );
}

bool Dictionary::KeyExists( const char* keyname ) const
{
	return pairs.find( keyname ) != pairs.end();
}

void Dictionary::Clear()
{
	pairs.clear();
}
