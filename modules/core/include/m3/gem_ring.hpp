#pragma once

#include "m3/gem.hpp"

#include "m3/stl/vector.hpp"

namespace m3
{
  class gem_ring
  {
  public:
    gem_ring();

    float get_launcher_angle() const;
    void set_launcher_angle( float angle );

    void launch( gem g );

    const std::vector< gem >& gems() const;
    const std::vector< std::size_t >& free_gems() const;
    
  private:
    std::vector< gem > m_all_gems;
    std::vector< std::size_t > m_ring;
    std::vector< std::size_t > m_free_gems;
    std::vector< float > m_free_gem_radius;
    std::vector< float > m_free_gem_angle;

    float m_launcher_angle;
  };
}
