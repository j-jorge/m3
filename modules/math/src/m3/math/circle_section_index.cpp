#include "m3/math/circle_section_index.hpp"

#include <cassert>
#include <cmath>

std::size_t m3::math::circle_section_index( float angle, std::size_t sections )
{
  static constexpr float pi_times_2( 2 * 3.14159265358979323846 );

  angle -= pi_times_2 * std::floor( angle / pi_times_2 );

  return angle / ( pi_times_2 / sections );
}

