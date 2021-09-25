
#pragma once

namespace adm
{
	class Vec3;

	/*
		Example usage:
		Dictionary dict;
		dict.SetFloat( "health", 100.0f );
		...
		float health = dict.GetFloat( "health" );
		
		OR
		
		float health = 0.0f;
		if ( dict.GetFloat( "health", health )
		{
			// the key has been found
		}

		OR

		if ( dict.KeyExists( "health" ) )
		{
			float health = dict.GetHealth( "health" );
		}
	*/

	// ============================
	// Game entity dictionary for keyvalues
	// ============================
	class Dictionary final
	{
	public:
		Dictionary();
		Dictionary( Dictionary&& dict ) noexcept;
		Dictionary( const Dictionary& dict );
		~Dictionary();

		// C++ strings
		std::string GetString( const char* keyname, const char* default = "" ) const;
		bool 		GetString( const char* keyname, std::string& out ) const;
		void 		SetString( const char* keyname, std::string_view value );
		// C strings
		const char* GetCString( const char* keyname, const char* default = "" ) const;
		bool 		GetCString( const char* keyname, char* out, int length ) const;
		void 		SetCString( const char* keyname, const char* value );
		// Floats
		float 		GetFloat( const char* keyname, const float& default = 0.0f ) const;
		bool 		GetFloat( const char* keyname, float& out ) const;
		void 		SetFloat( const char* keyname, float value );
		// Integers
		int 		GetInteger( const char* keyname, const int& default = 0 ) const;
		bool 		GetInteger( const char* keyname, int& out ) const;
		void 		SetInteger( const char* keyname, int value ) ;
		// Booleans
		bool 		GetBool( const char* keyname, const bool& default = false ) const;
		bool 		GetBool( const char* keyname, bool& out ) const;
		void 		SetBool( const char* keyname, bool value );
		// Vec3
		Vec3		GetVec3( const char* keyname, const Vec3& default = Vec3::Zero ) const;
		bool		GetVec3( const char* keyname, Vec3& out ) const;
		void		SetVec3( const char* keyname, const Vec3& value );
		// Does this key exist?
		bool		KeyExists( const char* keyname ) const;
		// Clears all keyvalue pairs
		void		Clear();

		std::string_view operator[] ( const char* keyname )
		{
			return pairs[keyname];
		}

	private:
		inline const char* GetStringDirect( const char* keyname ) const
		{
			return pairs.at( keyname ).c_str();
		}

		std::unordered_map<std::string, std::string> pairs;
};
}
