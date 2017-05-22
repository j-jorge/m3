#pragma once

namespace m3
{
  class gem_generator;
  class gem_ring;
  
  void random_ring_without_match
  ( gem_ring& ring, const gem_generator& generator, unsigned int count,
    unsigned int match_size );
}
