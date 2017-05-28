#include "m3/app/gem_ring.hpp"

#include "m3/app/config/get_config.hpp"

#include "m3/find_adjacent_ring_gems.hpp"
#include "m3/math/linear_range_interpolation.hpp"
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
    }
  }
}
        
m3::app::gem_ring::gem_ring()
  : m_game_loop
    ( m_ring,
      m3::gem_generator( get_config< unsigned int >( "gem-type-count" ) ) ),
    m_radius( 0 ),
    m_state( detail::state::expand )
{
  // TODO: use a modern random number generator
  std::srand( std::time( nullptr ) );

  set_angular_speed( get_config< float >( "initial-angular-speed" ) );
  set_friction( 1 );

  m_game_loop.connect_to_launcher_updated
    ( boost::bind( &gem_ring::set_launcher_visuals, this ) );
  m_game_loop.connect_to_destroyed
    ( boost::bind( &gem_ring::enter_radius_animation_state, this ) );
  m_game_loop.connect_to_inserted
    ( boost::bind( &gem_ring::enter_radius_animation_state, this ) );

  configure_game_loop();
}

m3::app::gem_ring::gem_ring( const gem_ring& that )
  : gem_ring()
{
  assert( false );
}

float m3::app::gem_ring::get_radius() const
{
  return m_radius;
}
      
boost::signals2::connection m3::app::gem_ring::connect_to_radius_updated
( const std::function< void() >& f )
{
  return m_radius_updated.connect( f );
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
  m_game_loop.start( get_config< unsigned int >( "initial-gem-count" ) );
  
  enter_radius_animation_state();
}

