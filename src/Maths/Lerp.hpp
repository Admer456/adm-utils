
#pragma once

namespace adm
{
    template<typename T>
    inline T Square( const T& x )
    {
        return x * x;
    }

    // Linear interpolation between 'a' and 'b'
    // @param alpha: goes from 0.0 (a) to 1.0 (b)
    template<typename T>
    inline T Lerp( const T& a, const T& b, const float& alpha )
    {
        return (a * 1.0f - alpha) + (b * alpha);  
    }

    // Non-linear transforms

    // Damp interpolation between 'source' and 'target'
    // You can use this instead of a = a*0.9 + a*0.1
    // Doesn't require normalised values except for rate
    // @param rate: How fast to transition
    inline float Fade( const float& source, const float& target, const float& rate, const float& deltaTime )
    {
        return Lerp( source, target, std::pow( rate, deltaTime ) );
    }

    // Normalised non-linear transforms

    // Ramp, x^2
    inline float Ramp2( const float& x )
    {
        return x * x;
    }

    // Ramp, x^3
    // Steeper than Ramp2
    inline float Ramp3( const float& x )
    {
        return x * x * x;
    }

    // Smooth step with bounce
    inline float SmoothStep2Bounce( const float& x, const float& bounceFactor = 1.0f )
    {
        float rising = 2.0f * bounceFactor * (x * x);
        float reaching = -2.0f * bounceFactor * Square( x - 1.0f ) + 1.0f;

        return Lerp( rising, reaching, x );
    }

    // Smooth step
    inline float SmoothStep2( const float& x )
    {
        float rising = x * x;
        float reaching = -Square( x - 1.0f ) + 1.0f;

        return Lerp( rising, reaching, x );
    }
}
