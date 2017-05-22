#include "m3/math/linear_range_interpolation.hpp"

float m3::math::linear_range_interpolation
( float t, float start, float end, float min, float max )
{
  if ( t <= start )
    return min;

  if ( t >= end )
    return max;
  
  return min + ( max - min ) * ( t - start ) / ( end - start );
}

