
#pragma once

#define JPH_EL( r, c ) columns[c][r]

namespace adm
{
	// ============================
	// Mat4::Equals
	// ============================
	inline bool Mat4::Equals( const Mat4& mat, float maxDistanceSquared ) const
	{
		for ( int i = 0; i < 4; i++ )
		{
			if ( !columns[i].Equals( mat.columns[i], maxDistanceSquared ) )
			{
				return false;
			}
		}

		return true;
	}

	// ============================
	// Mat4::Mul3
	// ============================
	inline Vec3 Mat4::Mul3( const Vec3& v ) const
	{
#if ADM_USE_SSE41
		// Jolt sets Z and W to be the same, though it does not look like it matters at all here
		Vec4 v4( v, v.z );
		__m128 t = _mm_mul_ps( columns[0].simdValue, _mm_shuffle_ps( v4.simdValue, v4.simdValue, _MM_SHUFFLE( 0, 0, 0, 0 ) ) );
		t = _mm_add_ps( t, _mm_mul_ps( columns[1].simdValue, _mm_shuffle_ps( v4.simdValue, v4.simdValue, _MM_SHUFFLE( 1, 1, 1, 1 ) ) ) );
		t = _mm_add_ps( t, _mm_mul_ps( columns[2].simdValue, _mm_shuffle_ps( v4.simdValue, v4.simdValue, _MM_SHUFFLE( 2, 2, 2, 2 ) ) ) );
		return Vec3( Vec4( t ) );
#else
		return Vec3(
			columns[0][0] * v[0] + columns[1][0] * v[1] + columns[2][0] * v[2],
			columns[0][1] * v[0] + columns[1][1] * v[1] + columns[2][1] * v[2],
			columns[0][2] * v[0] + columns[1][2] * v[1] + columns[2][2] * v[2] );
#endif
	}

	// ============================
	// Mat4::Mul3Transposed
	// ============================
	inline Vec3 Mat4::Mul3Transposed( const Vec3& v ) const
	{
#if ADM_USE_SSE41
		Vec4 v4( v, v.z );
		const __m128 x = _mm_dp_ps( columns[0].simdValue, v4.simdValue, 0x7f );
		const __m128 y = _mm_dp_ps( columns[1].simdValue, v4.simdValue, 0x7f );
		const __m128 xy = _mm_blend_ps( x, y, 0b0010 );
		const __m128 z = _mm_dp_ps( columns[2].simdValue, v4.simdValue, 0x7f );
		const __m128 xyzz = _mm_blend_ps( xy, z, 0b1100 );
		return xyzz;
#else
		return Transposed3().Mul3( v );
#endif
	}

	// ============================
	// Mat4::Transposed
	// ============================
	inline Mat4 Mat4::Transposed() const
	{
		Mat4 mat( *this );
		mat.Transpose();
		return mat;
	}

	// ============================
	// Mat4::Transpsoe
	// ============================
	inline void Mat4::Transpose()
	{
#if ADM_USE_SSE41
		const __m128 tmp1 = _mm_shuffle_ps( columns[0].simdValue, columns[1].simdValue, _MM_SHUFFLE( 1, 0, 1, 0 ) );
		const __m128 tmp3 = _mm_shuffle_ps( columns[0].simdValue, columns[1].simdValue, _MM_SHUFFLE( 3, 2, 3, 2 ) );
		const __m128 tmp2 = _mm_shuffle_ps( columns[2].simdValue, columns[3].simdValue, _MM_SHUFFLE( 1, 0, 1, 0 ) );
		const __m128 tmp4 = _mm_shuffle_ps( columns[2].simdValue, columns[3].simdValue, _MM_SHUFFLE( 3, 2, 3, 2 ) );

		columns[0].simdValue = _mm_shuffle_ps( tmp1, tmp2, _MM_SHUFFLE( 2, 0, 2, 0 ) );
		columns[1].simdValue = _mm_shuffle_ps( tmp1, tmp2, _MM_SHUFFLE( 3, 1, 3, 1 ) );
		columns[2].simdValue = _mm_shuffle_ps( tmp3, tmp4, _MM_SHUFFLE( 2, 0, 2, 0 ) );
		columns[3].simdValue = _mm_shuffle_ps( tmp3, tmp4, _MM_SHUFFLE( 3, 1, 3, 1 ) );
#else
		Mat4 copy( *this );
		for ( int c = 0; c < 4; ++c )
			for ( int r = 0; r < 4; ++r )
				columns[r][c] = copy.columns[c][r];
#endif
	}

