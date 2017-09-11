#include "m3/app/gem_ring.hpp"

#include "m3/app/create_level.hpp"
#include "m3/app/config/get_config.hpp"

#include "m3/game/find_adjacent_ring_gems.hpp"
#include "m3/math/linear_range_interpolation.hpp"
#include "m3/math/pi_times_2.hpp"

#include "engine/level_globals.hpp"
#include "engine/game.hpp"
#include "visual/scene_sprite.hpp"
#include "visual/scene_star.hpp"

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
      static float get_radius_for_gem_count( std::size_t count );
      static bear::engine::scene_visual create_game_over_visual
      ( const bear::universe::position_type& center, const float size,
        const bear::visual::color_type& color );

      namespace state
      {
        static constexpr int radius_animation = 1;
        static constexpr int expand = 2;
      }
    }
  }
}
        
m3::app::gem_ring::gem_ring()
  : m_win_size( get_config< unsigned int >( "winning-ring-size" ) ),
    m_lose_size( get_config< unsigned int >( "losing-ring-size" ) ),
    m_game_loop
    ( m_ring,
      m3::game::gem_generator
      ( get_config< unsigned int >( "gem-type-count" ) ) ),
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
  fill_game_over_bounds_visuals();
  
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

  get_game_over_bounds_visuals( visuals );
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

  m_game_loop.launch_count_interval_range
    ( get_config< unsigned int >( "launch-count-interval.min" ),
      get_config< unsigned int >( "launch-count-interval.max" ) );
  
  m_game_loop.launch_count_increment_time_range
    ( get_config< float >( "launch-count-increment.start-date" ),
      get_config< float >( "launch-count-increment.end-date" ) );

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

void m3::app::gem_ring::fill_game_over_bounds_visuals()
{
  m_game_over_bounds_visuals.reserve( 2 );

  static const bear::visual::color_type win_color( "#00e000" );
  static const bear::visual::color_type lose_color( "#e00000" );
  
  const float win_size
    ( 2 * detail::get_radius_for_gem_count( m_win_size ) );
  const float lose_size
    ( 2 * detail::get_radius_for_gem_count( m_lose_size ) );
  
  const bear::universe::position_type center( get_center_of_mass() );
  
  m_game_over_bounds_visuals.emplace_back
    ( detail::create_game_over_visual( center, win_size, win_color ) );
  m_game_over_bounds_visuals.emplace_back
    ( detail::create_game_over_visual( center, lose_size, lose_color ) );
}

void m3::app::gem_ring::get_game_over_bounds_visuals
( std::list< bear::engine::scene_visual >& visuals ) const
{
  visuals.insert
    ( visuals.end(), m_game_over_bounds_visuals.begin(),
      m_game_over_bounds_visuals.end() );
}

void m3::app::gem_ring::get_ring_visuals
( std::list< bear::engine::scene_visual >& visuals ) const
{
  const std::vector< m3::game::gem >& gems( m_ring.chain() );
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
    {
      const float glow( m_glow[ i ] );
      
      if ( glow > 0 )
        {
          const bear::visual::sprite& s( *glow_sprites[ i ] );
          const float x( center[ j ] - s.width() / 2 );
          const float y( center[ j + 1 ] - s.height() / 2 );

          bear::visual::scene_sprite element( x, y, s );
          element.get_rendering_attributes().set_opacity( glow );
          visuals.push_back( element );
        }
    }
}

void m3::app::gem_ring::get_launched_visuals
( std::list< bear::engine::scene_visual >& visuals ) const
{
  const std::vector< m3::game::gem >& gems( m_ring.free_gems() );
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

  const std::size_t count( m_ring.chain().size() );

  if ( ( count <= m_win_size ) || ( count >= m_lose_size ) )
    bear::engine::game::get_instance().set_waiting_level
      ( create_level() );
  else
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

  float scale( 1 );
  
  for ( const m3::game::gem& g : m_game_loop.coming_next() )
    {
      bear::visual::sprite s( m_gem_sprite[ g ] );

      s.set_width( s.width() * scale );
      s.set_height( s.height() * scale );
      
      m_coming_next_visuals.emplace_back
        ( bear::visual::scene_sprite
          ( center.x - s.width() / 2, center.y - s.height() / 2, s ) );
      scale *= 0.7;
    }
}

float m3::app::gem_ring::get_ring_radius() const
{
  return detail::get_radius_for_gem_count( m_ring.chain().size() );
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
  m3::game::gem_ring ring( m_ring );

  const std::size_t old_count( ring.chain().size() );
  
  std::vector< bool > glowing( old_count, false );
  m_glow.resize( old_count, 0 );

  std::vector< std::size_t > inserted( ring.expand( 1 ) );
  const std::size_t new_count( ring.chain().size() );
  const std::size_t inserted_count( inserted.size() );
  std::sort( inserted.begin(), inserted.end() );

  std::vector< std::size_t > index( new_count );
  const std::size_t guard( new_count );
  std::size_t j( 0 );
  std::size_t offset( 0 );
  
  for ( std::size_t i( 0 ); i != new_count; ++i )
    {
      const bool skip( ( j != inserted_count ) && ( i == inserted[ j ] ) );
      offset += skip;
      j += skip;
      index[ i ] = skip * guard + !skip * ( i - offset );
    }

  std::vector< std::size_t > removed;
  const std::vector< m3::game::gem >* chain;

  const unsigned int match_size( m_game_loop.match_size() );
  
  do
    {
      chain = &ring.chain();
      assert( index.size() == chain->size() );
      removed = m3::game::find_adjacent_ring_gems( *chain, match_size );

      const auto rend( removed.rend() );
      
      for ( auto it( removed.rbegin() ); it != rend; ++it )
        {
          const auto rit( index.begin() + *it );

          glowing[ *rit ] = ( *rit != guard );
          index.erase( rit );
        }

      ring.erase( removed );
      assert( index.size() == ring.chain().size() );
    }
  while( !removed.empty() );

  static const float glow_step( get_config< float >( "glow-per-frame" ) );

  for ( std::size_t i( 0 ); i != old_count; ++i )
    m_glow[ i ] = glowing[ i ] * std::min( 1.0f, m_glow[ i ] + glow_step );
}

float m3::app::detail::get_radius_for_gem_count( std::size_t count )
{
  static const float gem_size( get_config< float >( "gem-size" ) );
  
  return count * gem_size / m3::math::pi_times_2;
}

bear::engine::scene_visual m3::app::detail::create_game_over_visual
( const bear::universe::position_type& center, const float size,
  const bear::visual::color_type& color )
{
  const float half_size( size / 2 );
  
  bear::visual::scene_star result
    ( center.x - half_size, center.y - half_size, color,
      bear::visual::star( 30, 1 ), 3 );

  result.get_rendering_attributes().set_size( size, size );

  return result;
}
