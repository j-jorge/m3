#include "m3/app/gem_ring.hpp"

#include "m3/app/config/get_config.hpp"

#include "m3/find_adjacent_ring_gems.hpp"
#include "m3/math/pi_times_2.hpp"

#include "engine/level_globals.hpp"
#include "visual/scene_sprite.hpp"

#include <claw/tween/single_tweener.hpp>
#include <claw/tween/easing/easing_back.hpp>

#include <boost/bind.hpp>

BASE_ITEM_EXPORT( gem_ring, m3::app );

namespace m3
{
  namespace app
  {
    namespace detail
    {
      namespace state
      {
        static constexpr int radius_animation = 1;
        static constexpr int expand = 2;
      }

      static gem random_gem();
    }
  }
}
        
m3::app::gem_ring::gem_ring()
  : m_date( 0 ),
    m_next_launch_date( get_config< unsigned int >( "launch.first-date" ) ),
    m_coming_next_visual( bear::visual::scene_element() ),
    m_radius( 0 ),
    m_state( detail::state::expand )
{
  // TODO: use a modern random number generator
  std::srand( std::time( nullptr ) );

  set_angular_speed( get_config< float >( "initial-angular-speed" ) );
  set_friction( 1 );
}

void m3::app::gem_ring::pre_cache()
{
  super::pre_cache();

  get_level_globals().load_image( "gfx/sprites.png" );
}

void m3::app::gem_ring::build()
{
  super::build();

  fill_gem_sprites();
  initialize_ring();
  update_coming_next();
  
  enter_radius_animation_state();
}

void m3::app::gem_ring::progress( bear::universe::time_type elapsed_time )
{
  super::progress( elapsed_time );

  m_ring.set_orientation( get_system_angle() );

  switch( m_state )
    {
    case detail::state::radius_animation:
      update_radius();
      break;
    case detail::state::expand:
      update_expansion();
      break;
    }
}

void m3::app::gem_ring::enter_radius_animation_state()
{
  assert( m_state == detail::state::expand );

  m_state = detail::state::radius_animation;
  
  animate_radius_change();
}

void m3::app::gem_ring::animate_radius_change()
{
  static const float resize_duration
    ( get_config< float >( "resize-duration" ) );

  const auto update_radius_value
    ( [ this ]( float value ) -> void
      {
        m_radius = value;
      } );
                                  
  m_radius_tweener =
    claw::tween::single_tweener
    ( m_radius, get_ring_radius(), resize_duration, update_radius_value,
      &claw::tween::easing_back::ease_out );

  m_radius_tweener.on_finished
    ( boost::bind( &gem_ring::complete_radius_state, this ) );
}

void m3::app::gem_ring::update_radius()
{
  assert( m_state == detail::state::radius_animation );
  
  m_radius_tweener.update( 1 );
}

void m3::app::gem_ring::complete_radius_state()
{
  if ( remove_matches() )
    animate_radius_change();
  else
    enter_expansion_state();
}

void m3::app::gem_ring::enter_expansion_state()
{
  assert( m_state == detail::state::radius_animation );
  m_state = detail::state::expand;
}

void m3::app::gem_ring::update_expansion()
{
  assert( m_state == detail::state::expand );
  
  ++m_date;
  
  if ( m_date >= m_next_launch_date )
    {
      launch_gem();
      update_coming_next();
      update_next_launch_date();
    }

  if ( m_ring.expand( get_expansion_rate() ) != 0 )
    {
      remove_matches();
      enter_radius_animation_state();
    }
}

void m3::app::gem_ring::launch_gem()
{
  assert( m_state == detail::state::expand );
  
  const float orientation( m3::math::pi_times_2 * std::rand() / RAND_MAX );
  m_ring.launch( orientation, m_coming_next );
}

void m3::app::gem_ring::update_coming_next()
{
  const bear::universe::position_type center( get_center_of_mass() );

  m_coming_next = detail::random_gem();

  const bear::visual::sprite& s( m_gem_sprite[ m_coming_next ] );
  
  m_coming_next_visual =
    bear::visual::scene_sprite
    ( center.x - s.width() / 2, center.y - s.height() / 2, s );
}

void m3::app::gem_ring::update_next_launch_date()
{
  static const float max_interval
    ( get_config< unsigned int >( "launch-interval.max" ) );
  static const float start_date
    ( get_config< float >( "launch-speed-up.start-date" ) );

  if ( m_date < start_date )
    {
      m_next_launch_date += max_interval;
      return;
    }

  static const float min_interval
    ( get_config< unsigned int >( "launch-interval.min" ) );
  static const std::uint64_t duration
    ( get_config< unsigned int >( "launch-speed-up.duration" ) );

  if ( m_date > start_date + duration )
    {
      m_next_launch_date += min_interval;
      return;
    }
  
  m_next_launch_date +=
    max_interval
    - ( m_date - start_date ) * ( max_interval - min_interval ) / duration;
}

bool m3::app::gem_ring::remove_matches()
{
  static const std::size_t match_size
    ( get_config< unsigned int >( "minimum-match-size" ) );

  const std::vector< std::size_t > matches
    ( m3::find_adjacent_ring_gems( m_ring.chain(), match_size ) );

  m_ring.erase( matches );

  return !matches.empty();
}

float m3::app::gem_ring::get_ring_radius() const
{
  static const float gem_size( get_config< float >( "gem-size" ) );
  
  const std::vector< m3::gem >& gems( m_ring.chain() );
  const std::size_t count( gems.size() );

  return count * gem_size / m3::math::pi_times_2;
}