	// ============================
	// Mat4::Transposed3
	// ============================
	inline Mat4 Mat4::Transposed3() const
	{
		Mat4 mat( *this );
		mat.Transpose3();
		return mat;
	}

	// ============================
	// Mat4::Transpose3
	// ============================
	inline void Mat4::Transpose3()
	{
#if ADM_USE_SSE41
		const __m128 zero = _mm_setzero_ps();
		const __m128 tmp1 = _mm_shuffle_ps( columns[0].simdValue, columns[1].simdValue, _MM_SHUFFLE( 1, 0, 1, 0 ) );
		const __m128 tmp3 = _mm_shuffle_ps( columns[0].simdValue, columns[1].simdValue, _MM_SHUFFLE( 3, 2, 3, 2 ) );
		const __m128 tmp2 = _mm_shuffle_ps( columns[2].simdValue, zero, _MM_SHUFFLE( 1, 0, 1, 0 ) );
		const __m128 tmp4 = _mm_shuffle_ps( columns[2].simdValue, zero, _MM_SHUFFLE( 3, 2, 3, 2 ) );

		columns[0].simdValue = _mm_shuffle_ps( tmp1, tmp2, _MM_SHUFFLE( 2, 0, 2, 0 ) );
		columns[1].simdValue = _mm_shuffle_ps( tmp1, tmp2, _MM_SHUFFLE( 3, 1, 3, 1 ) );
		columns[2].simdValue = _mm_shuffle_ps( tmp3, tmp4, _MM_SHUFFLE( 2, 0, 2, 0 ) );
#else
		Mat4 copy( *this );
		for ( int c = 0; c < 3; ++c )
		{
			for ( int r = 0; r < 3; ++r )
			{
				columns[c][r] = copy.columns[r][c];
			}
			columns[c][3] = 0.0f;
		}
#endif
		columns[3] = Vec4( 0.0f, 0.0f, 0.0f, 1.0f );
	}

