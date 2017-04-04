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

void m3::gem_circle::launch( gem g )
{
  m_free_gems.push_back( m_all_gems.size() );
  m_free_gem_radius.push_back( 0 );
  m_free_gem_angle.push_back( m_launcher_angle );
  
  m_all_gems.push_back( g );
}

const std::vector< m3::gem >& m3::gem_circle::gems() const
{
  return m_all_gems;
}

const std::vector< std::size_t >& m3::gem_circle::free_gems() const
{
  return m_free_gems;
}