void m3::app::gem_ring::get_visual
( std::list< bear::engine::scene_visual >& visuals ) const
{
  super::get_visual( visuals );

  get_ring_visuals( visuals );
  get_launched_visuals( visuals );
  get_launcher_visual( visuals );
}

#include "m3/math/circle_section_index.hpp"

void m3::app::gem_ring::get_ring_visuals
( std::list< bear::engine::scene_visual >& visuals ) const
{
  const std::vector< m3::gem >& gems( m_ring.chain() );
  const float orientation( m_ring.get_orientation() );
  const std::size_t count( gems.size() );

  const bear::universe::position_type center( get_center_of_mass() );
  
  for ( std::size_t i( 0 ); i != count; ++i )
    {
      const bear::visual::sprite& s( m_gem_sprite[ gems[ i ] ] );
      const float a( orientation + i * m3::math::pi_times_2 / count );
      const float x( center.x + std::cos( a ) * m_radius - s.width() / 2 );
      const float y( center.y + std::sin( a ) * m_radius - s.height() / 2 );

      visuals.push_back( bear::visual::scene_sprite( x, y, s ) );
    }
}

void m3::app::gem_ring::get_launched_visuals
( std::list< bear::engine::scene_visual >& visuals ) const
{
  const std::vector< m3::gem >& gems( m_ring.free_gems() );
  const std::vector< float >& radius( m_ring.free_gem_radius() );
  const std::vector< float >& direction( m_ring.free_gem_direction() );

  const std::size_t count( gems.size() );
  const bear::universe::position_type center( get_center_of_mass() );

  for ( std::size_t i( 0 ); i != count; ++i )
    {
      const bear::visual::sprite& s( m_gem_sprite[ gems[ i ] ] );
      const float a( direction[ i ] );
      const float r( radius[ i ] * m_radius );
      const float x( center.x + std::cos( a ) * r - s.width() / 2 );
      const float y( center.y + std::sin( a ) * r - s.height() / 2 );

      visuals.push_back( bear::visual::scene_sprite( x, y, s ) );
    }
}

void m3::app::gem_ring::get_launcher_visual
( std::list< bear::engine::scene_visual >& visuals ) const
{
  visuals.push_back( m_coming_next_visual );
}

void m3::app::gem_ring::fill_gem_sprites()
{
  static const unsigned int gem_type_count
    ( get_config< unsigned int >( "gem-type-count" ) );
  assert( gem_type_count < 9 );
  
  m_gem_sprite.reserve( gem_type_count + 1 );

  m_gem_sprite.push_back
    ( get_level_globals().auto_sprite( "gfx/sprites.png", "gem-placeholder" ) );

  static const std::string prefix( "gem-" );
  
  for ( unsigned int i( 0 ); i != gem_type_count; ++i )
    m_gem_sprite.push_back
      ( get_level_globals().auto_sprite
        ( "gfx/sprites.png", prefix + char( '1' + i ) ) );
}

void m3::app::gem_ring::initialize_ring()
{
  static const unsigned int count
    ( get_config< unsigned int >( "initial-gem-count" ) );
  static const unsigned int gem_type_count
    ( get_config< unsigned int >( "gem-type-count" ) );
  static const unsigned int match_size
    ( get_config< unsigned int >( "minimum-match-size" ) );

  // TODO: move this in a utility function such that it can be tested.
  const m3::gem first_gem( detail::random_gem() );
  m_ring.launch( 0, first_gem );

  m3::gem last_gem( first_gem );
  unsigned int last_count( 1 );
  
  for ( unsigned int i( 0 ); i != count - 2; ++i )
    {
      m3::gem gem( detail::random_gem() );

      if ( gem != last_gem )
        {
          last_count = 1;
          last_gem = gem;
        }
      else if ( last_count + 1 != match_size )
        ++last_count;
      else
        {
          gem = ( gem + 1 ) % gem_type_count + 1;
          last_gem = gem;
        }

      m_ring.launch( 0, gem );
    }

  m3::gem final_gem( detail::random_gem() );

  if ( final_gem == first_gem )
    final_gem = ( final_gem + 1 ) % gem_type_count + 1;

  if ( final_gem == last_gem )
    final_gem = ( final_gem + 1 ) % gem_type_count + 1;
  
  m_ring.launch( 0, final_gem );
  
  m_ring.expand( 1 );
}

float m3::app::gem_ring::get_expansion_rate() const
{
  return get_expansion_speed() / m_radius;
}

float m3::app::gem_ring::get_expansion_speed() const
{
  static const float min_speed
    ( get_config< float >( "expansion.per-frame.min" ) );
  static const float start_date
    ( get_config< float >( "expansion.speed-up.start-date" ) );

  if ( m_date < start_date )
    return min_speed;
  
  static const float max_speed
    ( get_config< float >( "expansion.per-frame.max" ) );
  static const std::uint64_t duration
    ( get_config< unsigned int >( "expansion.speed-up.duration" ) );

  if ( m_date > start_date + duration )
    return max_speed;
  
  return
    min_speed + ( m_date - start_date ) * ( max_speed - min_speed ) / duration;
}

m3::gem m3::app::detail::random_gem()
{
  static const unsigned int gem_type_count
    ( get_config< unsigned int >( "gem-type-count" ) );

  return std::rand() % gem_type_count + 1;
}
