#pragma once

#include "m3/gem.hpp"

#include "m3/stl/vector.hpp"

namespace m3
{
  class gem_circle
  {
  public:
    gem_circle();

    float get_launcher_angle() const;
    void set_launcher_angle( float angle );

    std::vector< std::size_t > expand( float d );
    
    void launch( gem g );
    
  private:
    std::vector< gem > m_circle;
    std::vector< gem > m_free_gems;
    std::vector< float > m_free_gems_radius;
    std::vector< float > m_free_gems_angles;

    float m_launcher_angle;
  };
}
