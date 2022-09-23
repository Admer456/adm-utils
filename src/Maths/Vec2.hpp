
#pragma once

namespace adm
{
	// ============================
	// 2D vector class for game logic and rendering
	// ============================
	class Vec2 final
	{
	public: // Construction
		constexpr Vec2() = default;
		constexpr explicit Vec2( float XY ) : x(XY), y(XY) {}
		constexpr Vec2( float X, float Y ) : x(X), y(Y) {}
		constexpr Vec2( Vec2&& v ) noexcept = default;
		constexpr Vec2( const Vec2& v ) = default;
		constexpr Vec2( const Vec3& v ) : x(v.x), y(v.y) {}
		constexpr Vec2( const Vec4& v ) : x(v.x), y(v.y) {}
		Vec2( const char* string );

		// Generic 2-float array support
		constexpr Vec2( const float* vec ) : x(vec[0]), y(vec[1]) {}

	public: // Methods
		// 2D length of this vector
		inline float 		Length() const
		{
			return std::sqrt( LengthSquared() );
		}
		inline float		LengthSquared() const
		{
			return x*x + y*y;
		}
		// destination - this
		inline Vec2 		DirectionTo( const Vec2& destination, bool normalized = false ) const
		{
			Vec2 result = destination - *this;
			if ( normalized )
			{
				result.Normalize();
			}

			return result;
		}
		// Normalizes this vector and returns it
		inline const Vec2& 	Normalize()
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
		inline Vec2 		Normalized() const
		{
			return Vec2(*this).Normalize();
		}
		// Returns a dot product of this vector with another
		inline float 		Dot( const Vec2& vec ) const
		{
			return x * vec.x + y * vec.y;
		}
		// Returns a perpendicular dot product
		// Some folks may accidentally refer to this as a "2D cross product",
		// but mathematically, you can't have those
		inline float 		PerpDot( const Vec2& vec ) const
		{
			return x * vec.y - y * vec.x;
		}
		// Snaps this vector to an integer grid
		inline const Vec2& 	Snap( const int& grid = 1 )
		{
			if ( grid == 1 )
			{
				x = int(x);
				y = int(y);
				return *this;
			}

			x = int(x / grid) * grid;
			y = int(y / grid) * grid;
			return *this;
		}
		// Returns a snapped copy of this vector
		inline Vec2 		Snapped( const int& grid = 1 ) const
		{
			return Vec2(*this).Snap( grid );
		}
		// Reflection of this vector off a plane
		// @param bias: smaller = stronger normal influence, bigger = weaker normal influence
		inline Vec2 		Reflected( const Vec2& normal, const float& bias = 2.0f ) const
		{
			float dot = (*this) * normal;
			Vec2 projected = (normal * (bias * dot));
			return *this - projected;
		}
		// Projection of this vector onto a plane
		inline Vec2 		ProjectedOnPlane( const Vec2& normal ) const
		{
			const float dot = *this * normal;
			return *this - (normal * dot);
		}
		// Since Vec2 == Vec2 is too strict, this can be used to compare two
		// vectors with an epsilon value
		bool 				Equals( const Vec2& vec, const float& epsilon = 0.05f ) const
		{
			bool X = (x < vec.x + epsilon) && (x > vec.x - epsilon);
			bool Y = (y < vec.y + epsilon) && (y > vec.y - epsilon);

			return X && Y;
		}

	public: // Constants
		static const Vec2 Identity;
		static const Vec2 Zero;
						  
		static const Vec2 Forward;
		static const Vec2 Right;

	public: // Operators
		// Vec2 + Vec2 
		inline Vec2 		operator+ ( const Vec2& rhs ) const
		{
			return Vec2{
				x + rhs.x,
				y + rhs.y
			};
		}
		// Vec2 - Vec2
		inline Vec2			operator- ( const Vec2& rhs ) const
		{
			return Vec2{
				x - rhs.x,
				y - rhs.y
			};
		}
		// += Vec2
		inline const Vec2& 	operator+= ( const Vec2& rhs )
		{
			x += rhs.x;
			y += rhs.y;

			return *this;
		}
		// -= Vec2
		inline const Vec2& 	operator-= ( const Vec2& rhs )
		{
			x -= rhs.x;
			y -= rhs.y;

			return *this;
		}
		// -Vec2
		inline Vec2 		operator- () const
		{
			return *this * -1.0f;
		}
		// Vec2 == Vec2
		inline bool 		operator== ( const Vec2& rhs ) const
		{
			return x == rhs.x && y == rhs.y;
		}
		// Vec2 = Vec2
		inline const Vec2& 	operator= ( const Vec2& rhs )
		{
			x = rhs.x;
			y = rhs.y;

			return *this;
		}
		// Vec2 * Vec2, dot product
		inline float		operator* ( const Vec2& rhs ) const
		{
			return Dot( rhs );
		}
		// Vec2 * float
		inline Vec2 		operator* ( const float& rhs ) const
		{
			return Vec2{
				x * rhs,
				y * rhs
			};
		}
		// Vec2 / float
		inline Vec2 		operator/ ( const float& rhs ) const
		{
			return Vec2{
				x / rhs,
				y / rhs
			};
		}
		// Vec2 *= float
		inline const Vec2& 	operator*= ( const float& rhs )
		{
			x *= rhs;
			y *= rhs;

			return *this;
		}
		// Vec2 /= float
		inline const Vec2& 	operator/= ( const float& rhs )
		{
			x /= rhs;
			y /= rhs;

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
		float x{ 0.0f }, y{ 0.0f };
	};
}

// float * Vec2
inline adm::Vec2 operator* ( const float& lhs, const adm::Vec2& rhs )
{
	return adm::Vec2{
		rhs.x * lhs,
		rhs.y * lhs
	};
}

namespace std
{
	// Extending le standard bibliotheque to support Vec2
	inline std::string to_string( adm::Vec2 val )
	{
		// TODO: use fmtlib, sprintf is like 20x slower
		char buffer[128]; // God forbid you put such a large number to overflow this...
		snprintf( buffer, 128, "%f %f", val.x, val.y );
		return std::string( buffer );
	}

	inline adm::Vec2 fabs( const adm::Vec2& v )
	{
		return adm::Vec2{
			fabs( v.x ),
			fabs( v.y )
		};
	}
}

inline std::ostream& operator << ( std::ostream& os, const adm::Vec2& vec )
{
	os << vec.x << " " << vec.y;
	return os;
}
