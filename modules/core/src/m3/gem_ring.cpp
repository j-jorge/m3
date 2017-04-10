#include "m3/gem_ring.hpp"

m3::gem_ring::gem_ring()
  : m_orientation( 0 )
{

}

float m3::gem_ring::get_orientation() const
{
  return m_orientation;
}

void m3::gem_ring::set_orientation( float orientation )
{
  m_orientation = orientation;
}

void m3::gem_ring::launch( float direction, gem g )
{
  m_free_gems.push_back( m_all_gems.size() );
  m_free_gem_radius.push_back( 0 );
  m_free_gem_direction.push_back( direction );
  
  m_all_gems.push_back( g );
}

const std::vector< m3::gem >& m3::gem_ring::gems() const
{
  return m_all_gems;
}

const std::vector< std::size_t >& m3::gem_ring::chain() const
{
  return m_chain;
}

const std::vector< std::size_t >& m3::gem_ring::free_gems() const
{
  return m_free_gems;
}

const std::vector< float >& m3::gem_ring::free_gem_radius() const
{
  return m_free_gem_radius;
}

const std::vector< float >& m3::gem_ring::free_gem_direction() const
{
  return m_free_gem_direction;
}

void m3::gem_ring::expand( float d )
{
  std::size_t i( 0 );
  auto end( m_free_gem_radius.end() );
  
  for ( auto it( m_free_gem_radius.begin() ); it != end; )
    {
      float& r( *it );
      r += d;

      if ( r >= 1 )
        {
          auto gem_it( m_free_gems.begin() + i );
          
          m_chain.emplace_back( *gem_it );
          m_free_gems.erase( gem_it );
          
          m_free_gem_direction.erase( m_free_gem_direction.begin() + i );

          it = m_free_gem_radius.erase( it );
          end = m_free_gem_radius.end();
        }
      else
        {
          ++i;
          ++it;
        }
    }
}
