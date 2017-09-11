#include "m3/game/game_loop.hpp"

#include "m3/game/gem_ring.hpp"
#include "m3/game/find_adjacent_ring_gems.hpp"
#include "m3/game/random_ring_without_match.hpp"
#include "m3/math/linear_range_interpolation.hpp"
#include "m3/math/pi_times_2.hpp"

m3::game::game_loop::game_loop( gem_ring& ring, const gem_generator& generator )
  : m_ring( ring ),
    m_generator( generator ),
    m_next_launch_date( 0 )
{

}

boost::signals2::connection m3::game::game_loop::connect_to_destroyed
( const std::function< void( const std::vector< std::size_t >& ) >& f )
{
  return m_destroyed.connect( f );
}
  
boost::signals2::connection m3::game::game_loop::connect_to_inserted
( const std::function< void() >& f )
{
  return m_inserted.connect( f );
}
  
boost::signals2::connection m3::game::game_loop::connect_to_launcher_updated
( const std::function< void() >& f )
{
  return m_launcher_updated.connect( f );
}
  
const std::vector< m3::game::gem >& m3::game::game_loop::coming_next() const
{
  return m_coming_next;
}

unsigned int m3::game::game_loop::date() const
{
  return m_date;
}

void m3::game::game_loop::launch_speed_up_time_range
( unsigned int start_date, unsigned int end_date )
{
  assert( start_date <= end_date );
  
  m_launch_speed_up_start_date = start_date;
  m_launch_speed_up_end_date = end_date;
}

void
m3::game::game_loop::launch_interval_range( unsigned int min, unsigned int max )
{
  assert( min <= max );
  m_launch_interval_min = min;
  m_launch_interval_max = max;
}

void m3::game::game_loop::launch_count_increment_time_range
( unsigned int start_date, unsigned int end_date )
{
  assert( start_date <= end_date );
  
  m_launch_count_increment_start_date = start_date;
  m_launch_count_increment_end_date = end_date;
}

void m3::game::game_loop::launch_count_interval_range
( unsigned int min, unsigned int max )
{
  assert( min <= max );
  m_launch_count_interval_min = min;
  m_launch_count_interval_max = max;
}

void m3::game::game_loop::match_size( unsigned int size )
{
  assert( size >= 3 );
  m_match_size = size;
}

unsigned int m3::game::game_loop::match_size() const
{
  return m_match_size;
}

void m3::game::game_loop::start( unsigned int initial_gem_count )
{
  m_date = 0;
  update_next_launch_date();
  
  random_ring_without_match
    ( m_ring, m_generator, initial_gem_count, m_match_size );

  update_coming_next();
}

void m3::game::game_loop::tick( float expansion_rate )
{
  const std::vector< std::size_t > matches
    ( find_adjacent_ring_gems( m_ring.chain(), m_match_size ) );

  if ( !matches.empty() )
    {
      m_ring.erase( matches );
      m_destroyed( matches );
    }
  else
    update( expansion_rate );
}

void m3::game::game_loop::update( float expansion_rate )
{
  ++m_date;

  if ( m_date >= m_next_launch_date )
    {
      launch_gem();
      update_next_launch_date();
      update_coming_next();
    }

  if ( !m_ring.expand( expansion_rate ).empty() )
    m_inserted();
}

void m3::game::game_loop::launch_gem()
{
  for( gem g : m_coming_next )
    {
      const float orientation( m3::math::pi_times_2 * std::rand() / RAND_MAX );
      m_ring.launch( orientation, g );
    }
  
  m_coming_next.clear();
}

void m3::game::game_loop::update_coming_next()
{
  assert( m_coming_next.empty() );

  const std::size_t count( launch_count() );
  for ( std::size_t i( 0 ); i != count; ++i )
    m_coming_next.push_back( m_generator.random() );

  m_launcher_updated();
}

std::size_t m3::game::game_loop::launch_count() const
{
  return math::linear_range_interpolation
    ( m_date, m_launch_count_increment_start_date,
      m_launch_count_increment_end_date, m_launch_count_interval_min,
      m_launch_count_interval_max );
}

void m3::game::game_loop::update_next_launch_date()
{
  m_next_launch_date +=
    math::linear_range_interpolation
    ( m_date, m_launch_speed_up_start_date, m_launch_speed_up_end_date,
      m_launch_interval_max, m_launch_interval_min );
}
