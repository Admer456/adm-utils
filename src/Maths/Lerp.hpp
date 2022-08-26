
#pragma once

namespace adm
{
	template<typename T>
	constexpr T Square( const T& x )
	{
		return x * x;
	}

	template<typename T>
	constexpr T Cube( const T& x )
	{
		return x * x * x;
	}

	// Linear interpolation between 'a' and 'b'
	// @param alpha: Blend factor, from 0.0 (a) to 1.0 (b)
	template<typename T>
	constexpr T Lerp( const T& a, const T& b, const float& alpha )
	{
		return (a * (1.0f - alpha)) + (b * alpha);
	}

	// Non-linear transforms

	// Damp interpolation between 'source' and 'target'
	// You can use this instead of a = a*0.9 + b*0.1
	// @param source: The source value
	// @param target: The target to eventually reach
	// @param rate: How fast to transition [0,1]
	template<typename T>
	constexpr T Fade( const T& source, const T& target, const float& rate, const float& deltaTime )
	{
		return Lerp( target, source, std::pow( rate, deltaTime ) );
	}

	// Normalised non-linear transforms
	// Todo: links to graphs for each of these

	// Ramp, x^2
	template<typename T>
	constexpr T Ramp2( const T& x )
	{
		return x * x;
	}

	// Ramp, x^3
	// Steeper than Ramp2
	template<typename T>
	constexpr T Ramp3( const T& x )
	{
		return x * x * x;
	}

	// Smooth step with bounce
	template<typename T, typename T2>
	inline T SmoothStep2Bounce( const T& x, const T2& bounceFactor = 1.0f )
	{
		const T rising = 2.0f * bounceFactor * (x * x);
		const T reaching = -2.0f * bounceFactor * Square( x - 1.0f ) + 1.0f;

		return Lerp( rising, reaching, x );
	}

	// Smooth step
	template<typename T>
	inline float SmoothStep2( const T& x )
	{
		const T rising = Square( x );
		const T reaching = -Square( x - 1.0f ) + 1.0f;

		return Lerp( rising, reaching, x );
	}
}
