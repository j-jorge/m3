#include "m3/math/circle_section_index.hpp"

#include "m3/math/pi_times_2.hpp"

#include <cassert>
#include <cmath>

std::size_t m3::math::circle_section_index( float angle, std::size_t sections )
{
  if ( sections == 0 )
    return 0;
  
  angle -= pi_times_2 * std::floor( angle / pi_times_2 );
  
  return angle / ( pi_times_2 / sections ) + 1;
}

