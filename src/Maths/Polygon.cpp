
#include "Precompiled.hpp"
using namespace adm;

// ============================
// Polygon::ctor
// ============================
Polygon::Polygon( const Plane& plane, float radius )
{
	const Vec3 direction = plane.GetClosestAxisToNormal();
	const Vec3 tempVec = direction == Vec3::Up ? Vec3::Right : -Vec3::Up;

	const Vec3 up = tempVec.Cross( plane.GetNormal() ).Normalized();
	const Vec3 right = plane.GetNormal().Cross( up ).Normalized();

	const Vec3 pointOnPlane = plane.GetPointOnPlane();

	const Vec3 verts[] =
	{
		pointOnPlane + right + up, // Top right
		pointOnPlane - right + up, // Top left
		pointOnPlane - right - up, // Bottom left
		pointOnPlane + right - up  // Bottom right
	};

	Vec3 origin;
	for ( const Vec3& v : verts )
	{
		origin += v;
	}
	origin /= 4.0f;

	vertices.reserve( 4U );
	for ( const Vec3& v : verts )
	{
		Vec3 t = (v - origin).Normalized() * radius + origin;
		vertices.push_back( t );
	}
}

// ============================
// Polygon::Split
// Based on Sledge Editor code
// ============================
PolygonSplitResult Polygon::Split( const Plane& plane ) const
{
	constexpr float Epsilon = FLT_EPSILON;
	PolygonSplitResult result;

	// NGL this would be more elegant if std::vector had something like
	// List.Select in C#, probably gonna write something like that eventually
	Vector<float> distances;
	distances.reserve( vertices.size() );
	for ( const auto& v : vertices )
	{
		distances.push_back( plane.EvalAtPoint( v ) );
	}

	int countBack = 0, countFront = 0;
	for ( auto& d : distances )
	{
		if ( d < -Epsilon )
			countBack++;
		else if ( d > Epsilon )
			countFront++;
		else
			d = 0.0f;
	}

	// Coplanar, no intersection occurred
	if ( !countBack && !countFront )
	{
		if ( GetPlane().GetNormal() * plane.GetNormal() > 0.0f )
			result.coplanarFront = *this;
		else
			result.coplanarBack = *this;
	}
	// All vertices in front, no intersection occurred
	else if ( !countBack )
	{
		result.front = *this;
	}
	// All vertices behind, no intersection occurred
	else if ( !countFront )
	{
		result.back = *this;
	}
	// There has been an intersection, calculate back'n'front polygons
	else
	{
		Vector<Vec3> backVertices, frontVertices;

		for ( size_t i = 0U; i < vertices.size(); i++ )
		{
			const int j = (i + 1) % vertices.size();

			const Vec3& s = vertices[i], e = vertices[j];
			const float& sd = distances[i], ed = distances[j];

			if ( sd <= 0.0f )
				backVertices.push_back( s );
			if ( sd >= 0.0f )
				frontVertices.push_back( s );

			if ( (sd < 0.0f && ed > 0.0f) || (ed < 0.0f && sd > 0.0f) )
			{
				const float t = sd / (sd - ed);
				const Vec3 intersect = s * (1.0f - t) + e * t;

				backVertices.push_back( intersect );
				frontVertices.push_back( intersect );
			}
		}

		result.back = Polygon( backVertices );
		result.front = Polygon( frontVertices );
		result.didIntersect = true;
	}

	return result;
}
