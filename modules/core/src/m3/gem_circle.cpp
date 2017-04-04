#include "m3/gem_circle.hpp"

m3::gem_circle::gem_circle()
  : m_launcher_angle( 0 )
{

}

float m3::gem_circle::get_launcher_angle() const
{
  return m_launcher_angle;
}

void m3::gem_circle::set_launcher_angle( float angle )
{
  m_launcher_angle = angle;
}

std::vector< std::size_t > m3::gem_circle::expand( float d )
{
  return std::vector< std::size_t >();
}

    
void m3::gem_circle::launch( gem g )
{

}
