#pragma once

#include "m3/gem.hpp"

#include "m3/stl/vector.hpp"

namespace m3
{
  class gem_ring
  {
  public:
    gem_ring();

    float get_orientation() const;
    void set_orientation( float orientation );

    void launch( float direction, gem g );

    const std::vector< gem >& chain() const;

    const std::vector< gem >& free_gems() const;
    const std::vector< float >& free_gem_radius() const;
    const std::vector< float >& free_gem_direction() const;

    std::size_t expand( float d );
    
  private:
    std::size_t direction_to_chain_index( float d ) const;
    
  private:
    std::vector< gem > m_chain;
    
    std::vector< gem > m_free_gems;
    std::vector< float > m_free_gem_radius;
    std::vector< float > m_free_gem_direction;

    float m_orientation;
  };
}