	// ============================
	// Mat4::Inversed
	// ============================
	inline Mat4 Mat4::Inversed() const
	{
#if ADM_USE_SSE41
		// Algorithm from: http://download.intel.com/design/PentiumIII/sml/24504301.pdf
		// Streaming SIMD Extensions - inverse of 4x4 Matrix
		// Adapted to load data using _mm_shuffle_ps instead of loading from memory
		// Replaced _mm_rcp_ps with _mm_div_ps for better accuracy

		__m128 tmp1 = _mm_shuffle_ps( columns[0].simdValue, columns[1].simdValue, _MM_SHUFFLE( 1, 0, 1, 0 ) );
		__m128 row1 = _mm_shuffle_ps( columns[2].simdValue, columns[3].simdValue, _MM_SHUFFLE( 1, 0, 1, 0 ) );
		__m128 row0 = _mm_shuffle_ps( tmp1, row1, _MM_SHUFFLE( 2, 0, 2, 0 ) );
		row1 = _mm_shuffle_ps( row1, tmp1, _MM_SHUFFLE( 3, 1, 3, 1 ) );
		tmp1 = _mm_shuffle_ps( columns[0].simdValue, columns[1].simdValue, _MM_SHUFFLE( 3, 2, 3, 2 ) );
		__m128 row3 = _mm_shuffle_ps( columns[2].simdValue, columns[3].simdValue, _MM_SHUFFLE( 3, 2, 3, 2 ) );
		__m128 row2 = _mm_shuffle_ps( tmp1, row3, _MM_SHUFFLE( 2, 0, 2, 0 ) );
		row3 = _mm_shuffle_ps( row3, tmp1, _MM_SHUFFLE( 3, 1, 3, 1 ) );

		tmp1 = _mm_mul_ps( row2, row3 );
		tmp1 = _mm_shuffle_ps( tmp1, tmp1, _MM_SHUFFLE( 2, 3, 0, 1 ) );
		__m128 minor0 = _mm_mul_ps( row1, tmp1 );
		__m128 minor1 = _mm_mul_ps( row0, tmp1 );
		tmp1 = _mm_shuffle_ps( tmp1, tmp1, _MM_SHUFFLE( 1, 0, 3, 2 ) );
		minor0 = _mm_sub_ps( _mm_mul_ps( row1, tmp1 ), minor0 );
		minor1 = _mm_sub_ps( _mm_mul_ps( row0, tmp1 ), minor1 );
		minor1 = _mm_shuffle_ps( minor1, minor1, _MM_SHUFFLE( 1, 0, 3, 2 ) );

		tmp1 = _mm_mul_ps( row1, row2 );
		tmp1 = _mm_shuffle_ps( tmp1, tmp1, _MM_SHUFFLE( 2, 3, 0, 1 ) );
		minor0 = _mm_add_ps( _mm_mul_ps( row3, tmp1 ), minor0 );
		__m128 minor3 = _mm_mul_ps( row0, tmp1 );
		tmp1 = _mm_shuffle_ps( tmp1, tmp1, _MM_SHUFFLE( 1, 0, 3, 2 ) );
		minor0 = _mm_sub_ps( minor0, _mm_mul_ps( row3, tmp1 ) );
		minor3 = _mm_sub_ps( _mm_mul_ps( row0, tmp1 ), minor3 );
		minor3 = _mm_shuffle_ps( minor3, minor3, _MM_SHUFFLE( 1, 0, 3, 2 ) );

		tmp1 = _mm_mul_ps( _mm_shuffle_ps( row1, row1, _MM_SHUFFLE( 1, 0, 3, 2 ) ), row3 );
		tmp1 = _mm_shuffle_ps( tmp1, tmp1, _MM_SHUFFLE( 2, 3, 0, 1 ) );
		row2 = _mm_shuffle_ps( row2, row2, _MM_SHUFFLE( 1, 0, 3, 2 ) );
		minor0 = _mm_add_ps( _mm_mul_ps( row2, tmp1 ), minor0 );
		__m128 minor2 = _mm_mul_ps( row0, tmp1 );
		tmp1 = _mm_shuffle_ps( tmp1, tmp1, _MM_SHUFFLE( 1, 0, 3, 2 ) );
		minor0 = _mm_sub_ps( minor0, _mm_mul_ps( row2, tmp1 ) );
		minor2 = _mm_sub_ps( _mm_mul_ps( row0, tmp1 ), minor2 );
		minor2 = _mm_shuffle_ps( minor2, minor2, _MM_SHUFFLE( 1, 0, 3, 2 ) );

		tmp1 = _mm_mul_ps( row0, row1 );
		tmp1 = _mm_shuffle_ps( tmp1, tmp1, _MM_SHUFFLE( 2, 3, 0, 1 ) );
		minor2 = _mm_add_ps( _mm_mul_ps( row3, tmp1 ), minor2 );
		minor3 = _mm_sub_ps( _mm_mul_ps( row2, tmp1 ), minor3 );
		tmp1 = _mm_shuffle_ps( tmp1, tmp1, _MM_SHUFFLE( 1, 0, 3, 2 ) );
		minor2 = _mm_sub_ps( _mm_mul_ps( row3, tmp1 ), minor2 );
		minor3 = _mm_sub_ps( minor3, _mm_mul_ps( row2, tmp1 ) );

		tmp1 = _mm_mul_ps( row0, row3 );
		tmp1 = _mm_shuffle_ps( tmp1, tmp1, _MM_SHUFFLE( 2, 3, 0, 1 ) );
		minor1 = _mm_sub_ps( minor1, _mm_mul_ps( row2, tmp1 ) );
		minor2 = _mm_add_ps( _mm_mul_ps( row1, tmp1 ), minor2 );
		tmp1 = _mm_shuffle_ps( tmp1, tmp1, _MM_SHUFFLE( 1, 0, 3, 2 ) );
		minor1 = _mm_add_ps( _mm_mul_ps( row2, tmp1 ), minor1 );
		minor2 = _mm_sub_ps( minor2, _mm_mul_ps( row1, tmp1 ) );

		tmp1 = _mm_mul_ps( row0, row2 );
		tmp1 = _mm_shuffle_ps( tmp1, tmp1, _MM_SHUFFLE( 2, 3, 0, 1 ) );
		minor1 = _mm_add_ps( _mm_mul_ps( row3, tmp1 ), minor1 );
		minor3 = _mm_sub_ps( minor3, _mm_mul_ps( row1, tmp1 ) );
		tmp1 = _mm_shuffle_ps( tmp1, tmp1, _MM_SHUFFLE( 1, 0, 3, 2 ) );
		minor1 = _mm_sub_ps( minor1, _mm_mul_ps( row3, tmp1 ) );
		minor3 = _mm_add_ps( _mm_mul_ps( row1, tmp1 ), minor3 );

		__m128 det = _mm_mul_ps( row0, minor0 );
		det = _mm_add_ps( _mm_shuffle_ps( det, det, _MM_SHUFFLE( 2, 3, 0, 1 ) ), det ); // Original code did (x + z) + (y + w), changed to (x + y) + (z + w) to match the ARM code below and make the result cross platform deterministic
		det = _mm_add_ss( _mm_shuffle_ps( det, det, _MM_SHUFFLE( 1, 0, 3, 2 ) ), det );
		det = _mm_div_ss( _mm_set_ss( 1.0f ), det );
		det = _mm_shuffle_ps( det, det, _MM_SHUFFLE( 0, 0, 0, 0 ) );

		Mat4 result;
		result.columns[0].simdValue = _mm_mul_ps( det, minor0 );
		result.columns[1].simdValue = _mm_mul_ps( det, minor1 );
		result.columns[2].simdValue = _mm_mul_ps( det, minor2 );
		result.columns[3].simdValue = _mm_mul_ps( det, minor3 );
		return result;
#else
		const float m00 = JPH_EL( 0, 0 ), m10 = JPH_EL( 1, 0 ), m20 = JPH_EL( 2, 0 ), m30 = JPH_EL( 3, 0 );
		const float m01 = JPH_EL( 0, 1 ), m11 = JPH_EL( 1, 1 ), m21 = JPH_EL( 2, 1 ), m31 = JPH_EL( 3, 1 );
		const float m02 = JPH_EL( 0, 2 ), m12 = JPH_EL( 1, 2 ), m22 = JPH_EL( 2, 2 ), m32 = JPH_EL( 3, 2 );
		const float m03 = JPH_EL( 0, 3 ), m13 = JPH_EL( 1, 3 ), m23 = JPH_EL( 2, 3 ), m33 = JPH_EL( 3, 3 );

		const float m10211120 = m10 * m21 - m11 * m20;
		const float m10221220 = m10 * m22 - m12 * m20;
		const float m10231320 = m10 * m23 - m13 * m20;
		const float m10311130 = m10 * m31 - m11 * m30;
		const float m10321230 = m10 * m32 - m12 * m30;
		const float m10331330 = m10 * m33 - m13 * m30;
		const float m11221221 = m11 * m22 - m12 * m21;
		const float m11231321 = m11 * m23 - m13 * m21;
		const float m11321231 = m11 * m32 - m12 * m31;
		const float m11331331 = m11 * m33 - m13 * m31;
		const float m12231322 = m12 * m23 - m13 * m22;
		const float m12331332 = m12 * m33 - m13 * m32;
		const float m20312130 = m20 * m31 - m21 * m30;
		const float m20322230 = m20 * m32 - m22 * m30;
		const float m20332330 = m20 * m33 - m23 * m30;
		const float m21322231 = m21 * m32 - m22 * m31;
		const float m21332331 = m21 * m33 - m23 * m31;
		const float m22332332 = m22 * m33 - m23 * m32;

		Vec4 col0( m11 * m22332332 - m12 * m21332331 + m13 * m21322231, -m10 * m22332332 + m12 * m20332330 - m13 * m20322230, m10 * m21332331 - m11 * m20332330 + m13 * m20312130, -m10 * m21322231 + m11 * m20322230 - m12 * m20312130 );
		Vec4 col1( -m01 * m22332332 + m02 * m21332331 - m03 * m21322231, m00 * m22332332 - m02 * m20332330 + m03 * m20322230, -m00 * m21332331 + m01 * m20332330 - m03 * m20312130, m00 * m21322231 - m01 * m20322230 + m02 * m20312130 );
		Vec4 col2( m01 * m12331332 - m02 * m11331331 + m03 * m11321231, -m00 * m12331332 + m02 * m10331330 - m03 * m10321230, m00 * m11331331 - m01 * m10331330 + m03 * m10311130, -m00 * m11321231 + m01 * m10321230 - m02 * m10311130 );
		Vec4 col3( -m01 * m12231322 + m02 * m11231321 - m03 * m11221221, m00 * m12231322 - m02 * m10231320 + m03 * m10221220, -m00 * m11231321 + m01 * m10231320 - m03 * m10211120, m00 * m11221221 - m01 * m10221220 + m02 * m10211120 );

		float det = m00 * col0[0] + m01 * col0[1] + m02 * col0[2] + m03 * col0[3];

		return Mat4( col0 / det, col1 / det, col2 / det, col3 / det );
#endif
	}

