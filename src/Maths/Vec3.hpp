
#pragma once

namespace adm
{
	// ============================
	// 3D vector class for game logic
	// ============================
	class Vec3 final
	{
	public: // Construction
		constexpr Vec3() = default;
		constexpr explicit Vec3( float XYZ ) : x(XYZ), y(XYZ), z(XYZ) {}
		constexpr Vec3( float X, float Y, float Z ) : x(X), y(Y), z(Z) {}
		constexpr Vec3( Vec3&& v ) noexcept = default;
		constexpr Vec3( const Vec3& v ) = default;
		Vec3( const char* string );

		// Generic 3-float array support
		constexpr Vec3( const float* vec ) : x(vec[0]), y(vec[1]), z(vec[2]) {}

	public: // Methods
		// 3D length of this vector
		inline float 		Length() const
		{
			return std::sqrt( x*x + y*y + z*z );
		}
		// destination - this
		inline Vec3 		DirectionTo( const Vec3& destination, bool normalized = false ) const
		{
			Vec3 result = destination - *this;
			if ( normalized )
			{
				result.Normalize();
			}

			return result;
		}
		// Normalizes this vector and returns it
		inline const Vec3& 	Normalize()
		{
			float length = Length();
			if ( length == 0.0f )
			{
				return Zero;
			}

			*this /= length;

			return *this;
		}
		// Returns a normalized copy of this vector
		inline Vec3 		Normalized() const
		{
			return Vec3(*this).Normalize();
		}
		// Returns a dot product of this vector with another
		inline float 		Dot( const Vec3& vec ) const
		{
			return x*vec.x + y*vec.y + z*vec.z;
		}
		// Returns a cross product of this vector with another
		inline Vec3 		Cross( const Vec3& vec ) const
		{
			return Vec3(
				(y * vec.z - z * vec.y),
				(z * vec.x - x * vec.z),
				(x * vec.y - y * vec.x)
			);
		}
		// Snaps this vector to an integer grid
		inline const Vec3& 	Snap( const int& grid = 1 )
		{
			if ( grid == 1 )
			{
				x = int(x);
				y = int(y);
				z = int(z);
				return *this;
			}

			x = int(x / grid) * grid;
			y = int(y / grid) * grid;
			z = int(z / grid) * grid;
			return *this;
		}
		// Returns a snapped copy of this vector
		inline Vec3 		Snapped( const int& grid = 1 ) const
		{
			return Vec3(*this).Snap( grid );
		}
		// Reflection of this vector off a plane
		// @param bias: smaller = stronger normal influence, bigger = weaker normal influence
		inline Vec3 		Reflected( const Vec3& normal, const float& bias = 2.0f ) const
		{
			float dot = (*this) * normal;
			Vec3 projected = (normal * (bias * dot));
			return *this - projected;
		}
		// Projection of this vector onto a plane
		inline Vec3 		ProjectedOnPlane( const Vec3& normal ) const
		{
			const float dot = *this * normal;
			return *this - (normal * dot);
		}
		// Since Vec3 == Vec3 is too strict, this can be used to compare two
		// vectors with an epsilon value
		bool 				Equals( const Vec3& vec, const float& epsilon = 0.05f ) const
		{
			bool X = (x < vec.x + epsilon) && (x > vec.x - epsilon);
			bool Y = (y < vec.y + epsilon) && (y > vec.y - epsilon);
			bool Z = (z < vec.z + epsilon) && (z > vec.z - epsilon);

			return X && Y && Z;
		}

	public: // Constants
		static const Vec3 Identity;
		static const Vec3 Zero;
						  
		static const Vec3 Forward;
		static const Vec3 Right;
		static const Vec3 Up;

	public: // Operators
		// Vec3 + Vec3 
		inline Vec3 		operator+ ( const Vec3& rhs ) const
		{
			return Vec3{
				x + rhs.x,
				y + rhs.y,
				z + rhs.z
			};
		}
		// Vec3 - Vec3
		inline Vec3			operator- ( const Vec3& rhs ) const
		{
			return Vec3{
				x - rhs.x,
				y - rhs.y,
				z - rhs.z
			};
		}
		// += Vec3
		inline const Vec3& 	operator+= ( const Vec3& rhs )
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;

			return *this;
		}
		// -= Vec3
		inline const Vec3& 	operator-= ( const Vec3& rhs )
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;

			return *this;
		}
		// -Vec3
		inline Vec3 operator- () const
		{
			return *this * -1.0f;
		}
		// Vec3 == Vec3
		inline bool 		operator== ( const Vec3& rhs ) const
		{
			return x == rhs.x && y == rhs.y && z == rhs.z;
		}
		// Vec3 = Vec3
		inline const Vec3& 	operator= ( const Vec3& rhs )
		{
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;

			return *this;
		}
		// Vec3 * Vec3, dot product
		inline float		operator* ( const Vec3& rhs ) const
		{
			return Dot( rhs );
		}
		// Vec3 * float
		inline Vec3 		operator* ( const float& rhs ) const
		{
			return Vec3{
				x * rhs,
				y * rhs,
				z * rhs
			};
		}
		// Vec3 / float
		inline Vec3 		operator/ ( const float& rhs ) const
		{
			return Vec3{
				x / rhs,
				y / rhs,
				z / rhs
			};
		}
		// Vec3 *= float
		inline const Vec3& 	operator*= ( const float& rhs )
		{
			x *= rhs;
			y *= rhs;
			z *= rhs;

			return *this;
		}
		// Vec3 /= float
		inline const Vec3& 	operator/= ( const float& rhs )
		{
			x /= rhs;
			y /= rhs;
			z /= rhs;

			return *this;
		}
		// Generic float array support, in case someone uses this library with Quake or Half-Life
		inline operator		float* ()
		{
			return &x;
		}
		// Const version
		inline operator		const float*() const
		{
			return &x;
		}

	public:
		float x{0.0f}, y{0.0f}, z{0.0f};
	};
}

// float * Vec3
inline adm::Vec3 operator* ( const float& lhs, const adm::Vec3& rhs )
{
	return adm::Vec3{
		rhs.x * lhs,
		rhs.y * lhs,
		rhs.z * lhs
	};
}

namespace std
{
	// Extending le standard bibliotheque to support Vec3
	inline std::string to_string( adm::Vec3 val )
	{
		// TODO: use fmtlib, sprintf is like 20x slower
		char buffer[128]; // God forbid you put such a large number to overflow this...
		snprintf( buffer, 128, "%f %f %f", val.x, val.y, val.z );
		return std::string( buffer );
	}

	inline adm::Vec3 fabs( const adm::Vec3& v )
	{
		return adm::Vec3{
			fabs( v.x ),
			fabs( v.y ),
			fabs( v.z )
		};
	}
}

inline std::ostream& operator << ( std::ostream& os, const adm::Vec3& vec )
{
	os << vec.x << " " << vec.y << " " << vec.z;
	return os;
}
