#include "m3/gem_ring.hpp"

m3::gem_ring::gem_ring()
  : m_launcher_angle( 0 )
{

}

float m3::gem_ring::get_launcher_angle() const
{
  return m_launcher_angle;
}

void m3::gem_ring::set_launcher_angle( float angle )
{
  m_launcher_angle = angle;
}

void m3::gem_ring::launch( gem g )
{
  m_free_gems.push_back( m_all_gems.size() );
  m_free_gem_radius.push_back( 0 );
  m_free_gem_angle.push_back( m_launcher_angle );
  
  m_all_gems.push_back( g );
}

const std::vector< m3::gem >& m3::gem_ring::gems() const
{
  return m_all_gems;
}

const std::vector< std::size_t >& m3::gem_ring::free_gems() const
{
  return m_free_gems;
}

const std::vector< float >& m3::gem_ring::free_gem_radius() const
{
  return m_free_gem_radius;
}

const std::vector< float >& m3::gem_ring::free_gem_angle() const
{
  return m_free_gem_angle;
}

