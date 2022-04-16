
#pragma once

// Based on Daniel Walder's Sledge Editor code, massive thank you <3
// https://github.com/LogicAndTrick/sledge/blob/master/Sledge.DataStructures/Geometric/Precision/Polygon.cs

namespace adm
{
	// ============================
	// 3D polygon with at least 3 vertices
	// ============================
	class Polygon final
	{
	public: // Construction
		Polygon() = default;
		~Polygon() = default;

		// Construct a polygon from existing vertices
		Polygon( std::vector<Vec3>& verts ) : vertices( verts ) {}
		
		// Construct a very large polygon from a plane
		Polygon( const Plane& plane, float radius = 1'000'000.0f );
		//  This would be the first time I ever use this ^ feature
		
	public: // Getters & setters
		// Calculates a plane based on the first 3 vertices
		Plane GetPlane() const
		{
			return Plane( vertices[0], vertices[1], vertices[2] );
		}

		// Average of all vertices
		Vec3 GetOrigin() const
		{
			Vec3 origin;
			for ( const Vec3& v : vertices )
			{
				origin += v;
			}
			return origin / float( vertices.size() );
		}

	public: // Utilities
		// A polygon is valid if it's got more than 3 vertices
		// @param requirePlanar: Modifies the definition of "valid" by  also requiring 
		// all vertices to be on the same plane (used by CSG map editors and stuff)
		bool IsValid( bool requirePlanar = false ) const
		{
			if ( vertices.size() < 3 )
			{
				return false;
			}

			if ( requirePlanar )
			{
				const Plane plane = GetPlane();
				for ( const auto& v : vertices )
				{
					if ( !plane.OnPlane( v ) )
					{
						return false;
					}
				}
			}

			return true;
		}

		// Splits the polygon by a clipping plane, returning the back and front planes
		// The original polygon is not modified
		// @returns didIntersect = true if there was an intersection with the plane
		struct PolygonSplitResult Split( const Plane& plane ) const;

	public: // Members
		std::vector<Vec3> vertices;
	};

	// Helper structure to store polygon splitting data
	// Polygon::Split returns a buncha different stuff
	struct PolygonSplitResult
	{
		bool didIntersect{ false };
		std::optional<Polygon> front;
		std::optional<Polygon> back;
		std::optional<Polygon> coplanarFront;
		std::optional<Polygon> coplanarBack;
	};
}
