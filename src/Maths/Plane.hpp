
#pragma once

// Based on Daniel Walder's Sledge Editor code, massive thank you <3
// https://github.com/LogicAndTrick/sledge/blob/master/Sledge.DataStructures/Geometric/Precision/Plane.cs

namespace adm
{
	// ============================
	// 3D plane
	// ============================
	class Plane final
	{
	public: // Construction
		constexpr Plane( float A, float B, float C, float D )
			: a( A ), b( B ), c( C ), d( D )
		{

		}

		// Construct a plane from a normal & distance to centre
		constexpr Plane( const Vec3& normal, const float& distance )
			: a( normal.x ), b( normal.y ), c( normal.z ), d( -distance )
		{

		}

		// Construct a plane from 3 points
		Plane( const Vec3& pa, const Vec3& pb, const Vec3& pc )
		{
			const Vec3 ab = pb - pa;
			const Vec3 ac = pc - pa;
			const Vec3 normal = ac.Cross( ab ).Normalized();

			a = normal.x;
			b = normal.y;
			c = normal.z;
			d = (normal * pa) * -1.0f;
		}

	public: // Methods - getters & setters

		inline float GetDistanceFromOrigin() const
		{
			return -d;
		}

		inline void SetDistanceFromOrigin( const float& distance )
		{
			d = -distance;
		}

		inline Vec3 GetNormal() const
		{
			return Vec3( a, b, c );
		}

		inline void SetNormal( const Vec3& normal )
		{
			a = normal.x;
			b = normal.y;
			c = normal.z;
		}

		// Point on plane
		inline Vec3 GetPointOnPlane() const
		{
			return GetNormal() * GetDistanceFromOrigin();
		}

	public: // Methods - utilities

		// Evaluate a point on a plane
		// Returns 0 or close to 0 if the point lies on the plane
		inline float EvalAtPoint( const Vec3& p ) const
		{
			return a * p.x + b * p.y + c * p.z + d;
		}

		// Is point p on the plane?
		// @returns -1 if p is under the plane, 1 if p is above the plane, 
		// 0 if p is on the plane
		inline int OnPlane( const Vec3& p, const float& epsilon = 0.0001f ) const
		{
			const float evaluation = EvalAtPoint( p );
			if ( std::fabs( evaluation ) < epsilon )
			{
				return 0;
			}

			return (evaluation < 0.0f) ? -1 : 1;
		}

		// Calculates where p would be situated on this plane's space
		inline Vec3 Project( const Vec3& p ) const
		{
			return p - ((p - GetPointOnPlane()) * GetNormal()) * GetNormal();
		}

		// Calculate a line intersection
		// @returns A bool-Vec3 pair, false if there is no intersection, true+position when there is
		std::pair<bool, Vec3> GetIntersection( const Vec3& start, const Vec3& end, bool ignoreDirection = false, bool ignoreSegment = false ) const
		{
			const Vec3 normal = GetNormal();
			const Vec3 direction = end - start;
			const float denominator = -normal * direction;
			const float numerator = normal * (start - normal * GetDistanceFromOrigin());

			if ( std::fabs( denominator ) < 0.0001f || (!ignoreDirection && denominator < 0.0f) )
			{
				return std::make_pair( false, Vec3::Zero );
			}

			const float u = numerator / denominator;

			if ( !ignoreSegment && (u < 0.0f || u > 1.0f) )
			{
				return std::make_pair( false, Vec3::Zero );
			}

			return std::make_pair( true, start + direction * u );
		}

	public: // Constants
		static const Plane Zero;

		static const Plane Forward;
		static const Plane Right;
		static const Plane Up;

	public: // Operators
		inline Plane operator* ( const float& rhs ) const
		{
			return Plane{
				a * rhs,
				b * rhs,
				c * rhs,
				d * rhs
			};
		}

		inline Plane& operator*= ( const float& rhs )
		{
			a *= rhs;
			b *= rhs;
			c *= rhs;
			d *= rhs;
			return *this;
		}

	public:
		// abc -> normal
		// d -> distance from centre
		float a{ 0.0f }, b{ 0.0f }, c{ 0.0f }, d{ 0.0f };
	};
}
