#pragma once

#include "m3/gem.hpp"

namespace m3
{
  class gem_generator
  {
  public:
    explicit gem_generator( unsigned int gem_type_count );

    gem random() const;
    gem next( gem g ) const;
    
  private:
    unsigned int _gem_type_count;
  };
}
