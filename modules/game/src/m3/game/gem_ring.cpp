#include "m3/game/gem_ring.hpp"

#include "m3/math/circle_section_index.hpp"
#include "m3/math/pi_times_2.hpp"

#include <algorithm>
#include <cassert>

m3::game::gem_ring::gem_ring()
  : m_orientation( 0 )
{

}

float m3::game::gem_ring::get_orientation() const
{
  return m_orientation;
}

void m3::game::gem_ring::set_orientation( float orientation )
{
  m_orientation = orientation;
}

void m3::game::gem_ring::launch( float direction, gem g )
{
  m_free_gems.push_back( g );
  m_free_gem_radius.push_back( 0 );
  m_free_gem_direction.push_back( direction );
}

void m3::game::gem_ring::erase( const std::vector< std::size_t >& indices )
{
  assert( std::is_sorted( indices.begin(), indices.end() ) );

  const auto end( indices.rend() );

  for( auto it( indices.rbegin() ); it != end; ++it )
    m_chain.erase( m_chain.begin() + *it );
}

const std::vector< m3::game::gem >& m3::game::gem_ring::chain() const
{
  return m_chain;
}

const std::vector< m3::game::gem >& m3::game::gem_ring::free_gems() const
{
  return m_free_gems;
}

const std::vector< float >& m3::game::gem_ring::free_gem_radius() const
{
  return m_free_gem_radius;
}

const std::vector< float >& m3::game::gem_ring::free_gem_direction() const
{
  return m_free_gem_direction;
}

std::vector< std::size_t > m3::game::gem_ring::expand( float d )
{
  std::size_t i( 0 );
  auto end( m_free_gem_radius.end() );
  std::vector< std::size_t > result;
  
  for ( auto it( m_free_gem_radius.begin() ); it != end; )
    {
      float& r( *it );
      r += d;

      if ( r >= 1 )
        {
          const auto gem_it( m_free_gems.begin() + i );
          const auto gem_direction( m_free_gem_direction.begin() + i );

          const std::size_t index( direction_to_chain_index( *gem_direction ) );

          for ( std::size_t& inserted : result )
            inserted += ( inserted >= index );
      
          result.push_back( index );
          
          m_chain.insert( m_chain.begin() + index, *gem_it );
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

  return result;
}

std::size_t m3::game::gem_ring::direction_to_chain_index( float d ) const
{
  return m3::math::circle_section_index( d - m_orientation, m_chain.size() );
}