void m3::app::gem_ring::progress( bear::universe::time_type elapsed_time )
{
  super::progress( elapsed_time );

  m_ring.set_orientation( get_system_angle() );
  update_glow();
  
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

void m3::app::gem_ring::get_visual
( std::list< bear::engine::scene_visual >& visuals ) const
{
  super::get_visual( visuals );

  get_ring_visuals( visuals );
  get_launched_visuals( visuals );
  get_launcher_visual( visuals );
}

void m3::app::gem_ring::configure_game_loop()
{
  m_game_loop.launch_interval_range
    ( get_config< unsigned int >( "launch-interval.min" ),
      get_config< unsigned int >( "launch-interval.max" ) );
  
  m_game_loop.launch_speed_up_time_range
    ( get_config< float >( "launch-speed-up.start-date" ),
      get_config< float >( "launch-speed-up.end-date" ) );

  m_game_loop.match_size
    ( get_config< unsigned int >( "minimum-match-size" ) );
}

void m3::app::gem_ring::fill_gem_sprites()
{
  static const unsigned int gem_type_count
    ( get_config< unsigned int >( "gem-type-count" ) );
  assert( gem_type_count < 9 );

  fill_default_gem_sprites( gem_type_count );
  fill_glowing_gem_sprites( gem_type_count );
}

void m3::app::gem_ring::fill_default_gem_sprites( unsigned int gem_type_count )
{
  m_gem_sprite.reserve( gem_type_count + 1 );

  m_gem_sprite.push_back
    ( get_level_globals().auto_sprite( "gfx/sprites.png", "gem-placeholder" ) );

  static const std::string prefix( "gem-" );
  
  for ( unsigned int i( 0 ); i != gem_type_count; ++i )
    m_gem_sprite.push_back
      ( get_level_globals().auto_sprite
        ( "gfx/sprites.png", prefix + char( '0' + i ) ) );
}

void m3::app::gem_ring::fill_glowing_gem_sprites( unsigned int gem_type_count )
{
  m_glow_sprite.reserve( gem_type_count + 1 );

  m_glow_sprite.push_back
    ( get_level_globals().auto_sprite( "gfx/sprites.png", "gem-placeholder" ) );

  static const std::string prefix( "gem-" );
  static const std::string suffix( "-glow" );
  
  for ( unsigned int i( 0 ); i != gem_type_count; ++i )
    m_glow_sprite.push_back
      ( get_level_globals().auto_sprite
        ( "gfx/sprites.png", prefix + char( '0' + i ) + suffix ) );
}

void m3::app::gem_ring::get_ring_visuals
( std::list< bear::engine::scene_visual >& visuals ) const
{
  const std::vector< m3::gem >& gems( m_ring.chain() );
  const float orientation( m_ring.get_orientation() );
  const std::size_t count( gems.size() );

  std::vector< const bear::visual::sprite* > sprites( count );
  std::vector< const bear::visual::sprite* > glow_sprites( count );
  std::vector< float > center;
  center.reserve( 2 * count );
  const bear::universe::position_type ring_center( get_center_of_mass() );
  
  for ( std::size_t i( 0 ); i != count; ++i )
    {
      const std::size_t gem( gems[ i ] );

      sprites[ i ] = &m_gem_sprite[ gem ];
      glow_sprites[ i ] = &m_glow_sprite[ gem ];
      
      const float a( orientation + i * m3::math::pi_times_2 / count );

      center.push_back( ring_center.x + std::cos( a ) * m_radius );
      center.push_back( ring_center.y + std::sin( a ) * m_radius );
    }

  for ( std::size_t i( 0 ), j( 0 ); i != count; ++i, j += 2 )
    {
      const bear::visual::sprite& s( *sprites[ i ] );
      const float x( center[ j ] - s.width() / 2 );
      const float y( center[ j + 1 ] - s.height() / 2 );

      visuals.push_back( bear::visual::scene_sprite( x, y, s ) );
    }

  for ( std::size_t i( 0 ), j( 0 ); i != count; ++i, j += 2 )
    if ( m_glowing[ i ] )
      {
        const bear::visual::sprite& s( *glow_sprites[ i ] );
        const float x( center[ j ] - s.width() / 2 );
        const float y( center[ j + 1 ] - s.height() / 2 );

        bear::visual::scene_sprite element( x, y, s );
        element.get_rendering_attributes().set_opacity( m_glow[ i ] > 0 );
        visuals.push_back( element );
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
  visuals.insert
    ( visuals.end(), m_coming_next_visuals.begin(),
      m_coming_next_visuals.end() );
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
  enter_expansion_state();
  m_radius_updated();
}

void m3::app::gem_ring::enter_expansion_state()
{
  assert( m_state == detail::state::radius_animation );
  m_state = detail::state::expand;
}

void m3::app::gem_ring::update_expansion()
{
  assert( m_state == detail::state::expand );

  m_game_loop.tick( get_expansion_rate() );
}

void m3::app::gem_ring::set_launcher_visuals()
{
  const bear::universe::position_type center( get_center_of_mass() );

  m_coming_next_visuals.clear();

  for ( const m3::gem& g : m_game_loop.coming_next() )
    {
      const bear::visual::sprite& s( m_gem_sprite[ g ] );
  
      m_coming_next_visuals.emplace_back
        ( bear::visual::scene_sprite
          ( center.x - s.width() / 2, center.y - s.height() / 2, s ) );
    }
}

float m3::app::gem_ring::get_ring_radius() const
{
  static const float gem_size( get_config< float >( "gem-size" ) );
  
  const std::vector< m3::gem >& gems( m_ring.chain() );
  const std::size_t count( gems.size() );

  return count * gem_size / m3::math::pi_times_2;
}

float m3::app::gem_ring::get_expansion_rate() const
{
  return get_expansion_speed() / m_radius;
}

float m3::app::gem_ring::get_expansion_speed() const
{
  static const float min_speed
    ( get_config< float >( "expansion.per-frame.min" ) );
  static const float max_speed
    ( get_config< float >( "expansion.per-frame.max" ) );
  static const float start_date
    ( get_config< float >( "expansion.speed-up.start-date" ) );
  static const float end_date
    ( get_config< float >( "expansion.speed-up.end-date" ) );

  return m3::math::linear_range_interpolation
    ( m_game_loop.date(), start_date, end_date, min_speed, max_speed );
}

void m3::app::gem_ring::update_glow()
{
  m3::gem_ring ring( m_ring );
  std::vector< std::size_t > inserted( ring.expand( 1 ) );
  const std::size_t inserted_count( inserted.size() );
  std::sort( inserted.begin(), inserted.end() );
  
  const std::vector< m3::gem > chain( ring.chain() );
  const std::size_t count( chain.size() );
  
  m_glowing.clear();
  m_glowing.resize( count, 0 );
  m_glow.resize( count, 0 );

  std::size_t j( 0 );
  std::size_t offset( 0 );
  
  for ( std::size_t i
          : m3::find_adjacent_ring_gems( chain, m_game_loop.match_size() ) )
    {
      const bool skip( ( j < inserted_count ) && ( i == inserted[ j ] ) );
      m_glowing[ i - offset ] = !skip;
      offset += skip;
      j += skip;
    }

  static const float glow_step( get_config< float >( "glow-per-frame" ) );

  for ( std::size_t i( 0 ); i != count; ++i )
    m_glow[ i ] = m_glowing[ i ] * std::min( 1.0f, m_glow[ i ] + glow_step );
}
