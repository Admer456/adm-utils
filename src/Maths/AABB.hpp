// SPDX-FileCopyrightText: 2022 Admer Šuko
// SPDX-License-Identifier: MIT

#pragma once

namespace adm
{
	// Min-max axis-aligned bounding box
	class AABB
	{
	public: // Constructors
		AABB() = default;
		AABB( const AABB& bbox ) = default;
		AABB( AABB&& bbox ) = default;
		AABB( Vec3 min, Vec3 max )
			: mins( min ), maxs( max )
		{
			if ( IsInverted() )
			{
				Fix();
			}
		}
		AABB( const Vector<Vec3>& points )
		{
			for ( const auto& point : points )
			{
				Add( point );
			}
		}

	public: // Methods
		// Expands the bbox if the point is outside of it
		inline void Add( const Vec3& point )
		{
			mins.x = std::min( mins.x, point.x );
			mins.y = std::min( mins.y, point.y );
			mins.z = std::min( mins.z, point.z );
			maxs.x = std::max( maxs.x, point.x );
			maxs.y = std::max( maxs.y, point.y );
			maxs.z = std::max( maxs.z, point.z );
		}

		inline void Fix()
		{
			if ( mins.x > maxs.x )
			{
				std::swap( mins.x, maxs.x );
			}
			if ( mins.y > maxs.y )
			{
				std::swap( mins.y, maxs.y );
			}
			if ( mins.z > maxs.z )
			{
				std::swap( mins.z, maxs.z );
			}
		}

		// Checks if a point is inside the bounding box
		inline bool IsInside( Vec3 point ) const
		{
			return point.x >= mins.x && point.y >= mins.y && point.z >= mins.z
				&& point.x <= maxs.x && point.y <= maxs.y && point.z <= maxs.z;
		}

		// Length of the 3D diagonal from mins to maxs
		inline float Diagonal() const
		{
			return (mins - maxs).Length();
		}

		// Checks if mins and maxs accidentally swapped places
		inline bool IsInverted() const
		{
			return mins.x > maxs.x || mins.y > maxs.y || mins.z > maxs.z;
		}

		// Gets the centre point between mins and maxs
		Vec3 GetCentre() const
		{
			return (mins + maxs) * 0.5f;
		}

		// Gets the extents of the box from its centre
		Vec3 GetExtents() const
		{
			return maxs - GetCentre();
		}

		// Forms a box from mins and maxs and gets all the vertices
		// Vertices are arranged as a top & bottom face, clockwise order
		Vector<Vec3> GetBoxPoints() const
		{
			return
			{
				Vec3( mins.x, mins.y, maxs.z ),
				Vec3( mins.x, maxs.y, maxs.z ),
				maxs,
				Vec3( maxs.x, mins.y, maxs.z ),

				Vec3( maxs.x, maxs.y, mins.z ),
				Vec3( maxs.x, mins.y, mins.z ),
				mins,
				Vec3( mins.x, maxs.y, mins.z ),
			};
		}

	public: // Operators
		inline AABB operator+( const AABB& bbox ) const
		{
			return AABB( *this ) += bbox;
		}

		inline AABB& operator+=( const AABB& bbox )
		{
			Add( bbox.mins );
			Add( bbox.maxs );
			return *this;
		}

		AABB& operator=( const AABB& bbox ) = default;
		AABB& operator=( AABB&& bbox ) = default;
		inline bool operator==( const AABB& bbox ) const
		{
			return mins == bbox.mins && maxs == bbox.maxs;
		}

	public: // Member variables
		Vec3 mins{ Vec3::Zero };
		Vec3 maxs{ Vec3::Zero };
	};
}
