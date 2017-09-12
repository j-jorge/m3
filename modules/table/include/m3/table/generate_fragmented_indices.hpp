#pragma once

#include "m3/stl/vector.hpp"

namespace m3
{
  namespace table
  {
    std::vector< std::size_t > generate_fragmented_indices
    ( std::size_t count, const std::vector< std::size_t >& skip );
  }
}
