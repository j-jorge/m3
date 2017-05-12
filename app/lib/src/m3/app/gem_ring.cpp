#include "m3/app/gem_ring.hpp"

#include "m3/app/config/get_config.hpp"

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

      // TODO: set this constant in m3::match::pi_times_2
      static constexpr float pi_times_2( 2 * 3.14159265358979323846 );

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
    ( boost::bind( &gem_ring::enter_expansion_state, this ) );
}

void m3::app::gem_ring::update_radius()
{
  assert( m_state == detail::state::radius_animation );
  
  m_radius_tweener.update( 1 );
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
    enter_radius_animation_state();
}

void m3::app::gem_ring::launch_gem()
{
  assert( m_state == detail::state::expand );
  
  const float orientation( detail::pi_times_2 * std::rand() / RAND_MAX );
  m_ring.launch( orientation, m_coming_next );
}

void m3::app::gem_ring::update_coming_next()
{
  static const float half_gem_size( get_config< float >( "gem-size" ) / 2 );
  const bear::universe::position_type center( get_center_of_mass() );

  m_coming_next = detail::random_gem();
  
  m_coming_next_visual =
    bear::visual::scene_sprite
    ( center.x - half_gem_size, center.y - half_gem_size,
      m_gem_sprite[ m_coming_next ] );
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

float m3::app::gem_ring::get_ring_radius() const
{
  static const float gem_size( get_config< float >( "gem-size" ) );
  
  const std::vector< m3::gem >& gems( m_ring.chain() );
  const std::size_t count( gems.size() );

  return count * gem_size / detail::pi_times_2;
}

void m3::app::gem_ring::get_visual
( std::list< bear::engine::scene_visual >& visuals ) const
{
  super::get_visual( visuals );

  get_ring_visuals( visuals );
  get_launched_visuals( visuals );
  get_launcher_visual( visuals );
}

void m3::app::gem_ring::get_ring_visuals
( std::list< bear::engine::scene_visual >& visuals ) const
{
  static const float gem_size( get_config< float >( "gem-size" ) );
  static const float half_gem_size( gem_size / 2 );
  
  // TODO: insert the placeholders
  const std::vector< m3::gem >& gems( m_ring.chain() );
  const std::size_t count( gems.size() );
  const float orientation( m_ring.get_orientation() );

  const bear::universe::position_type center
    ( get_center_of_mass()
      - bear::universe::position_type( half_gem_size, half_gem_size ) );
  
  for ( std::size_t i( 0 ); i != count; ++i )
    {
      const float a( orientation + i * detail::pi_times_2 / count );
      const float x( center.x + std::cos( a ) * m_radius );
      const float y( center.y + std::sin( a ) * m_radius );

      visuals.push_back
        ( bear::visual::scene_sprite( x, y, m_gem_sprite[ gems[ i ] ] ) );
    }
}

void m3::app::gem_ring::get_launched_visuals
( std::list< bear::engine::scene_visual >& visuals ) const
{
  static const float gem_size( get_config< float >( "gem-size" ) );
  static const float half_gem_size( gem_size / 2 );
  
  const std::vector< m3::gem >& gems( m_ring.free_gems() );
  const std::vector< float >& radius( m_ring.free_gem_radius() );
  const std::vector< float >& direction( m_ring.free_gem_direction() );

  const std::size_t count( gems.size() );
  const bear::universe::position_type center
    ( get_center_of_mass()
      - bear::universe::position_type( half_gem_size, half_gem_size ) );

  for ( std::size_t i( 0 ); i != count; ++i )
    {
      const float a( direction[ i ] );
      const float r( radius[ i ] * m_radius );
      const float x( center.x + std::cos( a ) * r );
      const float y( center.y + std::sin( a ) * r );

      visuals.push_back
        ( bear::visual::scene_sprite( x, y, m_gem_sprite[ gems[ i ] ] ) );
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
      m3::gem gem;
      bool found( false );
      
      while ( !found )
        {
          gem = detail::random_gem();

          if ( gem != last_gem )
            {
              last_count = 1;
              last_gem = gem;
              found = true;
            }
          else if ( last_count + 1 != match_size )
            {
              ++last_count;
              found = true;
            }
        }

      m_ring.launch( 0, gem );
    }

  m3::gem final_gem( detail::random_gem() );

  if ( final_gem == first_gem )
    final_gem = ( first_gem + 1 ) % gem_type_count + 1;

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
