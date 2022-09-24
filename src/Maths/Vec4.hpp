
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
	public:
	#if ADM_USE_SSE41
		using SimdType = __m128;
	#else
		using SimdType = struct { float data[4]; };
	#endif

	public: // Construction
		Vec4() = default;
		constexpr explicit Vec4( float XYZW ) : m{ XYZW, XYZW, XYZW, XYZW } {}
		constexpr Vec4( float X, float Y, float Z, float W ) : m{ X, Y, Z, W } {}
		constexpr Vec4( const Vec2& vec, float Z = 0.0f, float W = 0.0f ) : m{ vec.x, vec.y, Z, W } {}
		constexpr Vec4( const Vec3& vec, float W = 0.0f ) : m{ vec.x, vec.y, vec.z, W } {}
		constexpr Vec4( const Vec4& v ) = default;
		Vec4( const char* string );

		// Generic 4-float array support
		constexpr Vec4( const float* vec ) : m{ vec[0], vec[1], vec[2], vec[3] } {}
		// Initialisation from SIMD value
		constexpr Vec4( SimdType simdValue ) : simdValue( simdValue ) {}

	public: // Methods
		// 4D length of this vector
		inline float 		Length() const
		{
			return std::sqrt( LengthSquared() );
		}
		inline float		LengthSquared() const
		{
			return m.x*m.x + m.y*m.y + m.z*m.z + m.w*m.w;
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
			return m.x*vec.m.x + m.y*vec.m.y + m.z*vec.m.z + m.w*vec.m.w;
		}
		// Snaps this vector to an integer grid
		inline const Vec4& 	Snap( const int& grid = 1 )
		{
			if ( grid == 1 )
			{
				m.x = int(m.x);
				m.y = int(m.y);
				m.z = int(m.z);
				m.w = int(m.w);
				return *this;
			}

			m.x = int(m.x / grid) * grid;
			m.y = int(m.y / grid) * grid;
			m.z = int(m.z / grid) * grid;
			m.w = int(m.w / grid) * grid;
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
			bool X = (m.x < vec.m.x + epsilon) && (m.x > vec.m.x - epsilon);
			bool Y = (m.y < vec.m.y + epsilon) && (m.y > vec.m.y - epsilon);
			bool Z = (m.z < vec.m.z + epsilon) && (m.z > vec.m.z - epsilon);
			bool W = (m.w < vec.m.w + epsilon) && (m.w > vec.m.w - epsilon);

			return X && Y && Z && W;
		}

	public: // Constants
		static const Vec4 Identity;
		static const Vec4 Zero;

		static const Vec4 Red;
		static const Vec4 Orange;
		static const Vec4 Yellow;
		static const Vec4 Green;
		static const Vec4 LightGreen;
		static const Vec4 Blue;
		static const Vec4 LightBlue;
		static const Vec4 Cyan;
		static const Vec4 Pink;
		static const Vec4 Purple;
		static const Vec4 Grey;
		static const Vec4 White;
		static const Vec4 Black;

	public: // Operators
		// Vec4 + Vec4 
		inline Vec4 		operator+ ( const Vec4& rhs ) const
		{
			return Vec4{
				m.x + rhs.m.x,
				m.y + rhs.m.y,
				m.z + rhs.m.z,
				m.w + rhs.m.w
			};
		}
		// Vec4 - Vec4
		inline Vec4			operator- ( const Vec4& rhs ) const
		{
			return Vec4{
				m.x - rhs.m.x,
				m.y - rhs.m.y,
				m.z - rhs.m.z,
				m.w - rhs.m.w
			};
		}
		// += Vec4
		inline const Vec4& 	operator+= ( const Vec4& rhs )
		{
			m.x += rhs.m.x;
			m.y += rhs.m.y;
			m.z += rhs.m.z;
			m.w += rhs.m.w;

			return *this;
		}
		// -= Vec4
		inline const Vec4& 	operator-= ( const Vec4& rhs )
		{
			m.x -= rhs.m.x;
			m.y -= rhs.m.y;
			m.z -= rhs.m.z;
			m.w -= rhs.m.w;

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
			return m.x == rhs.m.x && m.y == rhs.m.y && m.z == rhs.m.z && m.w == rhs.m.w;
		}
		// Vec4 = Vec4
		inline const Vec4& 	operator= ( const Vec4& rhs )
		{
			m.x = rhs.m.x;
			m.y = rhs.m.y;
			m.z = rhs.m.z;
			m.w = rhs.m.w;

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
				m.x * rhs,
				m.y * rhs,
				m.z * rhs,
				m.w * rhs
			};
		}
		// float * Vec4
		friend inline Vec4	operator* ( const float& lhs, const Vec4& rhs )
		{
			return rhs * lhs;
		}
		// Vec4 / float
		inline Vec4 		operator/ ( const float& rhs ) const
		{
			return Vec4{
				m.x / rhs,
				m.y / rhs,
				m.z / rhs,
				m.w / rhs
			};
		}
		// float / Vec4
		friend inline Vec4	operator/ ( const float& lhs, const Vec4& rhs )
		{
			return rhs / lhs;
		}
		// Vec4 *= float
		inline const Vec4& 	operator*= ( const float& rhs )
		{
			m.x *= rhs;
			m.y *= rhs;
			m.z *= rhs;
			m.w *= rhs;

			return *this;
		}
		// Vec4 /= float
		inline const Vec4& 	operator/= ( const float& rhs )
		{
			m.x /= rhs;
			m.y /= rhs;
			m.z /= rhs;
			m.w /= rhs;

			return *this;
		}
		// Generic float array support, in case someone uses this library with Quake or Half-Life
		inline operator		float* ()
		{
			return &m.x;
		}
		// Const version
		inline operator		const float*() const
		{
			return &m.x;
		}

	public:
		union
		{
			SimdType simdValue;
			struct
			{
				float x, y, z, w;
			} m;
		};
		
	};
}

namespace std
{
	// Extending le standard bibliotheque to support Vec4
	inline std::string to_string( adm::Vec4 val )
	{
		// TODO: use fmtlib, sprintf is like 20x slower
		char buffer[128]; // God forbid you put such a large number to overflow this...
		snprintf( buffer, 128, "%f %f %f %f", val.m.x, val.m.y, val.m.z, val.m.w );
		return std::string( buffer );
	}

	inline adm::Vec4 fabs( const adm::Vec4& v )
	{
		return adm::Vec4{
			fabs( v.m.x ),
			fabs( v.m.y ),
			fabs( v.m.z ),
			fabs( v.m.w )
		};
	}
}

inline std::ostream& operator << ( std::ostream& os, const adm::Vec4& vec )
{
	os << vec.m.x << " " << vec.m.y << " " << vec.m.z << " " << vec.m.w;
	return os;
}