	// ============================
	// Mat4::operator== Mat4
	// ============================
	inline bool Mat4::operator== ( const Mat4& rhs ) const
	{
		// TODO: this can be SIMD'ed
		for ( int i = 0; i < 4; i++ )
		{
			if ( columns[i] != rhs.columns[i] )
			{
				return false;
			}
		}

		return true;
	}

	// ============================
	// Mat4::operator* Mat4
	// ============================
	inline Mat4 Mat4::operator* ( const Mat4& rhs ) const
	{
		Mat4 result;
#if ADM_USE_SSE41
		for ( int i = 0; i < 4; ++i )
		{
			const __m128 c = rhs.columns[i].simdValue;
			__m128 t = _mm_mul_ps( columns[0].simdValue, _mm_shuffle_ps( c, c, _MM_SHUFFLE( 0, 0, 0, 0 ) ) );
			t = _mm_add_ps( t, _mm_mul_ps( columns[1].simdValue, _mm_shuffle_ps( c, c, _MM_SHUFFLE( 1, 1, 1, 1 ) ) ) );
			t = _mm_add_ps( t, _mm_mul_ps( columns[2].simdValue, _mm_shuffle_ps( c, c, _MM_SHUFFLE( 2, 2, 2, 2 ) ) ) );
			t = _mm_add_ps( t, _mm_mul_ps( columns[3].simdValue, _mm_shuffle_ps( c, c, _MM_SHUFFLE( 3, 3, 3, 3 ) ) ) );
			result.columns[i].simdValue = t;
		}
#else
		for ( int i = 0; i < 4; ++i )
			result.columns[i] = columns[0] * rhs.columns[i][0]
				+ columns[1] * rhs.columns[i][1]
				+ columns[2] * rhs.columns[i][2]
				+ columns[3] * rhs.columns[i][3];
#endif
		return result;
	}

