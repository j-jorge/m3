#include "m3/app/gem_ring.hpp"

#include "m3/app/config/get_config.hpp"

#include "engine/level_globals.hpp"
#include "visual/scene_sprite.hpp"

BASE_ITEM_EXPORT( gem_ring, m3::app );

m3::app::gem_ring::gem_ring()
  : m_date( 0 )
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
}

void m3::app::gem_ring::progress( bear::universe::time_type elapsed_time )
{
  super::progress( elapsed_time );

  ++m_date;
  m_ring.set_orientation( get_system_angle() );
  m_ring.expand( get_expansion_rate() );

  // handle the ring's state
}

void m3::app::gem_ring::get_visual
( std::list< bear::engine::scene_visual >& visuals ) const
{
  super::get_visual( visuals );

  // TODO: scale the sprites according to the size of the ring
  static const float gem_size( get_config< float >( "gem-size" ) );
  static const float half_gem_size( gem_size / 2 );
  
  // TODO: set this constant in m3::match::pi_times_2
  static constexpr float pi_times_2( 2 * 3.14159265358979323846 );

  // TODO: insert the placeholders
  const std::vector< m3::gem >& gems( m_ring.chain() );
  const std::size_t count( gems.size() );
  const float radius( count * gem_size / pi_times_2 );
  const float orientation( m_ring.get_orientation() );

  const bear::universe::position_type center( get_center_of_mass() );
  
  for ( std::size_t i( 0 ); i != count; ++i )
    {
      const float a( orientation + i * pi_times_2 / count );
      const float x( center.x + std::cos( a ) * radius - half_gem_size );
      const float y( center.y + std::sin( a ) * radius - half_gem_size );

      visuals.push_back
        ( bear::visual::scene_sprite( x, y, m_gem_sprite[ gems[ i ] ] ) );
    }
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
  const m3::gem first_gem( std::rand() % gem_type_count + 1 );
  m_ring.launch( 0, first_gem );

  m3::gem last_gem( first_gem );
  unsigned int last_count( 1 );
  
  for ( unsigned int i( 0 ); i != count - 2; ++i )
    {
      m3::gem gem;
      bool found( false );
      
      while ( !found )
        {
          gem = std::rand() % gem_type_count + 1;

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

  m3::gem final_gem( std::rand() % gem_type_count + 1 );

  if ( final_gem == first_gem )
    final_gem = ( first_gem + 1 ) % gem_type_count + 1;

  m_ring.launch( 0, final_gem );
  
  m_ring.expand( 1 );
}

float m3::app::gem_ring::get_expansion_rate() const
{
  static const float min_rate
    ( get_config< float >( "expansion-rate.min" ) );
  static const float frame_duration
    ( get_config< float >( "frame_duration.seconds" ) );
  static const float start_date
    ( get_config< float >( "expansion-rate.start_date.seconds" )
      * frame_duration );

  if ( m_date < start_date )
    return min_rate;
  
  static const float max_rate
    ( get_config< float >( "expansion-rate.max" ) );
  static const std::uint64_t duration
    ( get_config< unsigned int >( "expansion-rate.duration.seconds" )
      * frame_duration );

  if ( m_date > start_date + duration )
    return max_rate;
  
  return min_rate + ( m_date - start_date ) * ( max_rate - min_rate );
}
