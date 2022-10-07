// SPDX-FileCopyrightText: 2022 Admer Šuko
// SPDX-License-Identifier: MIT

#pragma once

// This is based on JoltPhysics' Mat44 class:
// https://github.com/jrouwe/JoltPhysics/blob/master/Jolt/Math/Mat44.h
// Licenced under MIT, copyright by Jorrit Rouwe
// 
// Generally any code involving SSE was directly copied from it, while
// the rest was used as a reference. You may see some things are entirely unchanged,
// like JPH_EL, and the original comments for the algorithms & formulae.

namespace adm
{
	class Vec4;

	// ============================
	// 4x4 matrix class for transformations, perspective etc.
	// ============================
	class alignas(16) Mat4 final
	{
	public:
		using SimdType = Vec4::SimdType;

	public: // Construction
		// Vec4 does not default-initialise stuff, and so,
		// neither will we here for performance reasons
		Mat4() = default;
		constexpr Mat4( float n ) : columns{ Vec4{ n }, Vec4{ n }, Vec4{ n }, Vec4{ n } } {}
		constexpr Mat4( Vec4 c0, Vec4 c1, Vec4 c2, Vec4 c3 ) : columns{ c0, c1, c2, c3 } {}
		constexpr Mat4( SimdType c0, SimdType c1, SimdType c2, SimdType c3 ) : columns{ c0, c1, c2, c3 } {}
		constexpr Mat4( const Mat4& mat ) = default;

	public: // Construction methods
		// Constructs a perspective projection matrix
		static Mat4				Perspective( float fovY, float aspectRatio, float zNear, float zFar );
		// Constructs an orthographic projection matrix
		static Mat4				Orthographic( float left, float right, float bottom, float top, float zNear, float zFar );
		// Constructs a view matrix
		// @param position: XYZ position in space
		// @param angles: Euler angles in pitch, yaw, roll
		// TODO: we need an Angles class or Euler whatever
		static Mat4				View( const Vec3& position, const Vec3& angles );
		//static Mat4			View( const Vec3& position, const Quat& orientation );

	public: // Methods
		// Comparison with an epsilon
		inline bool				Equals( const Mat4& mat, float maxDistanceSquared = 1.0e-12f ) const;
		// Multiply vector by only 3x3 part of the matrix
		inline Vec3				Mul3( const Vec3& v ) const;
		// Multiply vector by only 3x3 part of the transpose of the matrix
		inline Vec3				Mul3Transposed( const Vec3& v ) const;

		// Returns a transposed version of the matrix
		inline Mat4				Transposed() const;
		// Transposes the matrix
		inline void				Transpose();
		// Returns an inverse version of the matrix
		inline Mat4				Inversed() const;

		// Returns a transposed version of the 3x3 matrix part
		inline Mat4				Transposed3() const;
		// Transposes the 3x3 matrix part
		inline void				Transpose3();

	public: // Constants
		static const Mat4		Identity;
		static const Mat4		One;
		static const Mat4		Zero;

	public: // Operators
		inline const float&		operator() ( uint32_t row, uint32_t column ) const { return columns[column & 3][row & 3]; }
		inline float&			operator() ( uint32_t row, uint32_t column ) { return columns[column & 3][row & 3]; }

		inline bool				operator== ( const Mat4& rhs ) const;
		inline bool				operator!= ( const Mat4& rhs ) const { return !(*this == rhs); }

		// Mat4 * Mat4
		// Since matrix multiplication isn't commutative, we do not include a *= operator
		inline Mat4				operator* ( const Mat4& rhs ) const;
		
		// Mat4 * Vec3
		// Also look at Mul3
		inline Vec3				operator* ( const Vec3& rhs ) const;
		// Mat4 * Vec4
		inline Vec4				operator* ( const Vec4& rhs ) const;
		// Mat4 * float
		inline Mat4				operator* ( float rhs ) const;
		// float * Mat4
		friend inline Mat4		operator* ( float lhs, const Mat4& rhs ) { return rhs * lhs; }
		// Mat4 *= float
		inline Mat4&			operator*= ( float lhs );
		// Per-element addition
		inline Mat4				operator+ ( const Mat4& rhs ) const;
		inline Mat4&			operator+= ( const Mat4& rhs );
		// Per-element subtraction
		inline Mat4				operator- ( const Mat4& rhs ) const;
		inline Mat4&			operator-= ( const Mat4& rhs );
		// Negate
		inline Mat4				operator- () const;

	public:
		Vec4					columns[4];
	};
}

// Inline implementations
#include "Mat4.inl"
