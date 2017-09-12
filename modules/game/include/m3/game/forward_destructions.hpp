#pragma once

#include "m3/stl/vector.hpp"

namespace m3
{
  namespace game
  {
    class gem_ring;

    std::vector< bool > forward_destructions
    ( const gem_ring& source, unsigned int match_size );
  }
}
