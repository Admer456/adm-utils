
#pragma once

namespace adm
{
	class Vec2;
	class Vec3;

	// ============================
	// 4D vector class for colours, shader parameters etc.
	// For rotation maths, look at Quat
	// ============================
	class Vec4 final
	{
	public: // Construction
		constexpr Vec4() = default;
		constexpr explicit Vec4( float XYZW ) : x(XYZW), y(XYZW), z(XYZW), w(XYZW) {}
		constexpr Vec4( float X, float Y, float Z, float W ) : x(X), y(Y), z(Z), w(W) {}
		constexpr Vec4( Vec4&& v ) noexcept = default;
		constexpr Vec4( const Vec2& vec ) : x(vec.x), y(vec.y) {}
		constexpr Vec4( const Vec3& vec ) : x(vec.x), y(vec.y), z(vec.z) {}
		constexpr Vec4( const Vec3& vec, float W ) : x(vec.x), y(vec.y), z(vec.z), w(W) {}
		constexpr Vec4( const Vec4& v ) = default;
		Vec4( const char* string );

		// Generic 4-float array support
		constexpr Vec4( const float* vec ) : x(vec[0]), y(vec[1]), z(vec[2]), w(vec[3]) {}

	public: // Methods
		// 4D length of this vector
		inline float 		Length() const
		{
			return std::sqrt( LengthSquared() );
		}
		inline float		LengthSquared() const
		{
			return x*x + y*y + z*z + w*w;
		}
		// destination - this
		inline Vec4 		DirectionTo( const Vec4& destination, bool normalized = false ) const
		{
			Vec4 result = destination - *this;
			if ( normalized )
			{
				result.Normalize();
			}

			return result;
		}
		// Normalizes this vector and returns it
		inline const Vec4& 	Normalize()
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
		inline Vec4 		Normalized() const
		{
			return Vec4(*this).Normalize();
		}
		// Returns a dot product of this vector with another
		inline float 		Dot( const Vec4& vec ) const
		{
			return x*vec.x + y*vec.y + z*vec.z + w*vec.w;
		}
		// Snaps this vector to an integer grid
		inline const Vec4& 	Snap( const int& grid = 1 )
		{
			if ( grid == 1 )
			{
				x = int(x);
				y = int(y);
				z = int(z);
				w = int(w);
				return *this;
			}

			x = int(x / grid) * grid;
			y = int(y / grid) * grid;
			z = int(z / grid) * grid;
			w = int(w / grid) * grid;
			return *this;
		}
		// Returns a snapped copy of this vector
		inline Vec4 		Snapped( const int& grid = 1 ) const
		{
			return Vec4(*this).Snap( grid );
		}
		// Reflection of this vector off a plane
		// @param bias: smaller = stronger normal influence, bigger = weaker normal influence
		inline Vec4 		Reflected( const Vec4& normal, const float& bias = 2.0f ) const
		{
			float dot = (*this) * normal;
			Vec4 projected = (normal * (bias * dot));
			return *this - projected;
		}
		// Projection of this vector onto a plane
		inline Vec4 		ProjectedOnPlane( const Vec4& normal ) const
		{
			const float dot = *this * normal;
			return *this - (normal * dot);
		}
		// Since Vec4 == Vec4 is too strict, this can be used to compare two
		// vectors with an epsilon value
		bool 				Equals( const Vec4& vec, const float& epsilon = 0.05f ) const
		{
			bool X = (x < vec.x + epsilon) && (x > vec.x - epsilon);
			bool Y = (y < vec.y + epsilon) && (y > vec.y - epsilon);
			bool Z = (z < vec.z + epsilon) && (z > vec.z - epsilon);
			bool W = (w < vec.w + epsilon) && (w > vec.w - epsilon);

			return X && Y && Z && W;
		}

	public: // Constants
		static const Vec4 Identity;
		static const Vec4 Zero;

	public: // Operators
		// Vec4 + Vec4 
		inline Vec4 		operator+ ( const Vec4& rhs ) const
		{
			return Vec4{
				x + rhs.x,
				y + rhs.y,
				z + rhs.z,
				w + rhs.w
			};
		}
		// Vec4 - Vec4
		inline Vec4			operator- ( const Vec4& rhs ) const
		{
			return Vec4{
				x - rhs.x,
				y - rhs.y,
				z - rhs.z,
				w - rhs.w
			};
		}
		// += Vec4
		inline const Vec4& 	operator+= ( const Vec4& rhs )
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			w += rhs.w;

			return *this;
		}
		// -= Vec4
		inline const Vec4& 	operator-= ( const Vec4& rhs )
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			w -= rhs.w;

			return *this;
		}
		// -Vec4
		inline Vec4 		operator- () const
		{
			return *this * -1.0f;
		}
		// Vec4 == Vec4
		inline bool 		operator== ( const Vec4& rhs ) const
		{
			return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
		}
		// Vec4 = Vec4
		inline const Vec4& 	operator= ( const Vec4& rhs )
		{
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;
			w = rhs.w;

			return *this;
		}
		// Vec4 * Vec4, dot product
		inline float		operator* ( const Vec4& rhs ) const
		{
			return Dot( rhs );
		}
		// Vec4 * float
		inline Vec4 		operator* ( const float& rhs ) const
		{
			return Vec4{
				x * rhs,
				y * rhs,
				z * rhs,
				w * rhs
			};
		}
		// Vec4 / float
		inline Vec4 		operator/ ( const float& rhs ) const
		{
			return Vec4{
				x / rhs,
				y / rhs,
				z / rhs,
				w / rhs
			};
		}
		// Vec4 *= float
		inline const Vec4& 	operator*= ( const float& rhs )
		{
			x *= rhs;
			y *= rhs;
			z *= rhs;
			w *= rhs;

			return *this;
		}
		// Vec4 /= float
		inline const Vec4& 	operator/= ( const float& rhs )
		{
			x /= rhs;
			y /= rhs;
			z /= rhs;
			w /= rhs;

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
		float x{ 0.0f }, y{ 0.0f }, z{ 0.0f }, w{ 0.0f };
	};
}

// float * Vec4
inline adm::Vec4 operator* ( const float& lhs, const adm::Vec4& rhs )
{
	return adm::Vec4{
		rhs.x * lhs,
		rhs.y * lhs,
		rhs.z * lhs,
		rhs.w * lhs
	};
}

namespace std
{
	// Extending le standard bibliotheque to support Vec4
	inline std::string to_string( adm::Vec4 val )
	{
		// TODO: use fmtlib, sprintf is like 20x slower
		char buffer[128]; // God forbid you put such a large number to overflow this...
		snprintf( buffer, 128, "%f %f %f %f", val.x, val.y, val.z, val.w );
		return std::string( buffer );
	}

	inline adm::Vec4 fabs( const adm::Vec4& v )
	{
		return adm::Vec4{
			fabs( v.x ),
			fabs( v.y ),
			fabs( v.z ),
			fabs( v.w )
		};
	}
}

inline std::ostream& operator << ( std::ostream& os, const adm::Vec4& vec )
{
	os << vec.x << " " << vec.y << " " << vec.z << " " << vec.w;
	return os;
}
