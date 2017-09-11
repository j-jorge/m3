#pragma once

#include "m3/game/gem.hpp"

#include "m3/stl/vector.hpp"

namespace m3
{
  namespace game
  {
    std::vector< std::size_t > find_adjacent_ring_gems
    ( const std::vector< gem >& ring, std::size_t range_size );
  }
}