	// ============================
	// Mat4::operator* Vec3
	// ============================
	inline Vec3 Mat4::operator* ( const Vec3& rhs ) const
	{
#if ADM_USE_SSE41
		const Vec4 v4( rhs, rhs.z );
		__m128 t = _mm_mul_ps( columns[0].simdValue, _mm_shuffle_ps( v4.simdValue, v4.simdValue, _MM_SHUFFLE( 0, 0, 0, 0 ) ) );
		t = _mm_add_ps( t, _mm_mul_ps( columns[1].simdValue, _mm_shuffle_ps( v4.simdValue, v4.simdValue, _MM_SHUFFLE( 1, 1, 1, 1 ) ) ) );
		t = _mm_add_ps( t, _mm_mul_ps( columns[2].simdValue, _mm_shuffle_ps( v4.simdValue, v4.simdValue, _MM_SHUFFLE( 2, 2, 2, 2 ) ) ) );
		t = _mm_add_ps( t, columns[3].simdValue );
		return Vec3( Vec4( t ) );
#else
		return Vec3(
			columns[0][0] * rhs[0] + columns[1][0] * rhs[1] + columns[2][0] * rhs[2] + columns[3][0],
			columns[0][1] * rhs[0] + columns[1][1] * rhs[1] + columns[2][1] * rhs[2] + columns[3][1],
			columns[0][2] * rhs[0] + columns[1][2] * rhs[1] + columns[2][2] * rhs[2] + columns[3][2] );
#endif
	}

