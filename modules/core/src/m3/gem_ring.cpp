#include "m3/gem_ring.hpp"

#include "m3/math/circle_section_index.hpp"
#include "m3/math/pi_times_2.hpp"

#include <cassert>

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
  m_free_gems.push_back( g );
  m_free_gem_radius.push_back( 0 );
  m_free_gem_direction.push_back( direction );
}

const std::vector< m3::gem >& m3::gem_ring::chain() const
{
  return m_chain;
}

const std::vector< m3::gem >& m3::gem_ring::free_gems() const
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

std::size_t m3::gem_ring::expand( float d )
{
  std::size_t i( 0 );
  auto end( m_free_gem_radius.end() );
  const std::size_t initial_size( m_chain.size() );
  
  for ( auto it( m_free_gem_radius.begin() ); it != end; )
    {
      float& r( *it );
      r += d;

      if ( r >= 1 )
        {
          const auto gem_it( m_free_gems.begin() + i );
          const auto gem_direction( m_free_gem_direction.begin() + i );
          
          m_chain.insert
            ( m_chain.begin()
              + direction_to_chain_index( *gem_direction ) ,
              *gem_it );
          m_free_gems.erase( gem_it );
          
          m_free_gem_direction.erase( gem_direction );

          it = m_free_gem_radius.erase( it );
          end = m_free_gem_radius.end();
        }
      else
        {
          ++i;
          ++it;
        }
    }

  assert( initial_size <= m_chain.size() );
  return m_chain.size() - initial_size;
}

std::size_t m3::gem_ring::direction_to_chain_index( float d ) const
{
  return m3::math::circle_section_index( d - m_orientation, m_chain.size() );
}