	// ============================
	// Mat4::operator* Vec4
	// ============================
	inline Vec4 Mat4::operator* ( const Vec4& rhs ) const
	{
#if ADM_USE_SSE41
		__m128 t = _mm_mul_ps( columns[0].simdValue, _mm_shuffle_ps( rhs.simdValue, rhs.simdValue, _MM_SHUFFLE( 0, 0, 0, 0 ) ) );
		t = _mm_add_ps( t, _mm_mul_ps( columns[1].simdValue, _mm_shuffle_ps( rhs.simdValue, rhs.simdValue, _MM_SHUFFLE( 1, 1, 1, 1 ) ) ) );
		t = _mm_add_ps( t, _mm_mul_ps( columns[2].simdValue, _mm_shuffle_ps( rhs.simdValue, rhs.simdValue, _MM_SHUFFLE( 2, 2, 2, 2 ) ) ) );
		t = _mm_add_ps( t, _mm_mul_ps( columns[3].simdValue, _mm_shuffle_ps( rhs.simdValue, rhs.simdValue, _MM_SHUFFLE( 3, 3, 3, 3 ) ) ) );
		return t;
#else
		return Vec4(
			columns[0][0] * rhs[0] + columns[1][0] * rhs[1] + columns[2][0] * rhs[2] + columns[3][0] * rhs[3],
			columns[0][1] * rhs[0] + columns[1][1] * rhs[1] + columns[2][1] * rhs[2] + columns[3][1] * rhs[3],
			columns[0][2] * rhs[0] + columns[1][2] * rhs[1] + columns[2][2] * rhs[2] + columns[3][2] * rhs[3],
			columns[0][3] * rhs[0] + columns[1][3] * rhs[1] + columns[2][3] * rhs[2] + columns[3][3] * rhs[3] );
#endif
	}

	// ============================
	// Mat4::operator* float
	// ============================
	inline Mat4 Mat4::operator* ( float rhs ) const
	{
		Mat4 t( *this );
		t *= rhs;
		return t;
	}

	// ============================
	// Mat4::operator*= float
	// ============================
	inline Mat4& Mat4::operator*= ( float rhs )
	{
#if ADM_USE_SSE41
		const Vec4 rhsSimd( rhs );
		for ( int i = 0; i < 4; i++ )
		{
			columns[i].simdValue = _mm_mul_ps( columns[i].simdValue, rhsSimd.simdValue );
		}
#else
		for ( int i = 0; i < 4; i++ )
		{
			columns[i] *= rhs;
		}
#endif
	}

	// ============================
	// Mat4::operator+ Mat4
	// ============================
	inline Mat4 Mat4::operator+ ( const Mat4& rhs ) const
	{
		Mat4 t( *this );
		t += rhs;
		return t;
	}

	// ============================
	// Mat4::operator+= Mat4
	// ============================
	inline Mat4& Mat4::operator+= ( const Mat4& rhs )
	{
		for ( int i = 0; i < 4; i++ )
		{
			columns[i] += rhs.columns[i];
		}
	}

	// ============================
	// Mat4::operator- Mat4
	// ============================
	inline Mat4 Mat4::operator- ( const Mat4& rhs ) const
	{
		Mat4 t( *this );
		t -= rhs;
		return t;
	}

	// ============================
	// Mat4::operator-= Mat4
	// ============================
	inline Mat4& Mat4::operator-= ( const Mat4& rhs )
	{
		for ( int i = 0; i < 4; i++ )
		{
			columns[i] -= rhs.columns[i];
		}
	}

	// ============================
	// Mat4::operator-
	// ============================
	inline Mat4 Mat4::operator- () const
	{
		return Mat4( -columns[0], -columns[1], -columns[2], -columns[3] );
	}
}

#undef JPH_EL
